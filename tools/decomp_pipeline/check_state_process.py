#!/usr/bin/env python3
"""Compare full ProcessChangeState call traces with the retail oracle."""
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
    for _, va, _, raw, size in pe_oracle.pe_sections(image):
        machine.mem_write(0x400000+va, image[raw:raw+size])
    machine.mem_map(0x20000000, 0x20000)
    stop, stack, receiver, state, table = 0x20000000, 0x20008000, 0x20010000, 0x20011000, 0x20012000
    callbacks = {0x20013000: 'F', 0x20013010: 'P', 0x20013020: 'Z', 0x20013030: 'C'}
    def word(address): return struct.unpack('<I', machine.mem_read(address, 4))[0]
    def put(address, value): machine.mem_write(address, struct.pack('<I', value))
    for (address, kind), slot in zip(callbacks.items(), (0x21C, 0x80, 0x90, 0x9C)):
        put(table+slot, address); machine.mem_write(address, b'\xc2\x04\x00' if kind == 'F' else b'\xc2\x0c\x00')
    events, mode = [], 0
    def hook(machine, address, size, data):
        if address == stop: machine.emu_stop(); return
        if address not in callbacks: return
        esp = machine.reg_read(UC_X86_REG_ESP); kind = callbacks[address]
        if kind == 'F':
            events.append('F'+str(word(esp+4))); machine.reg_write(UC_X86_REG_EAX, state); return
        pointer, time, linear = word(esp+4), word(esp+8), word(esp+12)
        if kind == 'C': events.append(f'C{word(pointer)}:{time}:{linear}')
        else: events.append(f'{kind}{word(pointer)}:{word(pointer+4)}:{time}:{linear}')
        if mode == 1 and kind == 'P':
            machine.mem_write(receiver+0x5C, bytes(machine.mem_read(state+0x10, 8)))
            machine.mem_write(receiver+0x84, bytes(machine.mem_read(state+0x18, 4)))
        if mode == 2 and kind == 'P':
            put(state, 2); machine.mem_write(state+0x20, b'\x01')
    machine.hook_add(UC_HOOK_CODE, hook)
    epsilon = struct.unpack('<I', struct.pack('<f', 0.0001))[0]
    floats = (0, 0x80000000, epsilon-1, epsilon, epsilon+1, 0x3F800000, 0xBF800000,
              0x7F800000, 0xFF800000, 0x7FC00000)
    cases = []
    for x, y, flags in itertools.product(floats, floats, range(8)):
        cases.append((0, 1, flags, 0, 0, 0, x, y, 0, 0, y, x, 0xFFFFFFFF, 0x12345678, 0x40000000, 0))
    rng = random.Random(0x52C920)
    for _ in range(1024):
        vectors = [rng.choice(floats) for _ in range(8)]
        cases.append((rng.randrange(3), rng.randrange(3), rng.randrange(256), rng.choice((0, 1, 2, 255)),
                      *vectors, rng.getrandbits(32), rng.getrandbits(32), rng.choice(floats), rng.randrange(3)))
    directory = ROOT/'work/state_process_check'; directory.mkdir(parents=True, exist_ok=True)
    inputs = directory/'cases.txt'; inputs.write_text('\n'.join(' '.join(map(str, row)) for row in cases)+'\n')
    objects, env = [], parity.env()
    for i, source in enumerate(('rebuild/src/compiled/00/52/CChangingStateComponent_ProcessChangeState_0052c920.cpp',
                                'rebuild/tests/integration/StateProcess_test.cpp')):
        obj = directory/f'part{i}.obj'
        run([parity.CL_EXE, '/nologo', '/c', '/W3', '/MT', '/GS', '/O2', '/Oy',
             '/I'+str(ROOT/'rebuild/include'), '/Fo'+str(obj), ROOT/source], env); objects.append(obj)
    exe = directory/'behavior.exe'
    run([parity.VC/'bin/link.exe', '/nologo', '/subsystem:console', '/out:'+str(exe), *objects], env)
    lines = run([exe, inputs], env).splitlines()
    if len(lines) != len(cases): raise RuntimeError('Incomplete process-change traces')
    errors = []
    for index, (case, actual) in enumerate(zip(cases, lines)):
        current, target, flags, linear, px, py, tx, ty, zx, zy, ux, uy, colour, new_colour, time, mode = case
        events.clear(); machine.mem_write(receiver, b'\0'*0x15C); machine.mem_write(state, b'\0'*0x24)
        put(receiver, table); put(receiver+0x144, current); put(receiver+0x148, target); put(receiver+0x150, time)
        machine.mem_write(receiver+0x34, struct.pack('<II', px, py)); machine.mem_write(receiver+0x5C, struct.pack('<II', zx, zy))
        put(receiver+0x84, colour); put(state, flags); machine.mem_write(state+0x20, bytes((linear,)))
        machine.mem_write(state+8, struct.pack('<IIII', tx, ty, ux, uy)); put(state+0x18, new_colour)
        put(stack, stop); machine.reg_write(UC_X86_REG_ESP, stack); machine.reg_write(UC_X86_REG_ECX, receiver)
        machine.reg_write(UC_X86_REG_FPCW, 0x37F); machine.emu_start(0x52C920, stop, count=10000)
        if machine.reg_read(UC_X86_REG_EIP) != stop: raise RuntimeError('Retail process did not return')
        expected = 'TRACE'+''.join(' '+event for event in events)+' END'
        if actual != expected: errors.append({'case': index, 'actual': actual, 'retail': expected})
    (directory/'process.asm').write_text(run([parity.OBJDUMP, '-dr', objects[0]], env))
    report = {'accepted': not errors, 'cases': len(cases), 'errors': errors,
              'scope': 'complete native ProcessChangeState; lookup and relative change callbacks doubled',
              'byte_parity': 'DIFFER; functional acceptance'}
    (directory/'report.json').write_text(json.dumps(report, indent=2)+'\n')
    print(f"STATE_PROCESS {'PASS' if not errors else 'FAIL'} cases={len(cases)} failures={len(errors)}")
    return int(bool(errors))


if __name__ == '__main__': raise SystemExit(main())
