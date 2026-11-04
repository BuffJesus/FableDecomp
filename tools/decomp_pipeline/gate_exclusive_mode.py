"""Validate the native exclusive-mode helper without changing host priority/display."""
import argparse
import hashlib
import itertools
import json
import struct
import subprocess
import tempfile
from pathlib import Path

import pefile
import check_one
from gate_game_component_abi import Coff
from near_match import compare, retail_relocations
from pe_oracle import EXE
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import *

ROOT = Path(__file__).resolve().parents[2]
ENTRY = 0x009a59a0
SYSTEM = 0x02000000
CALLBACK = 0x02100000
STOP = CALLBACK+0x100
STACK = 0x03000000
SYMBOL = '?SetExclusiveMode@CSystemManager@@QAE_N_N@Z'
TARGETS = {'__imp__GetCurrentProcess@0': 0x0143fd6c,
           '__imp__SetPriorityClass@8': 0x0143fd70,
           '?SetExclusiveMode@CDisplayManager@@QAE?AW4ECreateDeviceSuccess@1@_N@Z': 0x009c0020}


def execute(code, prevent, previous, desired, have_input, input_mask, mutate,
            priority_result, display_result):
    uc = Uc(UC_ARCH_X86, UC_MODE_32)
    for address, size in [(ENTRY & ~4095, 4096), (0x009c0000, 4096),
                          (0x0143f000, 4096), (SYSTEM, 4096),
                          (CALLBACK, 4096), (STACK, 0x10000)]:
        uc.mem_map(address, size)
    uc.mem_write(ENTRY, bytes(code))
    def put(address, value): uc.mem_write(address, struct.pack('<I', value & 0xffffffff))
    def get(address): return struct.unpack('<I', uc.mem_read(address, 4))[0]
    def mode(): return uc.mem_read(SYSTEM+0x8e, 1)[0]
    input_device, input_table, display = SYSTEM+0x400, SYSTEM+0x500, SYSTEM+0x600
    uc.mem_write(SYSTEM, b'\xa5'*0x180)
    uc.mem_write(SYSTEM+0x8e, bytes([previous]))
    uc.mem_write(SYSTEM+0xed, bytes([prevent]))
    put(SYSTEM+0x58, input_device if have_input else 0)
    put(SYSTEM+0x60, display)
    put(input_device, input_table)
    put(input_table+4, CALLBACK+0x20)
    put(0x0143fd6c, CALLBACK)
    put(0x0143fd70, CALLBACK+0x10)
    before = bytes(uc.mem_read(SYSTEM, 4096))
    events = []
    priorities = 0
    returned = False
    def hook(uc, address, size, data):
        nonlocal priorities, returned
        if address == STOP:
            returned = True
            uc.emu_stop()
            return
        if address not in (CALLBACK, CALLBACK+0x10, CALLBACK+0x20, 0x009c0020): return
        sp = uc.reg_read(UC_X86_REG_ESP)
        if address == CALLBACK:
            argc, result = 0, 0x87654321
            events.append(('process',))
        elif address == CALLBACK+0x10:
            argc, result = 2, priority_result
            assert get(sp+4) == 0x87654321 and get(sp+8) == 0x20
            assert mode() == (desired if priorities == 0 else previous)
            priorities += 1
            events.append(('priority', mode(), priority_result))
        elif address == CALLBACK+0x20:
            argc = 1
            assert uc.reg_read(UC_X86_REG_ECX) == input_device
            assert get(sp+4) & 255 == mode()
            result = int(bool(input_mask & (1 << (priorities-1))))
            events.append(('input', mode(), result))
            if mutate: uc.mem_write(SYSTEM+0x8e, bytes([1-mode()]))
        else:
            argc, result = 1, display_result
            assert uc.reg_read(UC_X86_REG_ECX) == display
            assert get(sp+4) & 255 == mode()
            events.append(('display', mode(), result))
        ret = get(sp)
        uc.reg_write(UC_X86_REG_EAX, result)
        uc.reg_write(UC_X86_REG_ECX, 0xdead0001)
        uc.reg_write(UC_X86_REG_EDX, 0xdead0002)
        uc.reg_write(UC_X86_REG_ESP, sp+4+argc*4)
        uc.reg_write(UC_X86_REG_EIP, ret)
    uc.hook_add(UC_HOOK_CODE, hook)
    initial_sp = STACK+0x8000
    put(initial_sp, STOP)
    put(initial_sp+4, desired)
    uc.reg_write(UC_X86_REG_ESP, initial_sp)
    uc.reg_write(UC_X86_REG_ECX, SYSTEM)
    preserved = [UC_X86_REG_EBX, UC_X86_REG_EBP, UC_X86_REG_ESI, UC_X86_REG_EDI]
    for i, reg in enumerate(preserved): uc.reg_write(reg, 0xabc000+i)
    uc.emu_start(ENTRY, STOP+1, count=10000)
    assert returned and (uc.reg_read(UC_X86_REG_EAX) & 255) == 0
    assert uc.reg_read(UC_X86_REG_ESP) == initial_sp+8
    for i, reg in enumerate(preserved): assert uc.reg_read(reg) == 0xabc000+i
    expected = bytearray(before)
    if not prevent and have_input and mutate: expected[0x8e] = 1-previous
    after = bytes(uc.mem_read(SYSTEM, 4096))
    assert after == expected and priorities == (0 if prevent else 2)
    return events, after.hex()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, default=ROOT/'rebuild/src/compiled/00/9a/CSystemManager_SetExclusiveMode_009a59a0.cpp')
    parser.add_argument('--test', type=Path, default=ROOT/'rebuild/tests/00/9a/CSystemManager_SetExclusiveMode_009a59a0_test.cpp')
    parser.add_argument('--include', type=Path, default=ROOT/'rebuild/include')
    parser.add_argument('--output', type=Path, default=ROOT/'rebuild/compile-gate/exclusive-mode.json')
    args = parser.parse_args()
    work = Path(tempfile.mkdtemp(prefix='exclusive-mode-'))
    env = check_one.env()
    env['INCLUDE'] = str(args.include.resolve())+';'+env['INCLUDE']
    obj, test_obj = work/'source.obj', work/'fixture.obj'
    for source, target, optimization in [(args.source, obj, '/O2'), (args.test, test_obj, '/Od')]:
        result = check_one.cl(['/nologo','/c',optimization,'/Oy','/GS','/MT','/Fo'+str(target),str(source.resolve())], env, work)
        assert result.returncode == 0, result.stdout+result.stderr
    coff = Coff(obj)
    section, offset = coff.symbols[SYMBOL]
    built, relocs = coff.sections[section]
    assert offset == 0 and len(relocs) == 3
    with pefile.PE(str(EXE), fast_load=True) as pe:
        retail = pe.get_data(ENTRY-pe.OPTIONAL_HEADER.ImageBase, 137)
    native_relocs = retail_relocations('009a59a0', len(retail))
    score = compare(built, retail, relocs, native_relocs)
    assert score.near_match, score
    # Anchor every retail operand independently: candidate instruction offsets
    # differ. PE import lookup and FableWin establish these three identities.
    assert int.from_bytes(retail[0x1d:0x21], 'little') == TARGETS['__imp__GetCurrentProcess@0']
    assert int.from_bytes(retail[0x23:0x27], 'little') == TARGETS['__imp__SetPriorityClass@8']
    assert ENTRY+0x79+int.from_bytes(retail[0x75:0x79], 'little', signed=True) == 0x009c0020
    candidate = bytearray(built)
    checked = []
    for site, (symbol, kind) in relocs.items():
        target = TARGETS[symbol]
        assert kind == (6 if symbol.startswith('__imp__') else 20)
        assert int.from_bytes(built[site:site+4], 'little') == 0
        value = target if kind == 6 else target-(ENTRY+site+4)
        struct.pack_into('<I', candidate, site, value & 0xffffffff)
        checked.append({'offset': site, 'symbol': symbol, 'target': f'{target:08x}'})
    cases = []
    dimensions = [range(2), range(2), range(2), range(2), range(4), range(2), range(2), range(3)]
    for scenario in itertools.product(*dimensions):
        native = execute(retail, *scenario)
        recovered = execute(candidate, *scenario)
        assert native == recovered, scenario
        cases.append({'scenario': scenario, 'events': native[0]})
    exe = work/'fixture.exe'
    command = [str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    standalone = subprocess.run(command+[str(test_obj)], env=env, capture_output=True, text=True)
    assert standalone.returncode != 0, 'fixture must require candidate'
    linked = subprocess.run(command+[str(obj),str(test_obj)], env=env, capture_output=True, text=True)
    assert linked.returncode == 0, linked.stdout+linked.stderr
    tested = subprocess.run([str(exe)], env=env, capture_output=True, text=True, timeout=30)
    assert tested.returncode == 0 and 'EXCLUSIVE_MODE_BEHAVIOR_PASS scenarios=768' in tested.stdout, tested.stdout+tested.stderr
    grade = 'RELOCATION_MATCH' if candidate == retail else 'NEAR_MATCH'
    report = {'address': f'{ENTRY:08x}', 'grade': grade, 'retail_bytes': 137,
              'built_bytes': len(built), 'similarity': score.to_dict(), 'relocations': checked,
              'source_sha256': hashlib.sha256(args.source.read_bytes()).hexdigest(),
              'fixture_sha256': hashlib.sha256(args.test.read_bytes()).hexdigest(),
              'behavior': tested.stdout.strip(), 'comparative_scenarios': cases}
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2)+'\n')
    print('EXCLUSIVE_MODE_PASS grade='+grade+' retail=137 built='+str(len(built))+' compiled=768 comparative='+str(len(cases)))


if __name__ == '__main__': main()
