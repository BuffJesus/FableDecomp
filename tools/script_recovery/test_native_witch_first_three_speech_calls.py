import itertools
import struct
import sys
from pathlib import Path

import unicorn as u
from unicorn import x86_const as x

ROOT = next(p for p in Path(__file__).resolve().parents if (p / 'tools/script_recovery/script_units.py').is_file())
sys.path.insert(0, str(ROOT))
from tools.script_recovery.lift_native_lua import RData


def test_original_first_three_witch_speech_table_calls():
    rd = RData()
    sites = ((0xececbe, 0xecece3, 0x12f2710), (0xeced80, 0xeceda5, 0x12f26e0), (0xecee3f, 0xecee64, 0x12f26bc))
    rows = []
    for (begin, site, token), owner, stack in itertools.product(sites, (0x503000, 0x504000), (0x300800, 0x301800)):
        cpu = u.Uc(u.UC_ARCH_X86, u.UC_MODE_32)
        for address, size in ((0xece000, 0x1000), (0x200000, 0x1000), (0x300000, 0x10000), (0x500000, 0x10000)):
            cpu.mem_map(address, size)
        cpu.mem_write(0xece000, rd.bytes_at(0xece000, 0x1000))
        cpu.mem_write(0x200100, b'\xc3')
        cpu.mem_write(0x200200, b'\xc3')

        def put(address, value):
            cpu.mem_write(address, struct.pack('<I', value & 0xffffffff))

        def word(address):
            return struct.unpack('<I', cpu.mem_read(address, 4))[0]

        gsi, parent, table, hero = 0x500000, 0x505000, 0x506000, 0x507000
        put(gsi, 0x500800)
        put(0x500800 + 0x118, 0x200100)
        put(owner + 4, gsi)
        put(owner + 0x14, parent)
        put(parent + 0x58, table)
        put(table + 0x34, 0x200200)
        state = {'speech': False}

        def hook(machine, address, size, user):
            sp = machine.reg_read(x.UC_X86_REG_ESP)
            if address == 0x200100:
                assert machine.reg_read(x.UC_X86_REG_ECX) == gsi
                machine.reg_write(x.UC_X86_REG_EAX, hero)
                machine.reg_write(x.UC_X86_REG_EIP, word(sp))
                machine.reg_write(x.UC_X86_REG_ESP, sp + 4)
            elif address == 0x200200:
                assert machine.reg_read(x.UC_X86_REG_ECX) == parent + 0x58
                assert [word(sp + 4 + 4 * i) for i in range(6)] == [hero, token, 0, 0, 1, 0]
                state['speech'] = True
                machine.reg_write(x.UC_X86_REG_EIP, word(sp))
                machine.reg_write(x.UC_X86_REG_ESP, sp + 28)

        cpu.hook_add(u.UC_HOOK_CODE, hook)
        cpu.reg_write(x.UC_X86_REG_ESI, owner)
        cpu.reg_write(x.UC_X86_REG_ESP, stack)
        cpu.emu_start(begin, site + 3, count=100)
        assert state['speech']
        assert cpu.reg_read(x.UC_X86_REG_ESP) == stack
        rows.append((begin, owner, stack))
    assert len(rows) == 12


if __name__ == '__main__':
    test_original_first_three_witch_speech_table_calls()
