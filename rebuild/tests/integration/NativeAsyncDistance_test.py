"""Execute first compiled helper and retail independently against sorted ranges."""
import hashlib
import importlib.util
import itertools
import json
import struct
import sys
from pathlib import Path
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EIP, UC_X86_REG_ESP, UC_X86_REG_EAX, UC_X86_REG_EBX, UC_X86_REG_EBP, UC_X86_REG_ESI, UC_X86_REG_EDI

ROOT = Path(__file__).resolve().parents[3]
WORK = Path(__file__).parent
sys.path.insert(0, str(ROOT / 'tools/decomp_pipeline'))
from pe_oracle import EXE, read_virtual_data
BASE, ARENA, SIZE, STACK, STOP = 0x9d7550, 0x2000000, 32768, 0x3000000, 0x4000000



def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()

def main():
    import re, subprocess, tempfile
    import check_one as V
    from gate_game_component_abi import Coff
    build = Path(tempfile.mkdtemp(prefix='native-async-distance-'))
    OUTPUT = build / 'result.json'
    source = ROOT / 'rebuild/src/compiled/00/9d/STLAsyncPointerTree_Distance_009d7550.cpp'
    required = WORK / 'NativeAsyncDistance_required.cpp'
    assert not re.search(r'__forceinline|__asm|__naked|#\s*pragma', source.read_text())
    inputs = {Path(__file__), source, required, EXE}
    inputs.update(p for p in (ROOT / 'rebuild/include').rglob('*') if p.is_file())
    inputs.update(p for folder in (V.VC / 'include', V.VC / 'lib') for p in folder.rglob('*') if p.is_file())
    inputs.update(V.VC / 'bin' / n for n in ('cl.exe','c1xx.dll','c2.dll','mspdb71.dll','link.exe'))
    inputs.update(ROOT / 'tools/decomp_pipeline' / n for n in ('check_one.py','gate_game_component_abi.py','verify_and_land.py','pe_oracle.py'))
    before = {str(p): sha(p) for p in sorted(inputs)}
    objects = []
    for index, path in enumerate((source, required)):
        obj = build / (str(index) + '.obj')
        result = V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(path)], V.env(), build)
        assert result.returncode == 0, result.stdout + result.stderr
        objects.append(obj)
    c = Coff(objects[0])
    roots = [name for name in c.symbols if name.startswith('??$__distance@')]
    assert len(roots) == 1, roots
    symbol = roots[0]
    section, offset = c.symbols[symbol]
    compiled, relocations = c.sections[section]
    original = read_virtual_data(EXE.read_bytes(), BASE, 78)
    assert offset == 0 and compiled == original and not relocations
    assert hashlib.sha256(original).hexdigest() == '578d974e61be9db592105b3cce5aa69314cfaa9a42ec44a6e2d2c81330ea6877'
    caller = Coff(objects[1])
    requests = {target for body, relocs in caller.sections.values() for target, kind in relocs.values() if target.startswith('??$__distance@')}
    assert requests == {symbol}
    links = {}
    for label, providers in (('omission', [objects[1]]), ('control', objects)):
        result = subprocess.run([str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(build/(label+'.exe'))]+list(map(str,providers)), cwd=build, env=V.env(),capture_output=True,text=True)
        links[label] = {'returncode':result.returncode,'stdout':result.stdout,'stderr':result.stderr}
    assert links['omission']['returncode'] and symbol in links['omission']['stdout']+links['omission']['stderr']
    assert links['control']['returncode'] == 0
    object_hashes = {str(p): sha(p) for p in objects}
    runners = []
    for name, code in (('original', original), ('compiled', compiled)):
        u = Uc(UC_ARCH_X86, UC_MODE_32)
        for p, n in ((BASE & ~4095, 4096), (ARENA, SIZE), (STACK, 65536), (STOP, 4096)):
            u.mem_map(p, n)
        u.mem_write(BASE, code)
        hits = [0]
        def hook(uc, pc, length, userdata, hits=hits):
            if pc == BASE:
                hits[0] += 1
        u.hook_add(UC_HOOK_CODE, hook)
        runners.append((name, u, u.context_save(), hits))
    keys = (0, 1, 0x100, 0x7fffffff, 0x80000000, 0xffffff00, 0xffffffff)
    orders = ((), (3,), (3, 1, 5, 0, 2, 4, 6), tuple(range(7)), tuple(reversed(range(7))))
    cases = 0
    distances = {}
    registers = (UC_X86_REG_EBX, UC_X86_REG_EBP, UC_X86_REG_ESI, UC_X86_REG_EDI)
    for order, place, fill, stackfill in itertools.product(orders, range(2), (0, 0x95), (0, 0x95)):
        origin = ARENA + place * 0x3000
        initial = bytearray([fill]) * SIZE
        def put(p, value):
            struct.pack_into('<I', initial, p - ARENA, value)
        def get(p):
            return struct.unpack_from('<I', initial, p - ARENA)[0]
        header = origin + 0x200
        initial[header - ARENA] = 0
        for offset, value in ((4, 0), (8, header), (12, header)):
            put(header + offset, value)
        nodes = {}
        for index, key_index in enumerate(order):
            node = origin + 0x400 + index * 32
            key = keys[key_index]
            nodes[key] = node
            parent, cursor = header, get(header + 4)
            while cursor:
                parent = cursor
                cursor = get(cursor + (8 if key < get(cursor + 16) else 12))
            for offset, value in ((4, parent), (8, 0), (12, 0), (16, key)):
                put(node + offset, value)
            put(parent + (4 if parent == header else (8 if key < get(parent + 16) else 12)), node)
            put(header + 8, nodes[min(nodes)])
            put(header + 12, nodes[max(nodes)])
        ordered = [nodes[key] for key in sorted(nodes)] + [header]
        for first_index in range(len(ordered)):
            for last_index in range(first_index, len(ordered)):
                for alias in (False, True):
                    if alias and first_index != last_index:
                        continue
                    firstp = origin + 0x1000
                    lastp = firstp if alias else firstp + 4
                    put(firstp, ordered[first_index])
                    put(lastp, ordered[last_index])
                    snapshot = bytes(initial)
                    expected = last_index - first_index
                    for name, u, cpu, hits in runners:
                        u.context_restore(cpu)
                        u.mem_write(ARENA, snapshot)
                        u.mem_write(STACK, bytes([stackfill]) * 65536)
                        sp = STACK + 0x8000
                        u.mem_write(sp, struct.pack('<IIII', STOP, firstp, lastp, origin + 0x1800))
                        u.reg_write(UC_X86_REG_ESP, sp)
                        for index, register in enumerate(registers):
                            u.reg_write(register, 0x12340000 + index)
                        u.emu_start(BASE, STOP, count=10000)
                        assert u.reg_read(UC_X86_REG_EIP) == STOP
                        assert u.reg_read(UC_X86_REG_ESP) == sp + 4
                        assert u.reg_read(UC_X86_REG_EAX) == expected, (name, order, first_index, last_index)
                        assert [u.reg_read(r) for r in registers] == [0x12340000 + i for i in range(4)]
                        assert bytes(u.mem_read(ARENA, SIZE)) == snapshot
                    cases += 1
                    distances[expected] = distances.get(expected, 0) + 1
    assert all(hits[0] == cases for name, u, cpu, hits in runners)
    assert all(sha(Path(p)) == digest for p, digest in object_hashes.items())
    after = {str(p): sha(p) for p in sorted(inputs)}
    assert before == after
    report = dict(status='NATIVE_ASYNC_DISTANCE_GATE_PASS', cases_per_mode=cases,
                  actual_root_entries={name: hits[0] for name, u, cpu, hits in runners}, distances=distances,
                  arena_bytes_per_snapshot=SIZE, full_arena_checks_per_mode=cases,
                  input_sha256_before=before, input_sha256_after=after,
                  boundary='Actual retail and freshly compiled canonical 78-byte distance bodies on separate machines. Independent sorted-node interval oracle, empty/singleton/balanced/skew BST shapes, high-bit unsigned keys, multiple arena/stack fills and placements, aliasing equal iterator inputs, cdecl RET0/callee saves and unchanged full arena. No keyerase/range deletion/free/RB color balancing or whole-engine claim.',
                  canonical_promoted=False, symbol=symbol, exact=True, links=links, object_sha256=object_hashes, build=str(build))
    with OUTPUT.open('x', encoding='utf-8') as stream:
        json.dump(report, stream, indent=2)
        stream.write('\n')
    print(report['status'], cases, distances)
    print('RECEIPT', OUTPUT)

if __name__ == '__main__':
    main()
