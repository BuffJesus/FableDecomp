"""Compare retail and candidate instructions with in-memory Win32 boundaries."""
from pathlib import Path
import argparse
import hashlib
import subprocess
import tempfile
import itertools
import json
import struct
import sys

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'tools/decomp_pipeline'))
from gate_game_component_abi import Coff
from native_gfmain_harness import PE
import check_one
from near_match import compare
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import *


ENTRY = 0x9a4f20
SYSTEM = 0x2000000
FILTER = 0x2100000
STOP = 0x2100100
STACK = 0x3000000
TARGETS = {'__imp__EnterCriticalSection@4': (0x143fe0c, 'enter', 1),
           '__imp__LeaveCriticalSection@4': (0x143fe10, 'leave', 1),
           '__imp__PeekMessageW@20': (0x1440370, 'peek', 5),
           '__imp__TranslateMessage@4': (0x144036c, 'translate', 1),
           '__imp__DispatchMessageW@4': (0x1440368, 'dispatch', 1)}


def run(code, mode, count, changes, null_window):
    uc = Uc(UC_ARCH_X86, UC_MODE_32)
    for address, size in [(ENTRY & ~4095, 4096), (0x143f000, 0x2000),
                          (SYSTEM, 4096), (FILTER, 4096), (STACK, 0x10000)]:
        uc.mem_map(address, size)
    uc.mem_write(ENTRY, bytes(code))
    def put(address, value): uc.mem_write(address, struct.pack('<I', value & 0xffffffff))
    def get(address): return struct.unpack('<I', uc.mem_read(address, 4))[0]
    hooks = {}
    for index, (slot, name, argc) in enumerate(TARGETS.values()):
        address = FILTER + 0x200 + index * 0x10
        put(slot, address)
        hooks[address] = (name, argc)
    uc.mem_write(SYSTEM, b'\xa5' * 0x180)
    put(SYSTEM + 0x94, 0 if null_window else 0x12345678)
    put(SYSTEM + 0x17c, FILTER if mode else 0)
    before = bytes(uc.mem_read(SYSTEM, 0x180))
    events = []
    queue = [struct.pack('<7I', 0x10203040+i, 0x12 if i == 2 else 0x100+i,
                         0xfedcba98+i, (-17-i) & 0xffffffff, 0x98765432+i,
                         (-100) & 0xffffffff, 200) for i in range(count)]
    position = 0
    locked = False
    returned = False
    def hook(uc, address, size, data):
        nonlocal position, locked, returned
        if address == STOP:
            returned = True
            uc.emu_stop()
            return
        if address != FILTER and address not in hooks: return
        sp = uc.reg_read(UC_X86_REG_ESP)
        result = 0
        if address == FILTER:
            argc = 0
            message = uc.reg_read(UC_X86_REG_ECX)
            assert locked and bytes(uc.mem_read(message, 28)) == queue[position-1]
            events.append(('filter', bytes(uc.mem_read(message, 28)).hex()))
            if changes:
                put(SYSTEM+0x94, 0x87654321)
                put(message+8, get(message+8) ^ 0x80000000)
            if changes == 2: put(SYSTEM+0x17c, 0)
            result = int(mode == 2 or (mode == 3 and get(message+4) & 1))
        else:
            name, argc = hooks[address]
            args = [get(sp+4+4*i) for i in range(argc)]
            if name == 'enter':
                assert not locked and args == [SYSTEM+0x34]
                locked = True
                events.append((name,))
            elif name == 'leave':
                assert locked and args == [SYSTEM+0x34]
                locked = False
                events.append((name,))
            elif name == 'peek':
                assert locked and args[1:] == [get(SYSTEM+0x94), 0, 0, 1]
                events.append((name, args[1]))
                if position < count:
                    uc.mem_write(args[0], queue[position])
                    position += 1
                    result = 1
            else:
                assert locked
                events.append((name, bytes(uc.mem_read(args[0], 28)).hex()))
                result = position & 1 if name == 'translate' else 0xffffffff
        ret = get(sp)
        # Volatile registers are unspecified by all these ABIs; clobber them.
        uc.reg_write(UC_X86_REG_EAX, result)
        uc.reg_write(UC_X86_REG_ECX, 0xdead0001)
        uc.reg_write(UC_X86_REG_EDX, 0xdead0002)
        uc.reg_write(UC_X86_REG_ESP, sp+4+argc*4)
        uc.reg_write(UC_X86_REG_EIP, ret)
    uc.hook_add(UC_HOOK_CODE, hook)
    initial_sp = STACK + 0x8000
    put(initial_sp, STOP)
    uc.reg_write(UC_X86_REG_ESP, initial_sp)
    uc.reg_write(UC_X86_REG_ECX, SYSTEM)
    preserved = [UC_X86_REG_EBX, UC_X86_REG_ESI, UC_X86_REG_EDI, UC_X86_REG_EBP]
    for i, reg in enumerate(preserved): uc.reg_write(reg, 0xabc000+i)
    uc.emu_start(ENTRY, STOP+1, count=10000)
    assert returned and not locked and position == count
    assert uc.reg_read(UC_X86_REG_ESP) == initial_sp+4
    for i, reg in enumerate(preserved): assert uc.reg_read(reg) == 0xabc000+i
    after = bytes(uc.mem_read(SYSTEM, 0x180))
    expected = bytearray(before)
    if count and mode and changes:
        struct.pack_into('<I', expected, 0x94, 0x87654321)
        if changes == 2: struct.pack_into('<I', expected, 0x17c, 0)
    assert after == expected
    return events, after.hex()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, default=ROOT/'rebuild/src/compiled/00/9a/CSystemManager_ProcessWindowMessages_009a4f20.cpp')
    parser.add_argument('--test', type=Path, default=ROOT/'rebuild/tests/00/9a/CSystemManager_ProcessWindowMessages_009a4f20_test.cpp')
    parser.add_argument('--include', type=Path, default=ROOT/'rebuild/include')
    parser.add_argument('--output', type=Path, default=ROOT/'rebuild/compile-gate/window-messages.json')
    args = parser.parse_args()
    work = Path(tempfile.mkdtemp(prefix='window-messages-'))
    env = check_one.env()
    env['INCLUDE'] = str(args.include.resolve())+';'+env['INCLUDE']
    obj = work/'candidate.obj'
    test_obj = work/'fixture.obj'
    for source, target, optimization in [(args.source, obj, '/O2'), (args.test, test_obj, '/Od')]:
        result = check_one.cl(['/nologo','/c',optimization,'/Oy','/GS','/MT','/Fo'+str(target),str(source.resolve())], env, work)
        assert result.returncode == 0, result.stdout+result.stderr
    exe = work/'fixture.exe'
    # Fixture must depend on the actual candidate, never pass by itself.
    standalone = subprocess.run([str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),str(test_obj)], env=env, capture_output=True, text=True)
    assert standalone.returncode != 0, 'self-contained test cannot prove candidate behavior'
    linked = subprocess.run([str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),str(obj),str(test_obj)], env=env, capture_output=True, text=True)
    assert linked.returncode == 0, linked.stdout+linked.stderr
    tested = subprocess.run([str(exe)], env=env, capture_output=True, text=True, timeout=30)
    assert tested.returncode == 0 and 'WINDOW_MESSAGE_LOOP_BEHAVIOR_PASS scenarios=96' in tested.stdout, tested.stdout+tested.stderr
    coff = Coff(obj)
    section, start = coff.symbols['?ProcessWindowMessages@CSystemManager@@AAEXXZ']
    built, relocs = coff.sections[section]
    retail = PE.get_data(ENTRY - PE.OPTIONAL_HEADER.ImageBase, 141)
    candidate = bytearray(built)
    assert start == 0 and len(candidate) == len(retail)
    for offset, (symbol, kind) in relocs.items():
        assert kind == 6
        slot = TARGETS[symbol][0]
        assert struct.unpack_from('<I', retail, offset)[0] == slot
        struct.pack_into('<I', candidate, offset, slot)
    cases = []
    for scenario in itertools.product(range(4), range(4), range(3), range(2)):
        native = run(retail, *scenario)
        recovered = run(candidate, *scenario)
        assert native == recovered, scenario
        cases.append(dict(scenario=scenario, events=native[0]))
    score = compare(built, retail, relocs, relocs)
    assert score.near_match, score
    grade = 'RELOCATION_MATCH' if candidate == retail else 'NEAR_MATCH'
    report = {'address': '009a4f20', 'grade': grade, 'bytes': len(built),
              'relocations': len(relocs), 'similarity': score.to_dict(),
              'source_sha256': hashlib.sha256(args.source.read_bytes()).hexdigest(),
              'fixture_sha256': hashlib.sha256(args.test.read_bytes()).hexdigest(),
              'behavior': tested.stdout.strip(), 'comparative_scenarios': cases}
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2)+'\n')
    print('WINDOW_MESSAGES_PASS grade='+grade+' bytes='+str(len(built))+' relocations='+str(len(relocs))+' compiled=96 comparative='+str(len(cases)))


if __name__ == '__main__': main()
