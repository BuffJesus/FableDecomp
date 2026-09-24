#!/usr/bin/env python3
"""Check changing-component frame coordination and completion dependencies."""
import hashlib
import itertools
import json
import random
import struct
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX, UC_X86_REG_ECX, UC_X86_REG_EIP, UC_X86_REG_ESP, UC_X86_REG_FPCW
from check_cgame_play import ROOT, parity, pe_oracle, run


def main():
    image = pe_oracle.EXE.read_bytes()
    if hashlib.sha256(image).hexdigest() != '41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10':
        raise RuntimeError('Retail oracle executable changed')
    machine = Uc(UC_ARCH_X86, UC_MODE_32); machine.mem_map(0x400000, 0x1100000)
    for _, va, _, raw, size in pe_oracle.pe_sections(image): machine.mem_write(0x400000+va, image[raw:raw+size])
    machine.mem_map(0x20000000, 0x20000)
    stop, stack, receiver, state, table = 0x20000000, 0x20008000, 0x20010000, 0x20011000, 0x20012000
    children, entries = 0x20014000, 0x20015000
    callbacks = {0x20013000: 'F', 0x20013010: 'Q', 0x20013020: 'A', 0x531EC0: 'B'}
    def word(address): return struct.unpack('<I', machine.mem_read(address, 4))[0]
    def put(address, value): machine.mem_write(address, struct.pack('<I', value))
    for (address, kind), slot in zip(list(callbacks.items())[:3], (0x21C, 0xC4, 0x220)):
        put(table+slot, address); machine.mem_write(address, b'\xc2\x04\x00' if kind == 'F' else b'\xc3')
    machine.mem_write(0x531EC0, b'\xc2\x04\x00')
    events, mode, present, complete, child_mask = [], 0, 0, 0, 0
    def hook(machine, address, size, data):
        if address == stop: machine.emu_stop(); return
        if address not in callbacks: return
        kind = callbacks[address]; esp = machine.reg_read(UC_X86_REG_ESP)
        if kind == 'F':
            events.append('F'+str(word(esp+4))); machine.reg_write(UC_X86_REG_EAX, state if present else 0)
        elif kind == 'Q':
            component = machine.reg_read(UC_X86_REG_ECX)
            if component == receiver: events.append('Q'); result = complete
            else:
                index = (component-children)//0x200; events.append('Q'+str(index)); result = (child_mask >> index) & 1
                if mode == 3: put(receiver+0xB4, entries)
            machine.reg_write(UC_X86_REG_EAX, result)
        elif kind == 'B':
            events.append('B'+str(word(esp+4)))
            if mode in (1, 2): machine.mem_write(receiver+0x154, bytes((mode-1,)))
        else: events.append(kind)
    machine.hook_add(UC_HOOK_CODE, hook)
    floats = (0, 0x80000000, 0x3F800000, 0xBF800000, 0x7F800000, 0xFF800000, 0x7FC00000)
    cases = []
    for present, complete, flags, mode in itertools.product(range(2), range(2), range(128), range(3)):
        cases.append((0, present, complete, flags, mode, 0, 0, 0, 0, 0, 0, 3, 7, 0x3C800000))
    rng = random.Random(0x52C7E0)
    for _ in range(1024):
        cases.append((1, rng.randrange(2), 0, 0, 0, *[rng.choice(floats) for _ in range(6)], 3, 7, 0))
    for count, mask, mode in itertools.product(range(4), range(8), (0, 3)):
        cases.append((2, 0, 0, 0, mode, 0, 0, 0, 0, 0, 0, count, mask, 0))
    directory = ROOT/'work/state_update_check'; directory.mkdir(parents=True, exist_ok=True)
    inputs = directory/'cases.txt'; inputs.write_text('\n'.join(' '.join(map(str, row)) for row in cases)+'\n')
    objects, env = [], parity.env()
    sources = ['rebuild/src/compiled/00/52/CChangingStateComponent_'+name+'.cpp' for name in
               ('Update_0052c7e0', 'InternalChanged_0052c780', 'ChildrenChanged_0052cbf0')]
    sources.append('rebuild/tests/integration/StateUpdate_test.cpp')
    for i, source in enumerate(sources):
        obj = directory/f'part{i}.obj'
        run([parity.CL_EXE, '/nologo', '/c', '/W3', '/MT', '/GS', '/O2', '/Oy',
             '/I'+str(ROOT/'rebuild/include'), '/Fo'+str(obj), ROOT/source], env); objects.append(obj)
    exe = directory/'behavior.exe'
    run([parity.VC/'bin/link.exe', '/nologo', '/subsystem:console', '/out:'+str(exe), *objects], env)
    lines = run([exe, inputs], env).splitlines()
    if len(lines) != len(cases): raise RuntimeError('Incomplete state update traces')
    errors = []
    for index, (case, actual) in enumerate(zip(cases, lines)):
        kind, present, complete, flags, mode = case[:5]; count, child_mask, delta = case[11:]
        events.clear(); machine.mem_write(receiver, b'\0'*0x15C); machine.mem_write(state, b'\0'*0x24)
        put(receiver, table); put(receiver+0x148, 5); put(state, flags)
        machine.mem_write(receiver+0x98, struct.pack('<IIIIII', *case[5:11]))
        machine.mem_write(receiver+0x4C, struct.pack('<ff', 2, 3)); machine.mem_write(receiver+0x74, struct.pack('<ff', 4, 5))
        put(receiver+0x90, 0x04030201)
        for i in range(3): put(children+i*0x200, table); put(entries+i*8, children+i*0x200)
        machine.mem_write(receiver+0xB0, struct.pack('<III', entries, entries+count*8, entries+24))
        put(stack, stop); put(stack+4, delta)
        machine.reg_write(UC_X86_REG_ESP, stack); machine.reg_write(UC_X86_REG_ECX, receiver)
        machine.reg_write(UC_X86_REG_FPCW, 0x37F)
        machine.emu_start((0x52C7E0, 0x52C780, 0x52CBF0)[kind], stop, count=10000)
        if machine.reg_read(UC_X86_REG_EIP) != stop: raise RuntimeError('Retail state update did not return')
        result = (machine.reg_read(UC_X86_REG_EAX) & 255) if kind else 0
        snapshot = [result, machine.mem_read(receiver+0x154, 1)[0]]
        snapshot += [word(receiver+offset) for offset in (0x4C, 0x50, 0x74, 0x78, 0x90)]
        snapshot.append((word(receiver+0xB4)-entries)//8)
        expected = 'TRACE'+''.join(' '+event for event in events)+' END '+' '.join(map(str, snapshot))
        if actual != expected: errors.append({'case': index, 'actual': actual, 'retail': expected})
    report = {'accepted': not errors, 'cases': len(cases), 'errors': errors,
              'scope': 'complete native frame coordinator and internal/child completion methods; base Update and virtual dependencies doubled',
              'byte_parity': 'DIFFER; functional acceptance'}
    (directory/'report.json').write_text(json.dumps(report, indent=2)+'\n')
    print(f"STATE_UPDATE {'PASS' if not errors else 'FAIL'} cases={len(cases)} failures={len(errors)}")
    return int(bool(errors))


if __name__ == '__main__': raise SystemExit(main())
