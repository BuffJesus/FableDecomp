"""DRAFT: original UID iterator erase and genuine children; root reviews before run.
No compiler/source candidate, no range/full-clear oracle, no invalid/end/null domain.
"""
import argparse
import ast
import hashlib
import itertools
import json
from pathlib import Path
import struct
import sys
import traceback
import pefile
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import (UC_X86_REG_EAX, UC_X86_REG_EBX, UC_X86_REG_ECX,
    UC_X86_REG_EDX, UC_X86_REG_EBP, UC_X86_REG_ESI, UC_X86_REG_EDI,
    UC_X86_REG_ESP, UC_X86_REG_EIP)

HERE = Path(__file__).resolve().parent
ROOT = next(p for p in HERE.parents if (p / 'rebuild').is_dir())
sys.path.insert(0, str(ROOT / 'tools/decomp_pipeline'))
from pe_oracle import EXE, read_virtual_data

A, N, STACK, STOP = 0x2000000, 32768, 0x3000000, 0x4000000
ENTRY, REBAL, MINIMUM, MAXIMUM, LEFT, RIGHT, FREE = 0x990430, 0x42a34e, 0x41c7e0, 0x41c800, 0x42951b, 0x42955b, 0xbfea14
SPECS = {
    ENTRY: (50, '8d69723a76abb818dd2edc506c13672199979e656f7d1dcd9b615c4b79f7709c'),
    REBAL: (572, '70282448d15fc629d1d0f095ba566a159287c524ef047bed39c5c5f5a135e165'),
    MINIMUM: (26, 'ea57a9b278610718140d605034574268c88ed19a3f81e95078850bae76f21237'),
    MAXIMUM: (26, 'b02bc0e564a1cbd9f764212e08b65819569d8585b885c6b403bd6978fea9ed5a'),
    LEFT: (64, 'c44a29a3cd7e7ffa4ccb2dd5100a3b75d5577bf570984ee18c82149b9a7ac65b'),
    RIGHT: (64, '5da67519fdaf57f135fac9f30778b434763f172765a7f2b0f56ae0c675831015'),
}
MODEL = ROOT / 'rebuild/tests/integration/NativeAsyncPointerTreeRangeErase_model.py'


def digest(raw):
    return hashlib.sha256(raw).hexdigest()


def word(value):
    return struct.pack('<I', value & 0xffffffff)


class Memory:
    def __init__(self, raw=None, uc=None):
        self.raw, self.uc = raw, uc
    def read(self, address, size):
        assert size >= 0 and A <= address and address + size <= A + N
        return bytes(self.raw[address-A:address-A+size]) if self.raw is not None else bytes(self.uc.mem_read(address, size))
    def write(self, address, value):
        assert A <= address and address + len(value) <= A + N
        if self.raw is not None:
            self.raw[address-A:address-A+len(value)] = value
        else:
            self.uc.mem_write(address, value)
    def put(self, address, value):
        self.write(address, word(value))
    def w(self, address):
        return struct.unpack('<I', self.read(address, 4))[0]
    def snap(self):
        return self.read(A, N)


def independent_definitions():
    # Reuse only the separately authored abstract insertion/CLRS physical deletion.
    # Do not import the executable module, bodies, range expected(), or full-clear.
    parsed = ast.parse(MODEL.read_text())
    wanted = ('Node', 'Tree', 'validate_arena', 'remove')
    definitions = [node for node in parsed.body if isinstance(node, (ast.ClassDef, ast.FunctionDef)) and node.name in wanted]
    assert tuple(node.name for node in definitions) == wanted
    namespace = {}
    exec(compile(ast.Module(body=definitions, type_ignores=[]), str(MODEL), 'exec'), namespace)
    return namespace


def free_callback(memory, case, address, events, snapshots):
    assert address == case['erased_node']
    events.append(('free', address))
    snapshots.append(memory.snap())
    # Observable instrumentation only; physical CRT allocation/free is not simulated.
    memory.put(case['marker'], memory.w(case['marker']) + 1)
    mutation = case['mutation']
    if mutation == 'count_add7':
        memory.put(case['owner'] + 4, memory.w(case['owner'] + 4) + 7)
    elif mutation == 'count_zero':
        memory.put(case['owner'] + 4, 0)
    elif mutation == 'count_max':
        memory.put(case['owner'] + 4, 0xffffffff)
    elif mutation == 'detached_links_poison':
        # Only already detached/freed node links; live tree, pair and tails stay intact.
        memory.put(address + 8, 0xdeadbeef)
        memory.put(address + 12, 0xbaadf00d)
    else:
        assert mutation == 'none'


def make_case(defs, sequence, mapping, place, fill, key, mutation):
    initial = bytearray([fill]) * N
    memory, tree = Memory(initial), defs['Tree']()
    base = A + place * 0x3000
    for i, value in enumerate(sequence):
        node, inserted = tree.insert(mapping[value], base + 0x1000 + i * 32)
        assert inserted
        # pair<const ulong,CFileObject*>: mapped pointer at+20, detached pointee
        # resides in a different arena region; eight trailing node bytes are distinct.
        pointee = base + 0x2000 + i * 64
        memory.put(node.address + 20, pointee)
        memory.write(node.address + 24, bytes(((i * 29 + j * 17 + 0xa3) & 255) for j in range(8)))
        memory.write(pointee, bytes(((i * 37 + j * 11 + 0x61) & 255) for j in range(64)))
    owner, header = base + 0x100, base + 0x200
    tree.serialize(memory, owner, header)
    selected = tree.nodes[key]
    case = dict(owner=owner, header=header, marker=base + 0x500, key=key,
        erased_node=selected.address, nodeaddresses=[n.address for n in tree.nodes.values()],
        pointees=[memory.w(n.address + 20) for n in tree.nodes.values()],
        remaining=set(tree.nodes) - {key}, mutation=mutation)
    return bytes(initial), tree, case


def expected(defs, initial, tree, case):
    memory = Memory(bytearray(initial))
    events, snapshots = [], [memory.snap()]
    selected = tree.nodes[case['key']]
    predicted = {hex(address): 0 for address in SPECS}
    predicted[hex(ENTRY)] = predicted[hex(REBAL)] = 1
    predicted[hex(MINIMUM)] = int(selected is tree.nodes[min(tree.nodes)] and selected.right is not None)
    predicted[hex(MAXIMUM)] = int(selected is tree.nodes[max(tree.nodes)] and selected.left is not None)
    ordinary_rotate = tree.rotate
    def tracked_rotate(node, left):
        predicted[hex(LEFT if left else RIGHT)] += 1
        return ordinary_rotate(node, left)
    tree.rotate = tracked_rotate
    count = memory.w(case['owner'] + 4)
    detached = defs['remove'](tree, case['key'])
    # Serialization models changed live links/colors only, retaining original pair,
    # padding, header tail and detached node links. It cannot imply clear()/range path.
    tree.serialize(memory, case['owner'], case['header'])
    memory.put(case['owner'] + 4, count)
    memory.write(detached.address, bytes([int(detached.black)]))
    free_callback(memory, case, detached.address, events, snapshots)
    memory.put(case['owner'] + 4, memory.w(case['owner'] + 4) - 1)
    snapshots.append(memory.snap())
    return events, snapshots, predicted


class Runner:
    def __init__(self, bodies, defs):
        self.defs, self.bodies = defs, bodies
        uc = self.uc = Uc(UC_ARCH_X86, UC_MODE_32)
        for page in {address & ~4095 for address in (*bodies, FREE)}:
            uc.mem_map(page, 4096)
        uc.mem_map(A, N)
        uc.mem_map(STACK, 65536)
        uc.mem_map(STOP, 4096)
        for address, body in bodies.items():
            uc.mem_write(address, body)
        self.cpu = uc.context_save()
        self.active = None
        self.hits = {hex(address): 0 for address in bodies}
        uc.hook_add(UC_HOOK_CODE, lambda machine, pc, size, _: self.active(machine, pc))
    def run(self, initial, case):
        uc = self.uc
        uc.context_restore(self.cpu)
        uc.mem_write(A, initial)
        uc.mem_write(STACK, bytes(65536))
        memory = Memory(uc=uc)
        stack = STACK + 0x8000
        uc.mem_write(stack, word(STOP) + word(case['erased_node']))
        uc.reg_write(UC_X86_REG_ESP, stack)
        uc.reg_write(UC_X86_REG_ECX, case['owner'])
        registers = (UC_X86_REG_EBX, UC_X86_REG_EBP, UC_X86_REG_ESI, UC_X86_REG_EDI)
        for i, register in enumerate(registers):
            uc.reg_write(register, 0x11223300 + i)
        events, snapshots = [], [memory.snap()]
        old_hits = dict(self.hits)
        free_calls = 0
        def hook(machine, pc):
            nonlocal free_calls
            if pc == STOP:
                machine.emu_stop()
                return
            if hex(pc) in self.hits:
                self.hits[hex(pc)] += 1
                if pc == REBAL:
                    at = machine.reg_read(UC_X86_REG_ESP)
                    assert struct.unpack('<IIII', machine.mem_read(at + 4, 16)) == (case['erased_node'], case['header'] + 4, case['header'] + 8, case['header'] + 12)
            if pc != FREE:
                assert any(address <= pc < address + len(body) for address, body in self.bodies.items()), ('Execution escaped reviewed genuine bodies', hex(pc))
                return
            free_calls += 1
            at = machine.reg_read(UC_X86_REG_ESP)
            address = struct.unpack('<I', machine.mem_read(at + 4, 4))[0]
            free_callback(memory, case, address, events, snapshots)
            return_address = struct.unpack('<I', machine.mem_read(at, 4))[0]
            machine.reg_write(UC_X86_REG_EAX, 0xabcdef00)
            machine.reg_write(UC_X86_REG_ECX, 0x98765432)
            machine.reg_write(UC_X86_REG_EDX, 0x87654321)
            machine.reg_write(UC_X86_REG_ESP, at + 4)  # cdecl: caller releases argument.
            machine.reg_write(UC_X86_REG_EIP, return_address)
        self.active = hook
        uc.emu_start(ENTRY, STOP, count=20000)
        assert uc.reg_read(UC_X86_REG_EIP) == STOP
        assert uc.reg_read(UC_X86_REG_ESP) == stack + 8  # original root RET4.
        assert [uc.reg_read(r) for r in registers] == [0x11223300+i for i in range(4)]
        assert free_calls == 1 and events == [('free', case['erased_node'])]
        class StructuralView:
            def w(self, address):
                return len(case['remaining']) if address == case['owner'] + 4 else memory.w(address)
            def read(self, address, size):
                return memory.read(address, size)
        # Structural cardinality view is used only for deliberately mutated count
        # supplements; full arena snapshots always retain the exact raw count.
        view = memory if case['mutation'] in ('none', 'detached_links_poison') else StructuralView()
        self.defs['validate_arena'](view, case['owner'], case['header'], case['remaining'])
        for address in case['nodeaddresses']:
            assert memory.read(address + 16, 16) == initial[address-A+16:address-A+32], 'Pair payload/trailing node bytes changed'
        for pointee in case['pointees']:
            assert memory.read(pointee, 64) == initial[pointee-A:pointee-A+64], 'Map eraser modified mapped CFileObject pointee'
        snapshots.append(memory.snap())
        return events, snapshots, {name: self.hits[name] - old_hits[name] for name in self.hits}



def validate(bodies, out, mode):
    defs = independent_definitions()
    runner = Runner(bodies, defs)
    traces = []
    current_case = None
    def save(name, value):
        (out / (mode + "_" + name)).write_text(json.dumps(value, indent=2) + "\n")
    sequences = [(0,), (0,1), (1,0), *itertools.permutations(range(4)), tuple(range(8)), tuple(reversed(range(8))), (4,2,6,1,3,5,7,0), (0,7,1,6,2,5,3,4)]
    mappings = [tuple(range(8)), (0,1,2,0x7ffffffe,0x7fffffff,0x80000000,0xfffffffe,0xffffffff)]
    counts, snapshots, partitions = 0, 0, {}
    def check(sequence, mapping, place, fill, key, mutation, phase):
        nonlocal current_case, counts, snapshots
        current_case = dict(sequence=list(sequence), mapping=list(mapping), place=place, fill=fill, key=key, mutation=mutation, phase=phase)
        initial, tree, case = make_case(defs, sequence, mapping, place, fill, key, mutation)
        wanted = expected(defs, initial, tree, case)
        got = runner.run(initial, case)
        if got != wanted:
            save('first_difference.json', dict(case=current_case, actual_events=got[0], expected_events=wanted[0], actual_hits=got[2], expected_hits=wanted[2], actual_snapshots=[s.hex() for s in got[1]], expected_snapshots=[s.hex() for s in wanted[1]]))
        assert got == wanted, current_case
        assert len(got[1]) == 3
        traces.append(dict(case=current_case, provider_hits=got[2], full_arena_snapshot_sha256=[digest(s) for s in got[1]]))
        counts += 1
        snapshots += 3
        partitions[mutation] = partitions.get(mutation, 0) + 1
    for sequence, mapping, place, fill in itertools.product(sequences, mappings, range(2), (0,0x95)):
        for key in sorted(mapping[i] for i in sequence):
            check(sequence, mapping, place, fill, key, 'none', 'valid-single-iterator')
    assert counts == 1064 and snapshots == 3192
    baseline = dict(cases=counts, snapshots=snapshots, provider_hits=dict(runner.hits))
    for sequence, mapping, place, fill, mutation in itertools.product(sequences[-2:], mappings, range(2), (0,0x95), ('count_add7', 'count_zero', 'count_max', 'detached_links_poison')):
        for key in sorted(mapping[i] for i in sequence):
            check(sequence, mapping, place, fill, key, mutation, 'disclosed-free-callback-supplement')
    assert counts == 1576 and snapshots == 4728
    assert partitions == {'none':1064, 'count_add7':128, 'count_zero':128, 'count_max':128, 'detached_links_poison':128}
    assert runner.hits[hex(ENTRY)] == counts and runner.hits[hex(REBAL)] == counts
    assert all(value > 0 for value in baseline['provider_hits'].values())
    assert runner.hits == {hex(ENTRY):1576, hex(REBAL):1576, hex(MINIMUM):56, hex(MAXIMUM):56, hex(LEFT):208, hex(RIGHT):416}
    save("case_traces.json", traces)
    return dict(cases=counts, snapshots=snapshots, provider_hits=runner.hits, partitions=partitions, baseline=baseline)
