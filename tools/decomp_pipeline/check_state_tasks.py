#!/usr/bin/env python3
"""Native UpdateStateChange versus readable task queue and dispatch recovery."""
import hashlib
import itertools
import json
import random
import struct
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX, UC_X86_REG_ECX, UC_X86_REG_EIP, UC_X86_REG_ESP
from check_cgame_play import ROOT, parity, pe_oracle, run


def main(change=False):
    image = pe_oracle.EXE.read_bytes()
    if hashlib.sha256(image).hexdigest() != "41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10":
        raise RuntimeError("Retail oracle executable changed")
    machine = Uc(UC_ARCH_X86, UC_MODE_32)
    machine.mem_map(0x400000, 0x1100000)
    for _, va, _, raw, size in pe_oracle.pe_sections(image):
        machine.mem_write(0x400000+va, image[raw:raw+size])
    machine.mem_map(0x20000000, 0x20000)
    stop, stack, receiver = 0x20000000, 0x20008000, 0x20010000
    head, nodes, vtable, entries, children = 0x20011000, 0x20011100, 0x20012000, 0x20013000, 0x20014000
    addresses = {name: 0x20016000+i*16 for i, name in enumerate(('P', 'T', 'C', 'S', 'I', 'K', 'F', 'H', 'Z', 'O'))}
    slots = {'P': 0xD0, 'T': 0x104, 'C': 0xBC, 'S': 0xE8, 'I': 0x228, 'K': 0x22C,
             'F': 0x21C, 'H': 0xB0, 'Z': 0x234, 'O': 0x230}
    for name, address in addresses.items():
        machine.mem_write(address, b'\xc2\x08\x00' if name == 'C' else
                          (b'\xc2\x04\x00' if name in ('F', 'H') else b'\xc3'))
        machine.mem_write(vtable+slots[name], struct.pack('<I', address))
    machine.mem_write(0xBFEA14, b'\xc3')
    machine.mem_write(0xBFEA0E, b'\xc3')
    events, freed = [], []
    internal_done = children_done = mode = 0
    present = allocations = 0
    state = 0x20017000
    def word(address): return struct.unpack('<I', machine.mem_read(address, 4))[0]
    def put(address, value): machine.mem_write(address, struct.pack('<I', value))
    def hook(machine, address, size, data):
        nonlocal allocations
        if address == stop: machine.emu_stop(); return
        if address == 0xBFEA14:
            freed.append(word(machine.reg_read(UC_X86_REG_ESP)+4)); return
        if address == 0xBFEA0E:
            machine.reg_write(UC_X86_REG_EAX, 0x20018000+allocations*16)
            allocations += 1; return
        if address not in addresses.values(): return
        name = next(name for name, value in addresses.items() if value == address)
        child = machine.reg_read(UC_X86_REG_ECX)
        index = (child-children)//0x200
        if name == 'P':
            events.append(f'P{index}'); machine.reg_write(UC_X86_REG_EAX, word(child+0xC8))
        elif name == 'T':
            events.append(f'T{index}'); machine.reg_write(UC_X86_REG_EAX, word(child+0x134))
        elif name == 'C':
            esp = machine.reg_read(UC_X86_REG_ESP)
            events.append(f'C{index}:{word(esp+4)}:{word(esp+8)}')
            if mode == 2: put(receiver+0xB4, entries)
        elif name == 'S':
            events.append(name)
            if mode == 1: put(receiver+0x134, word(receiver+0x134) & ~1)
        elif name in ('F', 'H'):
            events.append(name+str(word(machine.reg_read(UC_X86_REG_ESP)+4)))
            machine.reg_write(UC_X86_REG_EAX, (state if present else 0) if name == 'F' else present)
        elif name in ('Z', 'O'):
            events.append(name)
        else:
            events.append(name)
            machine.reg_write(UC_X86_REG_EAX, internal_done if name == 'I' else children_done)
    machine.hook_add(UC_HOOK_CODE, hook)
    cases = []
    for pending, done, count, tasks in itertools.product(range(4), range(4), range(3), range(4)):
        for requested in (0, 1, 2, 3, 4, 5):
            cases.append((pending, done, count, tasks, 0, 1, requested, 1, 1, 63, 0))
    rng = random.Random(0x52CD20)
    for _ in range(1024):
        cases.append((rng.randrange(8), rng.randrange(8), rng.randrange(3), rng.randrange(8),
                      rng.randrange(3), rng.randrange(3), rng.randrange(6), rng.randrange(2),
                      rng.randrange(2), rng.randrange(64), rng.randrange(3)))
    if change:
        cases = []
        durations = (0, 0x3F800000, 0xBF800000, 0x7FC00000, 0x7F800000, 0x80000000)
        for kind, present, requested, duration in itertools.product(range(7), range(2), range(8), durations):
            cases.append((3, 2, 2, 3, 0, 1, requested, 1, 1, 63, 0,
                          present, kind, duration, 0x40000000, 99))
        for _ in range(1024):
            cases.append((rng.randrange(8), rng.randrange(8), rng.randrange(3), rng.randrange(8),
                          rng.randrange(3), rng.randrange(3), rng.randrange(8), 1, 1, rng.randrange(64),
                          rng.choice((0, 2)), rng.randrange(2), rng.randrange(7), rng.choice(durations),
                          rng.choice(durations), rng.randrange(8)))
    directory = ROOT/('work/state_request_check' if change else 'work/state_tasks_check')
    directory.mkdir(parents=True, exist_ok=True)
    inputs = directory/'cases.txt'
    inputs.write_text('\n'.join(' '.join(map(str, case)) for case in cases)+'\n')
    env, objects = parity.env(), []
    method = 'CChangingStateComponent_ChangeState_0052cf40.cpp' if change else 'CChangingStateComponent_UpdateStateChange_0052cd20.cpp'
    for i, source in enumerate(('rebuild/src/compiled/00/52/'+method,
                                'rebuild/tests/integration/StateTasks_test.cpp')):
        obj = directory/f'part{i}.obj'
        run([parity.CL_EXE, '/nologo', '/c', '/W3', '/MT', '/GS', '/O2', '/Oy',
             *(['/DFABLE_TEST_CHANGE_STATE'] if change else []),
             '/I'+str(ROOT/'rebuild/include'), '/Fo'+str(obj), ROOT/source], env)
        objects.append(obj)
    exe = directory/'behavior.exe'
    run([parity.VC/'bin/link.exe', '/nologo', '/subsystem:console', '/out:'+str(exe), *objects], env)
    lines = run([exe, inputs], env).splitlines()
    if len(lines) != len(cases): raise RuntimeError('Incomplete state task results')
    errors = []
    for case_index, (case, actual) in enumerate(zip(cases, lines)):
        pending, done, count, tasks, current, target, requested, internal_done, children_done, bits, mode = case[:11]
        events.clear(); freed.clear(); allocations = 0
        data = bytearray(0x15C); struct.pack_into('<I', data, 0, vtable)
        struct.pack_into('<III', data, 0xB0, entries, entries+24, entries+24)
        struct.pack_into('<III', data, 0x134, pending, done, head)
        struct.pack_into('<IIIf', data, 0x144, current, target, requested, 2.5)
        struct.pack_into('<I', data, 0x158, 99); machine.mem_write(receiver, bytes(data))
        if change:
            present, kind, duration, parent_time, old_requested = case[11:]
            machine.mem_write(state, b'\0'*0x24); put(state+4, kind); put(state+0x1C, duration)
            put(receiver+0x140, parent_time); put(receiver+0x14C, old_requested)
        chain = [head]+[nodes+i*16 for i in range(count)]
        for i, node in enumerate(chain):
            machine.mem_write(node, struct.pack('<III', chain[(i+1)%len(chain)], chain[i-1], tasks))
        for i in range(3):
            child = children+i*0x200; put(entries+i*8, child); put(entries+i*8+4, 0)
            machine.mem_write(child, b'\0'*0x140); put(child, vtable)
            put(child+0xC8, receiver if bits & (1 << (i*2)) else 0)
            put(child+0x134, 8 if bits & (2 << (i*2)) else 0)
        put(stack, stop); put(stack+4, requested)
        machine.reg_write(UC_X86_REG_ESP, stack); machine.reg_write(UC_X86_REG_ECX, receiver)
        machine.emu_start(0x52CF40 if change else 0x52CD20, stop, count=10000)
        if machine.reg_read(UC_X86_REG_EIP) != stop: raise RuntimeError('Retail state tasks did not return')
        remaining, node, queue = 0, word(head), []
        while node != head:
            if remaining >= 2 or word(word(node)+4) != node or word(word(node+4)) != node:
                raise RuntimeError('Retail task links invalid')
            queue.append(word(node+8)); remaining += 1; node = word(node)
        if len(freed) != count+allocations-remaining: raise RuntimeError('Retail task release count invalid')
        snapshot = [word(receiver+offset) for offset in (0x134, 0x138, 0x144, 0x148, 0x158)]
        snapshot += [remaining, (word(receiver+0xB4)-entries)//8]
        expected = 'TRACE'+''.join(' '+event for event in events)+' END '+' '.join(map(str, snapshot))
        if change:
            expected += f' REQUEST {word(receiver+0x14C)} {word(receiver+0x150)} QUEUE'
            expected += ''.join(' '+str(task) for task in queue)
        if actual != expected: errors.append({'case': case_index, 'actual': actual, 'retail': expected})
    (directory/'update.asm').write_text(run([parity.OBJDUMP, '-dr', objects[0]], env))
    report = {'accepted': not errors, 'cases': len(cases), 'errors': errors,
              'scope': 'complete native '+('ChangeState' if change else 'UpdateStateChange')+'; allocator and virtual callees explicitly doubled',
              'mutations': ('child ChangeState clears live vector' if change else
                            'ProcessChangeState changes task bits; child ChangeState clears live vector'),
              'byte_parity': 'DIFFER; functional acceptance'}
    (directory/'report.json').write_text(json.dumps(report, indent=2)+'\n')
    label = 'STATE_REQUEST' if change else 'STATE_TASKS'
    print(f"{label} {'PASS' if not errors else 'FAIL'} cases={len(cases)} failures={len(errors)}")
    return int(bool(errors))


if __name__ == '__main__': raise SystemExit(main())
