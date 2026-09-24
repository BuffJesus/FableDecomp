#!/usr/bin/env python3
"""Compare complete position/zoom updates and coordinate conversion to retail."""
import hashlib
import itertools
import json
import math
import random
import struct
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX, UC_X86_REG_ECX, UC_X86_REG_EIP, UC_X86_REG_ESP, UC_X86_REG_FPCW
from check_cgame_play import ROOT, parity, pe_oracle, run


def bits(value): return struct.unpack('<I', struct.pack('<f', value))[0]
def number(value): return struct.unpack('<f', struct.pack('<I', value))[0]


def main():
    image = pe_oracle.EXE.read_bytes()
    if hashlib.sha256(image).hexdigest() != '41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10':
        raise RuntimeError('Retail oracle executable changed')
    machine = Uc(UC_ARCH_X86, UC_MODE_32); machine.mem_map(0x400000, 0x1100000)
    for _, va, _, raw, size in pe_oracle.pe_sections(image): machine.mem_write(0x400000+va, image[raw:raw+size])
    machine.mem_map(0x20000000, 0x20000)
    stop, stack, receiver, table, output = 0x20000000, 0x20008000, 0x20010000, 0x20011000, 0x20012000
    independent, relative = 0x20013000, 0x20013010
    def word(address): return struct.unpack('<I', machine.mem_read(address, 4))[0]
    def put(address, value): machine.mem_write(address, struct.pack('<I', value))
    for slot in (0x198, 0x19C): put(table+slot, independent)
    for slot in (0x1D0, 0x1D4): put(table+slot, relative)
    machine.mem_write(independent, b'\xc3'); machine.mem_write(relative, b'\xc3')
    machine.mem_write(0x41E5F2, b'\xc3'); machine.mem_write(0x41CF47, b'\xc2\x04\x00')
    events, independent_calls, scale_calls = [], 0, 0
    case = []
    def hook(machine, address, size, data):
        nonlocal independent_calls, scale_calls
        if address == independent:
            events.append(f'I{independent_calls}')
            if case[5] == 2 and independent_calls == 0: put(receiver+0xC8, 0)
            machine.reg_write(UC_X86_REG_EAX, (case[1] >> independent_calls) & 1); independent_calls += 1
        elif address == relative:
            events.append('R'); machine.reg_write(UC_X86_REG_EAX, int(bool(case[2])))
            if case[5] == 1:
                put(receiver+0x34, bits(5)); put(receiver+0x5C, bits(7)); put(receiver+0x110, bits(2))
        elif address == 0x41E5F2:
            events.append('M'); machine.reg_write(UC_X86_REG_EAX, 0x1234)
        elif address == 0x41CF47:
            if machine.reg_read(UC_X86_REG_ECX) != 0x1234: raise RuntimeError('Manager receiver changed')
            events.append(f'S{scale_calls}'); destination = word(machine.reg_read(UC_X86_REG_ESP)+4)
            machine.mem_write(destination, struct.pack('<ff', number(case[27])+(scale_calls*0.125 if case[29] else 0),
                                                       number(case[28])+(scale_calls*0.25 if case[29] else 0)))
            machine.reg_write(UC_X86_REG_EAX, destination); scale_calls += 1
    machine.hook_add(UC_HOOK_CODE, hook)
    cases = []
    base = list(map(bits, (0.25, 0.5, 2, 4, 6, 20, 30, 0, 1, 3, 5, 2, 3, 7, 11, 0.5, 2, 640, 480, 1280, 720, 1.25, 0.75)))
    for method, query, convert, enabled, parent, mutation in itertools.product(range(3), range(4), range(2), (0, 1, 255), range(2), range(3)):
        cases.append([method, query, convert, enabled, parent, mutation, *base, 1])
    rng = random.Random(0x52FFD0)
    for i in range(1600):
        values = [rng.choice((-1, 0, 0.016, 0.25, 8)), rng.choice((0, 0.25, 1, 4, 12)), rng.choice((0.125, 0.5, 2, 8))]
        values += [rng.uniform(-1000, 1000) for _ in range(8)]
        values += [rng.uniform(-3, 3) for _ in range(6)]
        values += [640, 480, rng.choice((640, 1280, 1920)), rng.choice((480, 720, 1080)), 1.25, 0.75]
        cases.append([i%2, rng.randrange(4), rng.randrange(2), rng.randrange(2), rng.randrange(2), 0, *map(bits, values), 0])
    for method, special in itertools.product(range(3), (0, 0x80000000, 0x7FC00000, 0x7F800000, 0xFF800000)):
        for location in (6, 7, 8, 9, 10, 23, 24):
            row = [method, 0, 1, 1, 1, 0, *base, 0]; row[location] = special; cases.append(row)
    directory = ROOT/'work/ui_transform_check'; directory.mkdir(parents=True, exist_ok=True)
    inputs = directory/'cases.txt'; inputs.write_text('\n'.join(' '.join(map(str, row)) for row in cases)+'\n')
    sources = ['rebuild/src/compiled/00/52/'+name+'.cpp' for name in
               ('global_ConvertCoordinates_0052e580', 'CComponent_UpdatePosition_0052ffd0', 'CComponent_UpdateZoom_0052f5c0')]
    sources.append('rebuild/tests/integration/UiTransform_test.cpp'); env, objects = parity.env(), []
    for i, source in enumerate(sources):
        obj = directory/f'part{i}.obj'
        run([parity.CL_EXE, '/nologo', '/c', '/W3', '/MT', '/GS', '/O2', '/Oy', '/I'+str(ROOT/'rebuild/include'),
             '/Fo'+str(obj), ROOT/source], env); objects.append(obj)
    exe = directory/'behavior.exe'; run([parity.VC/'bin/link.exe', '/nologo', '/subsystem:console', '/out:'+str(exe), *objects], env)
    lines = run([exe, inputs], env).splitlines()
    if len(lines) != len(cases): raise RuntimeError('Incomplete transform results')
    errors, worst = [], 0
    for index, (case, line) in enumerate(zip(cases, lines)):
        method = case[0]; events.clear(); independent_calls = scale_calls = 0
        machine.mem_write(receiver, b'\0'*0x200); put(receiver, table); put(receiver+0xC8, receiver if case[4] else 0)
        for offset in (0x98, 0xA0): machine.mem_write(receiver+offset, struct.pack('<II', *case[7:9]))
        for offset, values in ((0x34, case[9:15]), (0x5C, case[9:15]), (0x4C, case[15:17]),
                               (0x74, case[17:19]), (0x100, case[19:21]), (0x110, case[21:23])):
            machine.mem_write(receiver+offset, struct.pack('<'+'I'*len(values), *values))
        machine.mem_write(0x13B8768, bytes((case[3],)))
        machine.mem_write(0x1375CD4, struct.pack('<II', *case[23:25])); machine.mem_write(0x13B876C, struct.pack('<II', *case[25:27]))
        args = [stop, *case[9:11]] if method == 2 else [stop, case[6]]
        machine.mem_write(stack, struct.pack('<'+'I'*len(args), *args))
        machine.reg_write(UC_X86_REG_ESP, stack); machine.reg_write(UC_X86_REG_ECX, output if method == 2 else receiver)
        machine.reg_write(UC_X86_REG_FPCW, 0x37F)
        machine.emu_start((0x52FFD0, 0x52F5C0, 0x52E580)[method], stop, count=10000)
        if machine.reg_read(UC_X86_REG_EIP) != stop: raise RuntimeError('Native transform did not return')
        offsets = (0x34, 0x38, 0x44, 0x48, 0x54, 0x58, 0xF8, 0xFC, 0x98) if method == 0 else (0x5C, 0x60, 0x6C, 0x70, 0x7C, 0x80, 0x108, 0x10C, 0xA0)
        expected = [word(output), word(output+4)] if method == 2 else [word(receiver+offset) for offset in offsets]
        trace, values = line.split(' END'); actual = list(map(int, values.split()))
        trace_ok = trace == 'TRACE'+''.join(' '+event for event in events)
        matches = []
        for a, b in zip(actual, expected):
            x, y = number(a), number(b)
            if math.isfinite(x) and math.isfinite(y):
                error = abs(x-y); worst = max(worst, error); matches.append(error <= 0.0001+abs(y)*0.000002)
            else: matches.append((math.isnan(x) and math.isnan(y)) or x == y)
        if not trace_ok or len(actual) != len(expected) or not all(matches) or (method != 2 and actual[-1] != expected[-1]):
            errors.append({'case': index, 'actual': actual, 'retail': expected, 'trace': trace, 'retail_trace': list(events)})
    for i in range(3): (directory/f'part{i}.asm').write_text(run([parity.OBJDUMP, '-dr', objects[i]], env))
    report = {'accepted': not errors, 'cases': len(cases), 'errors': errors, 'maximum_absolute_error': worst,
              'tolerance': '0.0001 + abs(retail)*0.000002 for vectors; elapsed bits and callback traces exact; nonfinite classification equal',
              'scope': 'complete native position/zoom/coordinate routines; independence/relative queries and manager services doubled',
              'byte_parity': 'DIFFER; functional acceptance'}
    (directory/'report.json').write_text(json.dumps(report, indent=2)+'\n')
    print(f"UI_TRANSFORM {'PASS' if not errors else 'FAIL'} cases={len(cases)} max_absolute_error={worst} failures={len(errors)}")
    return int(bool(errors))


if __name__ == '__main__': raise SystemExit(main())
