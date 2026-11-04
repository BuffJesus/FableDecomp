import struct
import sys
from pathlib import Path

import unicorn as u
from unicorn import x86_const as x

ROOT = next(p for p in Path(__file__).resolve().parents if (p / 'tools/script_recovery/script_units.py').is_file())
sys.path.insert(0, str(ROOT))
from tools.script_recovery.lift_native_lua import RData


def run(alive, owner, stack):
    rd = RData()
    cpu = u.Uc(u.UC_ARCH_X86, u.UC_MODE_32)
    for address, size in ((0x00CBE000, 0x1000), (0x0124E000, 0x1000), (0x0122D000, 0x1000),
                          (0x0143E000, 0x1000), (0x00200000, 0x1000), (0x00300000, 0x10000),
                          (0x00500000, 0x10000)):
        cpu.mem_map(address, size)
    cpu.mem_write(0x00CBE000, rd.bytes_at(0x00CBE000, 0x1000))
    for address in (0x0124E640, 0x0122DED8):
        cpu.mem_write(address, rd.bytes_at(address, 4))
    for address in (0x00200100, 0x00200200, 0x00200300, 0x00200400):
        cpu.mem_write(address, b'\xc3')

    def put(address, value):
        cpu.mem_write(address, struct.pack('<I', value & 0xffffffff))

    def word(address):
        return struct.unpack('<I', cpu.mem_read(address, 4))[0]

    table = 0x00506000
    interface = 0x00507000
    interface_table = 0x00508000
    put(owner, table)
    put(table + 0x12c, 0x00200100)
    put(0x0143E8F8, interface)
    put(interface, interface_table)
    put(interface_table + 0x7f8, 0x00200200)
    put(interface_table + 0x1c, 0x00200300)
    put(stack, 0x00200400)
    put(stack + 4, 0x40000000)
    state = {'frames': 0, 'alphas': []}

    def hook(machine, address, size, user):
        sp = machine.reg_read(x.UC_X86_REG_ESP)
        if address == 0x00200100:
            assert machine.reg_read(x.UC_X86_REG_ECX) == owner
            machine.reg_write(x.UC_X86_REG_EAX, 1 if alive else 0)
            machine.reg_write(x.UC_X86_REG_EIP, word(sp))
            machine.reg_write(x.UC_X86_REG_ESP, sp + 4)
        elif address == 0x00200200:
            assert machine.reg_read(x.UC_X86_REG_ECX) == interface
            assert word(sp + 4) == owner and word(sp + 12) == 1
            state['alphas'].append(struct.unpack('<f', cpu.mem_read(sp + 8, 4))[0])
            machine.reg_write(x.UC_X86_REG_EIP, word(sp))
            machine.reg_write(x.UC_X86_REG_ESP, sp + 16)
        elif address == 0x00200300:
            assert machine.reg_read(x.UC_X86_REG_ECX) == interface
            state['frames'] += 1
            machine.reg_write(x.UC_X86_REG_EIP, word(sp))
            machine.reg_write(x.UC_X86_REG_ESP, sp + 4)

    cpu.hook_add(u.UC_HOOK_CODE, hook)
    cpu.reg_write(x.UC_X86_REG_ECX, owner)
    cpu.reg_write(x.UC_X86_REG_ESP, stack)
    cpu.emu_start(0x00CBE7AA, 0x00200400, count=5000)
    assert cpu.reg_read(x.UC_X86_REG_ESP) == stack + 8
    return state


def test_retail_fade_frames_and_alpha():
    rows = [(alive, owner, stack, run(alive, owner, stack)) for alive in (False, True)
            for owner in (0x00503000, 0x00504000) for stack in (0x00300800, 0x00301800)]
    assert len(rows) == 8
    assert len({row[3]['frames'] for row in rows}) == 1
    frames = rows[0][3]['frames']
    assert 29 <= frames <= 32
    assert all((len(result['alphas']) == frames if alive else not result['alphas']) for alive, _, _, result in rows)
    assert all(result['alphas'][0] == 0.0 for alive, _, _, result in rows if alive)
    print('retail fade frames', frames, 'first and last alpha', rows[4][3]['alphas'][0], rows[4][3]['alphas'][-1])


if __name__ == '__main__':
    test_retail_fade_frames_and_alpha()
