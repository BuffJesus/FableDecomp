#!/usr/bin/env python3
"""Verify target-state independence and callback ordering against full retail bodies."""
import hashlib
import itertools
import json
import struct
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX, UC_X86_REG_ECX, UC_X86_REG_EIP, UC_X86_REG_ESP
from check_cgame_play import ROOT, parity, pe_oracle, run


def main():
    image = pe_oracle.EXE.read_bytes()
    if hashlib.sha256(image).hexdigest() != '41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10':
        raise RuntimeError('Retail oracle executable changed')
    machine = Uc(UC_ARCH_X86, UC_MODE_32); machine.mem_map(0x400000, 0x1100000)
    for _, va, _, raw, size in pe_oracle.pe_sections(image): machine.mem_write(0x400000+va, image[raw:raw+size])
    machine.mem_map(0x20000000, 0x20000)
    stop, stack, receiver, table, state, query, find = (0x20000000, 0x20008000, 0x20010000, 0x20011000, 0x20012000, 0x20013000, 0x20013010)
    def put(address, value): machine.mem_write(address, struct.pack('<I', value))
    def word(address): return struct.unpack('<I', machine.mem_read(address, 4))[0]
    put(receiver, table); put(table+0x194, query); put(table+0x218, find)
    machine.mem_write(query, b'\xc3'); machine.mem_write(find, b'\xc2\x04\x00')
    events, case = [], []
    def hook(machine, address, size, data):
        if address == query:
            events.append('I'); machine.reg_write(UC_X86_REG_EAX, case[1])
            if case[4] == 1: put(receiver+0x148, 9)
        elif address == find:
            events.append('F'+str(word(machine.reg_read(UC_X86_REG_ESP)+4)))
            if case[4] == 2: put(state, case[2]^0x60)
            machine.reg_write(UC_X86_REG_EAX, state if case[3] else 0)
    machine.hook_add(UC_HOOK_CODE, hook)
    cases = list(itertools.product(range(2), range(2), range(256), range(2), range(3)))
    directory = ROOT/'work/state_independence_check'; directory.mkdir(parents=True, exist_ok=True)
    inputs = directory/'cases.txt'; inputs.write_text('\n'.join(' '.join(map(str, row)) for row in cases)+'\n')
    sources = ['rebuild/src/compiled/00/52/'+name+'.cpp' for name in (
        'CComponent_IsPositionIndependent_0052f1a0', 'CComponent_IsZoomIndependent_0052f1b0',
        'CChangingStateComponent_IsPositionIndependent_0052c870', 'CChangingStateComponent_IsZoomIndependent_0052c8b0')]
    sources.append('rebuild/tests/integration/StateIndependence_test.cpp')
    env, objects = parity.env(), []
    for i, source in enumerate(sources):
        obj = directory/f'part{i}.obj'
        run([parity.CL_EXE, '/nologo', '/c', '/W3', '/MT', '/GS', '/O2', '/Oy', '/I'+str(ROOT/'rebuild/include'), '/Fo'+str(obj), ROOT/source], env); objects.append(obj)
    exe = directory/'behavior.exe'; run([parity.VC/'bin/link.exe', '/nologo', '/subsystem:console', '/out:'+str(exe), *objects], env)
    lines = run([exe, inputs], env).splitlines()
    if len(lines) != len(cases): raise RuntimeError('Incomplete independence results')
    errors = []
    for index, (case, actual) in enumerate(zip(cases, lines)):
        events.clear(); put(receiver+0x148, 3); put(state, case[2]); put(stack, stop)
        machine.reg_write(UC_X86_REG_ESP, stack); machine.reg_write(UC_X86_REG_ECX, receiver)
        machine.emu_start((0x52C870, 0x52C8B0)[case[0]], stop, count=1000)
        if machine.reg_read(UC_X86_REG_EIP) != stop: raise RuntimeError('Native query did not return')
        expected = 'TRACE'+''.join(' '+event for event in events)+f' END {machine.reg_read(UC_X86_REG_EAX)&255} {word(receiver+0x148)}'
        if actual != expected: errors.append(dict(case=index, actual=actual, retail=expected))
    for i in range(4): (directory/f'part{i}.asm').write_text(run([parity.OBJDUMP, '-dr', objects[i]], env))
    report = dict(accepted=not errors, cases=len(cases), errors=errors, scope='complete state independence methods and base forwarding; independence and lookup callbacks doubled')
    (directory/'report.json').write_text(json.dumps(report, indent=2)+'\n')
    print(f"STATE_INDEPENDENCE {'FAIL' if errors else 'PASS'} cases={len(cases)} failures={len(errors)}")
    return int(bool(errors))


if __name__ == '__main__': raise SystemExit(main())
