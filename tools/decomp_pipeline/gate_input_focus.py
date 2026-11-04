"""Check native input-focus dispatch and its recovered C++ against retail.

All device callbacks are isolated fixtures; no real device receives focus.
"""
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
from near_match import compare
from pe_oracle import EXE
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import *

ROOT = Path(__file__).resolve().parents[2]
ENTRY = 0x009f4e20
SYSTEM = 0x02000000
CALLBACK = 0x02100000
STOP = CALLBACK+0x100
STACK = 0x03000000
SYMBOL = '?SetHasInputFocus@CInputManager@@QAEX_N@Z'


def execute(code, focused, have_mouse, have_keyboard, count, mutation):
    uc = Uc(UC_ARCH_X86, UC_MODE_32)
    for address, size in [(ENTRY & ~4095, 4096), (SYSTEM, 4096),
                          (CALLBACK, 4096), (STACK, 0x10000)]:
        uc.mem_map(address, size)
    uc.mem_write(ENTRY, bytes(code))
    def put(address, value): uc.mem_write(address, struct.pack('<I', value & 0xffffffff))
    def get(address): return struct.unpack('<I', uc.mem_read(address, 4))[0]
    mouse, keyboard, alt_keyboard = SYSTEM+0x100, SYSTEM+0x110, SYSTEM+0x120
    joysticks = [SYSTEM+0x180+i*0x10 for i in range(6)]
    original, alternate = SYSTEM+0x400, SYSTEM+0x440
    mouse_table, keyboard_table, joystick_table = SYSTEM+0x600, SYSTEM+0x680, SYSTEM+0x700
    uc.mem_write(SYSTEM, b'\xa5'*0x60)
    put(SYSTEM+0x24, mouse if have_mouse else 0)
    put(SYSTEM+0x28, keyboard if have_keyboard else 0)
    put(SYSTEM+0x2c, original)
    put(SYSTEM+0x30, original+8*count)
    put(SYSTEM+0x34, original+24)
    for obj, table in [(mouse, mouse_table), (keyboard, keyboard_table), (alt_keyboard, keyboard_table)]:
        put(obj, table)
    for obj in joysticks: put(obj, joystick_table)
    put(mouse_table+0x38, CALLBACK)
    put(keyboard_table+0x0c, CALLBACK+0x10)
    put(joystick_table+0x40, CALLBACK+0x20)
    for i in range(3):
        put(original+8*i, joysticks[i])
        put(original+8*i+4, 0xdead0000+i)
        put(alternate+8*i, joysticks[i+3])
        put(alternate+8*i+4, 0xbeef0000+i)
    expected = bytearray(uc.mem_read(SYSTEM, 4096))
    def change(offset, value):
        put(SYSTEM+offset, value)
        struct.pack_into('<I', expected, offset, value)
    def change_vector(length):
        change(0x2c, alternate)
        change(0x30, alternate+8*length)
        change(0x34, alternate+24)
    events = []
    joystick_visits = 0
    returned = False
    def hook(uc, address, size, data):
        nonlocal joystick_visits, returned
        if address == STOP:
            returned = True
            uc.emu_stop()
            return
        if address not in (CALLBACK, CALLBACK+0x10, CALLBACK+0x20): return
        sp = uc.reg_read(UC_X86_REG_ESP)
        self = uc.reg_read(UC_X86_REG_ECX)
        assert get(sp+4) & 255 == focused
        if address == CALLBACK:
            assert self == mouse
            events.append(('mouse', focused))
            if mutation == 1: change(0x28, alt_keyboard)
        elif address == CALLBACK+0x10:
            assert self in (keyboard, alt_keyboard)
            events.append(('alternate_keyboard' if self == alt_keyboard else 'keyboard', focused))
            if mutation == 2: change_vector(count)
        else:
            assert self in joysticks
            events.append(('joystick', joysticks.index(self), focused))
            if mutation == 3 and joystick_visits == 0: change_vector(0)
            joystick_visits += 1
        ret = get(sp)
        uc.reg_write(UC_X86_REG_EAX, 0xdead0000)
        uc.reg_write(UC_X86_REG_ECX, 0xdead0001)
        uc.reg_write(UC_X86_REG_EDX, 0xdead0002)
        uc.reg_write(UC_X86_REG_ESP, sp+8)
        uc.reg_write(UC_X86_REG_EIP, ret)
    uc.hook_add(UC_HOOK_CODE, hook)
    initial_sp = STACK+0x8000
    put(initial_sp, STOP)
    put(initial_sp+4, focused)
    uc.reg_write(UC_X86_REG_ESP, initial_sp)
    uc.reg_write(UC_X86_REG_ECX, SYSTEM)
    preserved = [UC_X86_REG_EBX, UC_X86_REG_EBP, UC_X86_REG_ESI, UC_X86_REG_EDI]
    for i, reg in enumerate(preserved): uc.reg_write(reg, 0xabc000+i)
    uc.emu_start(ENTRY, STOP+1, count=10000)
    assert returned and joystick_visits == count
    assert uc.reg_read(UC_X86_REG_ESP) == initial_sp+8
    for i, reg in enumerate(preserved): assert uc.reg_read(reg) == 0xabc000+i
    after = bytes(uc.mem_read(SYSTEM, 4096))
    assert after == expected
    return events, after.hex()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, default=ROOT/'rebuild/src/compiled/00/9f/CInputManager_SetHasInputFocus_009f4e20.cpp')
    parser.add_argument('--test', type=Path, default=ROOT/'rebuild/tests/integration/InputFocus_test.cpp')
    parser.add_argument('--include', type=Path, default=ROOT/'rebuild/include')
    parser.add_argument('--output', type=Path, default=ROOT/'rebuild/compile-gate/input-focus.json')
    args = parser.parse_args()
    work = Path(tempfile.mkdtemp(prefix='input-focus-'))
    env = check_one.env()
    env['INCLUDE'] = str(args.include.resolve())+';'+env['INCLUDE']
    obj, test_obj = work/'source.obj', work/'fixture.obj'
    for source, target, optimization in [(args.source, obj, '/O2'), (args.test, test_obj, '/Od')]:
        result = check_one.cl(['/nologo','/c',optimization,'/Oy','/GS','/MT','/Fo'+str(target),str(source.resolve())], env, work)
        assert result.returncode == 0, result.stdout+result.stderr
    coff = Coff(obj)
    section, offset = coff.symbols[SYMBOL]
    built, relocs = coff.sections[section]
    assert offset == 0 and not relocs
    with pefile.PE(str(EXE), fast_load=True) as pe:
        base = pe.OPTIONAL_HEADER.ImageBase
        retail = pe.get_data(ENTRY-base, 75)
        for table, slot, target in [(0x0129dbc4, 14, 0x00a673a0),
                                    (0x0129dba8, 3, 0x00a66ac0),
                                    (0x0129c0ec, 16, 0x009e4950)]:
            assert int.from_bytes(pe.get_data(table+4*slot-base, 4), 'little') == target
    score = compare(built, retail)
    assert score.near_match, score
    cases = []
    for scenario in itertools.product(range(2), range(2), range(2), range(4), range(4)):
        native = execute(retail, *scenario)
        candidate = execute(built, *scenario)
        assert candidate == native, scenario
        cases.append({'scenario': scenario, 'events': native[0]})
    exe = work/'fixture.exe'
    command = [str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    standalone = subprocess.run(command+[str(test_obj)], env=env, capture_output=True, text=True)
    assert standalone.returncode != 0, 'fixture must require candidate'
    linked = subprocess.run(command+[str(obj),str(test_obj)], env=env, capture_output=True, text=True)
    assert linked.returncode == 0, linked.stdout+linked.stderr
    tested = subprocess.run([str(exe)], env=env, capture_output=True, text=True, timeout=30)
    assert tested.returncode == 0 and 'INPUT_FOCUS_BEHAVIOR_PASS scenarios=128' in tested.stdout, tested.stdout+tested.stderr
    grade = 'MATCH' if built == retail else 'NEAR_MATCH'
    report = {'address': f'{ENTRY:08x}', 'grade': grade, 'retail_bytes': 75,
              'built_bytes': len(built), 'similarity': score.to_dict(),
              'source_sha256': hashlib.sha256(args.source.read_bytes()).hexdigest(),
              'fixture_sha256': hashlib.sha256(args.test.read_bytes()).hexdigest(),
              'behavior': tested.stdout.strip(), 'comparative_scenarios': cases}
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2)+'\n')
    print('INPUT_FOCUS_PASS grade='+grade+' retail=75 built='+str(len(built))+' compiled=128 comparative='+str(len(cases)))


if __name__ == '__main__': main()
