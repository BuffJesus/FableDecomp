#!/usr/bin/env python3
"""Connect recovered state methods to colour updates; compare native frame histories."""
import hashlib
import json
import struct
import sys
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX, UC_X86_REG_ECX, UC_X86_REG_EIP, UC_X86_REG_ESP, UC_X86_REG_FPCW
from check_cgame_play import ROOT, parity, pe_oracle, run


def main():
    motion = '--motion' in sys.argv[1:]
    image = pe_oracle.EXE.read_bytes()
    if hashlib.sha256(image).hexdigest() != '41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10':
        raise RuntimeError('Retail oracle executable changed')
    machine = Uc(UC_ARCH_X86, UC_MODE_32); machine.mem_map(0x400000, 0x1100000)
    for _, va, _, raw, size in pe_oracle.pe_sections(image): machine.mem_write(0x400000+va, image[raw:raw+size])
    machine.mem_map(0x20000000, 0x20000)
    stop, stack, receiver, states, table, head, entries = (0x20000000, 0x20008000, 0x20010000,
        0x20011000, 0x20012000, 0x20014000, 0x20015000)
    find, independent = 0x20013000, 0x20013010
    def word(address): return struct.unpack('<I', machine.mem_read(address, 4))[0]
    def put(address, value): machine.mem_write(address, struct.pack('<I', value))
    slots = {0x218: find, 0x21C: find, 0xB0: 0x41C5C0, 0xC4: 0x52C8F0, 0xE8: 0x52C920,
             0x220: 0x52CD20, 0x228: 0x52C780, 0x22C: 0x52CBF0,
             0x230: 0x41C640, 0x234: 0x41C650, 0x9C: 0x5305A0,
             0x98: 0x52EC60, 0x194: independent, 0x198: 0x52C870, 0x19C: 0x52C8B0,
             0x1D0: independent, 0x1D4: independent,
             0x88: 0x52FFD0, 0x94: 0x52F5C0, 0xA0: 0x52F900,
             0x7C: 0x52E9C0, 0x80: 0x52EA30, 0x8C: 0x52EBB0, 0x90: 0x52EC20}
    for slot, address in slots.items(): put(table+slot, address)
    machine.mem_write(find, b'\xc2\x04\x00'); machine.mem_write(independent, b'\x33\xc0\xc3')
    machine.mem_write(0xBFEA14, b'\xc3'); machine.mem_write(0xBFEA0E, b'\xc3')
    # Execute native base Update too. Its ownership/position-child collections
    # are empty; --motion exercises position/zoom alongside colour.
    allocations = 0
    def hook(machine, address, size, data):
        nonlocal allocations
        if address == find:
            requested = word(machine.reg_read(UC_X86_REG_ESP)+4)
            machine.reg_write(UC_X86_REG_EAX, states+requested*0x24 if requested < 2 else 0)
        elif address == 0xBFEA0E:
            machine.reg_write(UC_X86_REG_EAX, 0x20016000+allocations*16); allocations += 1
    machine.hook_add(UC_HOOK_CODE, hook)
    def invoke(address, argument=None):
        put(stack, stop)
        if argument is not None: put(stack+4, argument)
        machine.reg_write(UC_X86_REG_ESP, stack); machine.reg_write(UC_X86_REG_ECX, receiver)
        machine.reg_write(UC_X86_REG_FPCW, 0x37F); machine.emu_start(address, stop, count=30000)
        if machine.reg_read(UC_X86_REG_EIP) != stop: raise RuntimeError('Native lifecycle call did not return')
        return machine.reg_read(UC_X86_REG_EAX) & 255
    directory = ROOT/('work/state_motion_lifecycle_check' if motion else 'work/state_lifecycle_check'); directory.mkdir(parents=True, exist_ok=True)
    sources = ['rebuild/src/compiled/00/52/CChangingStateComponent_'+name+'.cpp' for name in
               ('ChangeState_0052cf40', 'UpdateStateChange_0052cd20', 'ProcessChangeState_0052c920',
                'Update_0052c7e0', 'InternalChanged_0052c780', 'ChildrenChanged_0052cbf0',
                'HasCompletedStateChange_0052c8f0')]
    sources += ['rebuild/src/compiled/00/41/CChangingStateComponent_ChangedStateLastUpdate_0041c5e0.cpp',
                'rebuild/tests/integration/StateLifecycle_test.cpp']
    sources += ['rebuild/src/compiled/00/52/'+name+'.cpp' for name in (
        'CComponent_ChangePosition_0052e9c0', 'CComponent_ChangeZoom_0052ebb0',
        'CComponent_ChangePositionDelta_0052ea30', 'CComponent_ChangeZoomDelta_0052ec20',
        'CComponent_UpdatePosition_0052ffd0', 'CComponent_UpdateZoom_0052f5c0',
        'global_ConvertCoordinates_0052e580', 'global_ConvertCoordinatesInverse_0052e530')]
    sources += ['rebuild/src/compiled/00/52/'+name+'.cpp' for name in (
        'CComponent_IsPositionIndependent_0052f1a0', 'CComponent_IsZoomIndependent_0052f1b0',
        'CChangingStateComponent_IsPositionIndependent_0052c870', 'CChangingStateComponent_IsZoomIndependent_0052c8b0')]
    sources += ['rebuild/src/compiled/00/41/'+name+'.cpp' for name in (
        'CManager_GetUIScale_0041cf47', 'global_GetCoordinateWidth_0041cc14', 'global_GetCoordinateHeight_0041cc2b')]
    sources += ['rebuild/src/compiled/00/53/'+name+'.cpp' for name in (
        'CComponent_Update_00531ec0','global_FindCountedChild_00534eb0','global_MoveCountedChildren_00535000',
        'global_AssignDeletionParents_00535800')]
    sources += ['rebuild/src/compiled/00/42/'+name+'.cpp' for name in ('global_CopyDeletion_0042cd84','global_DestroyDeletionParents_0042abca')]
    objects, env = [], parity.env()
    for i, source in enumerate(sources):
        obj = directory/f'part{i}.obj'
        run([parity.CL_EXE, '/nologo', '/c', '/W3', '/MT', '/GS', '/O2', '/Oy',
             '/I'+str(ROOT/'rebuild/include'), '/Fo'+str(obj), ROOT/source], env); objects.append(obj)
    exe = directory/'behavior.exe'
    run([parity.VC/'bin/link.exe', '/nologo', '/subsystem:console', '/out:'+str(exe), *objects], env)
    lines = run([exe, '--motion'] if motion else [exe], env).splitlines()
    if len(lines) != 480: raise RuntimeError('Incomplete lifecycle histories')
    errors, sample, worst = [], 0, 0
    for kind in range(5):
        for duration_index, duration in enumerate((0.0, 0.5, 2.0)):
            allocations = 0; machine.mem_write(receiver, b'\0'*0x15C)
            put(receiver, table); put(receiver+0x13C, head)
            put(head, head); put(head+4, head)
            for offset in (0xB0, 0xB4, 0xB8, 0xBC, 0xC0, 0xC4): put(receiver+offset, entries)
            position_head = 0x20015100
            put(receiver+0x11C, position_head); put(position_head+8, position_head)
            # Non-null parent and dependent zoom avoid unrelated manager root scaling.
            put(receiver+0xC8, 0x20015200)
            for offset in (0x84, 0x88, 0x8C, 0x90, 0x94): put(receiver+offset, 0xFFFFFFFF)
            if motion:
                for offset in (0x5C, 0x60, 0x64, 0x68, 0x6C, 0x70, 0x74, 0x78, 0x110, 0x114): put(receiver+offset, 0x3F800000)
                machine.mem_write(receiver+0x4C, struct.pack('<ff', 5, -7))
            for i in range(2):
                state = states+i*0x24; machine.mem_write(state, b'\0'*0x24)
                put(state, 7 if motion else 2); put(state+4, kind); put(state+0x18, 0xFFFFFFFF if i == 0 else 0x00FFFFFF)
                if motion: machine.mem_write(state+8, struct.pack('<ffff', 16 if i else 0, -8 if i else 0, 2 if i else 1, 0.5 if i else 1))
                machine.mem_write(state+0x1C, struct.pack('<f', duration))
            for frame in range(32):
                if frame in (0, 2, 8): invoke(0x52CF40, 1)
                if frame in (3, 20): invoke(0x52CF40, 0)
                delta = (0, 0.125, 0.25, 2, 0.5, 0, 0.25, 0.125)[frame % 8] if motion else 0.25
                invoke(0x52C7E0, struct.unpack('<I', struct.pack('<f', delta))[0]); edge = invoke(0x41C5E0)
                expected = [kind, duration_index, frame, edge, machine.mem_read(receiver+0x154, 1)[0]]
                expected += [word(receiver+offset) for offset in
                             (0x134, 0x138, 0x144, 0x148, 0x14C, 0x158, 0xA8, 0xAC, 0x84, 0x88, 0x8C, 0x94)]
                node = word(head)
                if motion:
                    expected += [word(receiver+offset) for offset in (0x34,0x38,0x3C,0x40,0x44,0x48,0x54,0x58,0xF8,0xFC,
                        0x5C,0x60,0x64,0x68,0x6C,0x70,0x7C,0x80,0x108,0x10C,0x98,0x9C,0xA0,0xA4)]
                while node != head:
                    if len(expected) >= (43 if motion else 19): raise RuntimeError('Unexpected lifecycle queue cycle')
                    expected.append(word(node+8)); node = word(node)
                actual = list(map(int, lines[sample].split())); sample += 1
                channel_error = max(abs((a >> shift & 255)-(b >> shift & 255))
                                    for a, b in zip(actual[13:17], expected[13:17]) for shift in (0, 8, 16, 24))
                worst = max(worst, channel_error)
                if actual[:13] != expected[:13] or actual[17:] != expected[17:] or channel_error > 1:
                    errors.append({'sample': sample-1, 'actual': actual, 'retail': expected})
    report = {'accepted': not errors, 'scenarios': 15, 'frames': sample, 'max_channel_error': worst, 'errors': errors,
              'scope': 'connected state methods and recovered full base Update, transforms and colour; empty collections; state lookup, allocator and independence doubled',
              'motion': motion, 'excludes': 'child hierarchy, actions, ownership and presenter integration'}
    (directory/'report.json').write_text(json.dumps(report, indent=2)+'\n')
    print(f"STATE_LIFECYCLE {'PASS' if not errors else 'FAIL'} motion={motion} scenarios=15 frames={sample} max_channel_error={worst} failures={len(errors)}")
    return int(bool(errors))


if __name__ == '__main__': raise SystemExit(main())
