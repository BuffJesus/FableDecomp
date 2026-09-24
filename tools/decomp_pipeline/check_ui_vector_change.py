#!/usr/bin/env python3
"""Differential test of vector targets/deltas with native scale and conversion."""
import hashlib
import itertools
import json
import math
import random
import struct
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX, UC_X86_REG_ECX, UC_X86_REG_EIP, UC_X86_REG_ESP, UC_X86_REG_FPCW
from check_cgame_play import ROOT, parity, pe_oracle, run
from check_ui_transform import bits, number


def main():
    image = pe_oracle.EXE.read_bytes()
    if hashlib.sha256(image).hexdigest() != '41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10':
        raise RuntimeError('Retail oracle executable changed')
    machine = Uc(UC_ARCH_X86, UC_MODE_32); machine.mem_map(0x400000, 0x1100000)
    for _, va, _, raw, size in pe_oracle.pe_sections(image): machine.mem_write(0x400000+va, image[raw:raw+size])
    machine.mem_map(0x20000000, 0x20000)
    stop, stack, receiver, table, argument, query = (0x20000000, 0x20008000, 0x20010000, 0x20011000, 0x20012000, 0x20013000)
    def put(address, value): machine.mem_write(address, struct.pack('<I', value))
    put(table+0x7C, 0x52E9C0); put(table+0x8C, 0x52EBB0); put(table+0x1D4, query)
    machine.mem_write(query, b'\xc3'); machine.mem_write(0x41E5F2, b'\xc3')
    events, case = [], []
    def hook(machine, address, size, data):
        if address == query:
            events.append('R'); machine.reg_write(UC_X86_REG_EAX, int(bool(case[1])))
            if case[2]: put(receiver+0x34, bits(17)); put(receiver+0x38, bits(-23))
        elif address == 0x41E5F2:
            events.append('M'); machine.reg_write(UC_X86_REG_EAX, 0)
    machine.hook_add(UC_HOOK_CODE, hook)
    cases = []
    for method, relative, mutation, enabled, context, duration, alias in itertools.product(
            range(4), range(2), range(2), range(2), range(2), (0, 2, -1), (0, 0x34, 0x3C, 0x44, 0x5C, 0x64, 0x6C)):
        cases.append([method, relative, mutation, enabled, context, 0, bits(duration), 1,
                      *map(bits, (5, 7, 11, -13, 0.75, 640, 480, 1280, 720)), alias, 0, 0])
    rng = random.Random(0x52EA30)
    for i in range(800):
        cases.append([i%5, i%2, 0, (i//2)%2, (i//4)%2, 0, bits(rng.choice((0, 0.5, 8))), i%2,
                      *map(bits, [rng.uniform(-1000, 1000) for _ in range(4)]), bits(0.75),
                      *map(bits, (rng.choice((640, 1920)), 480, rng.choice((640, 1280)), 720)), 0, 0, 0])
    for method, location, special in itertools.product(range(5), (6, 8, 10, 13, 15), (0x80000000, 0x7FC00000, 0x7F800000)):
        row = [method, 1, 0, 1, 1, 0, bits(2), 0, *map(bits, (5, 7, 11, -13, 0.75, 640, 480, 1280, 720)), 0, 0, 0]
        row[location] = special; cases.append(row)
    directory = ROOT/'work/ui_vector_change_check'; directory.mkdir(parents=True, exist_ok=True)
    inputs = directory/'cases.txt'; inputs.write_text('\n'.join(' '.join(map(str, row)) for row in cases)+'\n')
    sources = ['rebuild/src/compiled/00/52/'+name+'.cpp' for name in (
        'CComponent_ChangePosition_0052e9c0', 'CComponent_ChangeZoom_0052ebb0',
        'CComponent_ChangePositionDelta_0052ea30', 'CComponent_ChangeZoomDelta_0052ec20', 'global_ConvertCoordinatesInverse_0052e530')]
    sources += ['rebuild/src/compiled/00/41/'+name+'.cpp' for name in (
        'CManager_GetUIScale_0041cf47', 'global_GetCoordinateWidth_0041cc14', 'global_GetCoordinateHeight_0041cc2b')]
    sources.append('rebuild/tests/integration/UiVectorChange_test.cpp')
    env, objects = parity.env(), []
    for i, source in enumerate(sources):
        obj = directory/f'part{i}.obj'
        run([parity.CL_EXE, '/nologo', '/c', '/W3', '/MT', '/GS', '/O2', '/Oy', '/I'+str(ROOT/'rebuild/include'), '/Fo'+str(obj), ROOT/source], env)
        objects.append(obj)
    exe = directory/'behavior.exe'; run([parity.VC/'bin/link.exe', '/nologo', '/subsystem:console', '/out:'+str(exe), *objects], env)
    lines = run([exe, inputs], env).splitlines()
    if len(lines) != len(cases): raise RuntimeError('Incomplete vector results')
    errors = []
    for index, (case, line) in enumerate(zip(cases, lines)):
        events.clear(); machine.mem_write(receiver, b'\0'*0x200); put(receiver, table)
        for offset in (0x34, 0x5C): machine.mem_write(receiver+offset, struct.pack('<IIIIII', *case[8:10], *map(bits, (33,44,-55,-66))))
        for offset in (0x98, 0xA0): put(receiver+offset, case[12])
        machine.mem_write(argument, struct.pack('<II', *case[10:12]))
        machine.mem_write(0x13B8768, bytes((case[3],))); put(0x13B86A0, receiver if case[4] else 0)
        machine.mem_write(0x1375CD4, struct.pack('<II', *case[13:15])); machine.mem_write(0x13B876C, struct.pack('<II', *case[15:17]))
        args = [stop, *case[10:12]] if case[0] == 4 else [stop, receiver+case[17] if case[17] else argument, case[6], case[7]]
        machine.mem_write(stack, struct.pack('<'+'I'*len(args), *args))
        machine.reg_write(UC_X86_REG_ESP, stack); machine.reg_write(UC_X86_REG_ECX, argument if case[0] == 4 else receiver)
        machine.reg_write(UC_X86_REG_FPCW, 0x37F)
        machine.emu_start((0x52E9C0, 0x52EBB0, 0x52EA30, 0x52EC20, 0x52E530)[case[0]], stop, count=10000)
        if machine.reg_read(UC_X86_REG_EIP) != stop: raise RuntimeError('Native vector change did not return')
        expected = list(struct.unpack('<II', machine.mem_read(argument, 8))) if case[0] == 4 else list(struct.unpack('<16I', machine.mem_read(receiver+0x34, 64)))+list(struct.unpack('<4I', machine.mem_read(receiver+0x98, 16)))
        trace, values = line.split(' END'); actual = list(map(int, values.split()))
        # Target/delta operations preserve float bits, apart from NaN payloads.
        equal = len(actual) == len(expected) and all(a == b or (math.isnan(number(a)) and math.isnan(number(b))) for a,b in zip(actual, expected))
        if not equal or trace != 'TRACE'+''.join(' '+event for event in events):
            errors.append(dict(case=index, actual=actual, retail=expected, trace=trace, retail_trace=list(events)))
    for i in range(5): (directory/f'part{i}.asm').write_text(run([parity.OBJDUMP, '-dr', objects[i]], env))
    report = dict(accepted=not errors, cases=len(cases), errors=errors, comparison='exact float bits except NaN payloads; exact callback trace',
                  scope='native target/delta setters, inverse conversion, scale and dimensions; relative query and manager singleton doubled', byte_parity='DIFFER; functional acceptance')
    (directory/'report.json').write_text(json.dumps(report, indent=2)+'\n')
    print(f"UI_VECTOR_CHANGE {'FAIL' if errors else 'PASS'} cases={len(cases)} failures={len(errors)}")
    return int(bool(errors))


if __name__ == '__main__': raise SystemExit(main())
