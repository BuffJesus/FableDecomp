"""Compare native window setup with isolated imports; never creates a host window."""
import argparse
import collections
import hashlib
import itertools
import json
import struct
import subprocess
import tempfile
from pathlib import Path

import capstone
import pefile
import check_one
from gate_game_component_abi import Coff
from near_match import compare, retail_relocations
from pe_oracle import EXE
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import *

ROOT = Path(__file__).resolve().parents[2]
ENTRY = 0x009a64b0
SYSTEM = 0x02000000
CALLBACK = 0x02100000
STOP = CALLBACK+0x100
STACK = 0x03000000
NAME = SYSTEM+0x500
SYMBOL = '?InitWindow@CSystemManager@@AAE_NJJ@Z'
PROCEDURE = '?WinProcedure@CSystemManager@@CGJPAUHWND__@@IIJ@Z'
CONVERSION = '??BCWideString@@QBEPBGXZ'
IMPORTS = {
    '__imp__LoadCursorW@8': (0x01440398, 'cursor', 2),
    '__imp__GetStockObject@4': (0x0143fc78, 'brush', 1),
    '__imp__LoadIconW@8': (0x01440394, 'icon', 2),
    '__imp__RegisterClassExW@4': (0x01440390, 'register', 1),
    '__imp__GetSystemMetrics@4': (0x0144038c, 'metric', 1),
    '__imp__CreateWindowExW@48': (0x01440388, 'create', 12),
    '__imp__ShowWindow@8': (0x01440354, 'show', 2),
    '__imp__UpdateWindow@4': (0x01440384, 'update', 1),
}
TARGETS = {name: value[0] for name, value in IMPORTS.items()}
TARGETS.update({PROCEDURE: 0x009a5b60, CONVERSION: 0x0099af00})
DIMENSIONS = [(800,600), (0,0), (-1,-1), (4000,3001), (1279,719)]
SCREENS = [(1920,1080,1,1,23), (801,601,2,3,27), (-9,-11,0,0,0)]
SHOWS = [-1,0,5]


def execute(code, dimension, screen, command, create_success, mutate, initial_valid, failed_resources):
    uc = Uc(UC_ARCH_X86, UC_MODE_32)
    for address, size in [(ENTRY & ~4095, 4096), (0x0099a000, 4096),
                          (0x0143f000, 0x2000), (SYSTEM, 4096),
                          (CALLBACK, 4096), (STACK, 0x10000)]:
        uc.mem_map(address, size)
    uc.mem_write(ENTRY, bytes(code))
    # End the helper's translated block inside its mapped page. Its hook supplies
    # the return value and ABI; this RET is never executed as recovered code.
    uc.mem_write(0x0099af00, b'\xc3')
    def put(address, value): uc.mem_write(address, struct.pack('<I', value & 0xffffffff))
    def get(address): return struct.unpack('<I', uc.mem_read(address, 4))[0]
    def signed(value): return value if value < 0x80000000 else value-0x100000000
    def half(value): return value//2 if value >= 0 else -((-value)//2)
    def resource(value): return 0 if failed_resources else value
    width, height = DIMENSIONS[dimension]
    sw, sh, bx, by, caption = SCREENS[screen]
    metrics = {0: sw, 1: sh, 5: bx, 6: by, 4: caption}
    instance, changed_window = 0x13570000, 0x24680000
    created_window = 0x12340000 if create_success else 0
    uc.mem_write(SYSTEM, b'\xa5'*0x180)
    put(SYSTEM+0x90, instance)
    put(SYSTEM+0x98, SHOWS[command])
    uc.mem_write(SYSTEM+0xe8, bytes([initial_valid]))
    expected = bytearray(uc.mem_read(SYSTEM, 4096))
    def change(offset, value):
        put(SYSTEM+offset, value)
        struct.pack_into('<I', expected, offset, value & 0xffffffff)
    hooks = {}
    for i, (slot, name, argc) in enumerate(IMPORTS.values()):
        address = CALLBACK+16*i
        put(slot, address)
        hooks[address] = (name, argc)
    events = []
    names = icons = metric_calls = 0
    returned = False
    def hook(uc, address, size, data):
        nonlocal names, icons, metric_calls, returned
        if address == STOP:
            returned = True
            uc.emu_stop()
            return
        if address != 0x0099af00 and address not in hooks: return
        sp = uc.reg_read(UC_X86_REG_ESP)
        if address == 0x0099af00:
            assert uc.reg_read(UC_X86_REG_ECX) == SYSTEM+0xa0 and names < 3
            argc, result = 0, NAME+names*0x20
            events.append(('name', names))
            names += 1
        else:
            name, argc = hooks[address]
            args = [get(sp+4+4*i) for i in range(argc)]
            if name == 'cursor':
                assert args == [0, 0x7f00]
                result = resource(0x11110000)
            elif name == 'brush':
                assert args == [4]
                result = resource(0x22220000)
            elif name == 'icon':
                assert args == [instance, 101] and icons < 2
                result = resource(0x33330000+icons)
                icons += 1
            elif name == 'register':
                assert args == [SYSTEM+0xa4]
                expected_class = struct.pack('<12I', 48, 0, 0x009a5b60, 0, 0, instance,
                    resource(0x33330001), resource(0x11110000), resource(0x22220000), 0, NAME,
                    resource(0x33330000))
                assert bytes(uc.mem_read(args[0], 48)) == expected_class
                expected[0xa4:0xd4] = expected_class
                result = 0 if failed_resources else 42
            elif name == 'metric':
                assert metric_calls < 5 and args[0] == [0,1,5,6,4][metric_calls]
                metric_calls += 1
                result = metrics[args[0]]
            elif name == 'create':
                assert args[:4] == [0, NAME+0x40, NAME+0x20, 0x00ca0000]
                assert [signed(x) for x in args[4:8]] == [half(sw-2*bx-width), half(sh-2*by-height-caption), width, height]
                assert args[8:] == [0,0,instance,SYSTEM]
                if mutate: change(0x98, -7)
                result = created_window
            elif name == 'show':
                assert args[0] == get(SYSTEM+0x94) == created_window
                assert signed(args[1]) == (-7 if mutate else SHOWS[command])
                if mutate: change(0x94, changed_window)
                result = 0
            else:
                assert name == 'update' and args == [changed_window if mutate else created_window]
                assert uc.mem_read(SYSTEM+0xe8, 1)[0] == initial_valid
                result = 0
            events.append((name, args, result))
        ret = get(sp)
        uc.reg_write(UC_X86_REG_EAX, result & 0xffffffff)
        uc.reg_write(UC_X86_REG_ECX, 0xdead0001)
        uc.reg_write(UC_X86_REG_EDX, 0xdead0002)
        uc.reg_write(UC_X86_REG_ESP, sp+4+argc*4)
        uc.reg_write(UC_X86_REG_EIP, ret)
    uc.hook_add(UC_HOOK_CODE, hook)
    initial_sp = STACK+0x8000
    put(initial_sp, STOP)
    put(initial_sp+4, width)
    put(initial_sp+8, height)
    uc.reg_write(UC_X86_REG_ESP, initial_sp)
    uc.reg_write(UC_X86_REG_ECX, SYSTEM)
    preserved = [UC_X86_REG_EBX, UC_X86_REG_EBP, UC_X86_REG_ESI, UC_X86_REG_EDI]
    for i, reg in enumerate(preserved): uc.reg_write(reg, 0xabc000+i)
    try:
        uc.emu_start(ENTRY, STOP+1, count=10000)
    except Exception as error:
        raise RuntimeError(f"window emulation stopped at {uc.reg_read(UC_X86_REG_EIP):08x}; events={events}") from error
    assert returned and (uc.reg_read(UC_X86_REG_EAX) & 255) == 1
    assert uc.reg_read(UC_X86_REG_ESP) == initial_sp+12
    for i, reg in enumerate(preserved): assert uc.reg_read(reg) == 0xabc000+i
    struct.pack_into('<I', expected, 0x94, changed_window if mutate else created_window)
    expected[0xe8] = 1
    after = bytes(uc.mem_read(SYSTEM, 4096))
    assert after == expected and names == 3 and icons == 2 and metric_calls == 5
    return events, after.hex()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, default=ROOT/'rebuild/src/compiled/00/9a/CSystemManager_InitWindow_009a64b0.cpp')
    parser.add_argument('--test', type=Path, default=ROOT/'rebuild/tests/00/9a/CSystemManager_InitWindow_009a64b0_test.cpp')
    parser.add_argument('--include', type=Path, default=ROOT/'rebuild/include')
    parser.add_argument('--output', type=Path, default=ROOT/'rebuild/compile-gate/init-window.json')
    args = parser.parse_args()
    work = Path(tempfile.mkdtemp(prefix='init-window-'))
    env = check_one.env()
    env['INCLUDE'] = str(args.include.resolve())+';'+env['INCLUDE']
    obj, test_obj = work/'source.obj', work/'fixture.obj'
    for source, target, optimization in [(args.source, obj, '/O2'), (args.test, test_obj, '/Od')]:
        result = check_one.cl(['/nologo','/c',optimization,'/Oy','/GS','/MT','/Fo'+str(target),str(source.resolve())], env, work)
        assert result.returncode == 0, result.stdout+result.stderr
    coff = Coff(obj)
    section, offset = coff.symbols[SYMBOL]
    built, relocs = coff.sections[section]
    assert offset == 0 and len(relocs) == 12
    with pefile.PE(str(EXE), fast_load=True) as pe:
        retail = pe.get_data(ENTRY-pe.OPTIONAL_HEADER.ImageBase, 346)
    native_relocs = retail_relocations('009a64b0', len(retail))
    score = compare(built, retail, relocs, native_relocs)
    assert score.near_match, score
    # Independently recover the absolute/import operands and direct call targets
    # from retail before comparing them with the candidate's named relocations.
    native_targets = []
    decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    decoder.detail = True
    for ins in decoder.disasm(retail, ENTRY):
        for operand in ins.operands:
            if operand.type == capstone.x86.X86_OP_MEM and not operand.mem.base and not operand.mem.index:
                native_targets.append(operand.mem.disp)
            elif operand.type == capstone.x86.X86_OP_IMM:
                if ins.mnemonic == 'call' or operand.imm == TARGETS[PROCEDURE]: native_targets.append(operand.imm)
    assert collections.Counter(native_targets) == collections.Counter(TARGETS[name] for name, kind in relocs.values())
    candidate = bytearray(built)
    checked = []
    for site, (symbol, kind) in relocs.items():
        target = TARGETS[symbol]
        assert kind == (20 if symbol == CONVERSION else 6)
        assert int.from_bytes(built[site:site+4], 'little') == 0
        value = target if kind == 6 else target-(ENTRY+site+4)
        struct.pack_into('<I', candidate, site, value & 0xffffffff)
        checked.append({'offset': site, 'symbol': symbol, 'target': f'{target:08x}'})
    cases = []
    for scenario in itertools.product(range(5), range(3), range(3), range(2), range(2), range(2), range(2)):
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
    assert tested.returncode == 0 and 'INIT_WINDOW_BEHAVIOR_PASS scenarios=720' in tested.stdout, tested.stdout+tested.stderr
    grade = 'RELOCATION_MATCH' if candidate == retail else 'NEAR_MATCH'
    report = {'address': f'{ENTRY:08x}', 'grade': grade, 'retail_bytes': 346,
              'built_bytes': len(built), 'similarity': score.to_dict(), 'relocations': checked,
              'source_sha256': hashlib.sha256(args.source.read_bytes()).hexdigest(),
              'fixture_sha256': hashlib.sha256(args.test.read_bytes()).hexdigest(),
              'behavior': tested.stdout.strip(), 'comparative_scenarios': cases}
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2)+'\n')
    print('INIT_WINDOW_PASS grade='+grade+' retail=346 built='+str(len(built))+' compiled=720 comparative='+str(len(cases)))


if __name__ == '__main__': main()
