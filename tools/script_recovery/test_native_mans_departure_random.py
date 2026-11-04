import itertools
import struct
import sys
from pathlib import Path

ROOT = next(p for p in Path(__file__).resolve().parents if (p / 'tools/script_recovery/script_units.py').is_file())
sys.path.insert(0, str(ROOT))
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_random import RAND_THUNK, RAW_MODULUS, RAW_THUNK_BYTES


def test_native_optional_departure_random_branch():
    import unicorn as u
    from unicorn import x86_const as x

    rd = RData()
    assert RAND_THUNK == 0x00bfeb16 and RAW_MODULUS == 32768
    assert rd.bytes_at(RAND_THUNK, len(RAW_THUNK_BYTES)) == RAW_THUNK_BYTES
    cpu = u.Uc(u.UC_ARCH_X86, u.UC_MODE_32)
    for address, size in ((0xec9000, 0x3000), (0xbfe000, 0x1000), (0x300000, 0x10000), (0x500000, 0x1000)):
        cpu.mem_map(address, size)
    cpu.mem_write(0xec9000, rd.bytes_at(0xec9000, 0x3000))
    cpu.mem_write(RAND_THUNK, b'\xc3')
    state = {}

    def hook(machine, address, size, user):
        if address in (0xecab59, 0xecaa9c):
            state['exit'] = address
            machine.emu_stop()
        elif address == RAND_THUNK:
            state['draws'] += 1
            machine.reg_write(x.UC_X86_REG_EAX, state['roll'])

    cpu.hook_add(u.UC_HOOK_CODE, hook)
    rolls = (0, 1, 2, 3, 0x7ffe, 0x7fff)
    count = 0
    for leaving, roll, owner, stack in itertools.product((0, 1, 255), rolls, (0x500000, 0x500100), (0x300800, 0x301800)):
        state = {'roll': roll, 'draws': 0, 'exit': None}
        cpu.mem_write(owner + 0x1b, bytes((0x55, leaving, 0x66)))
        cpu.reg_write(x.UC_X86_REG_ESI, owner)
        cpu.reg_write(x.UC_X86_REG_ESP, stack)
        cpu.emu_start(0xecaa7a, 0xecab5a, count=30)
        assert state['draws'] == int(leaving != 0)
        assert state['exit'] == (0xecaa9c if leaving and roll % 2 == 0 else 0xecab59)
        assert cpu.reg_read(x.UC_X86_REG_ESP) == stack
        assert cpu.mem_read(owner + 0x1b, 3) == bytes((0x55, leaving, 0x66))
        count += 1
    assert count == 72


def test_generated_optional_departure_random_and_cleanup():
    from lupa.lua54 import LuaRuntime
    from tools.script_recovery.test_native_mans_departure import H

    harness = H.replace('savedStart)\n', 'savedStart,roll)\n', 1)
    harness = harness.replace(' local mode,initial,inventory,cancel=', ' local draws,talkedCount=0,0\n local mode,initial,inventory,cancel=', 1)
    harness = harness.replace(' function q:GetHero()return hero end', ' function q:RetailRandModulo(modulus) assert(modulus==32768 and roll>=0 and roll<32768);draws=draws+1;return roll end\n function q:GetHero()return hero end', 1)
    harness = harness.replace("return table.concat(trace,'|'),frame,", "return draws,table.concat(trace,'|'),frame,", 1)
    harness = harness.replace('function me:IsTalkedToByHero()return false end', 'function me:IsTalkedToByHero() if not savedStart then return false end; talkedCount=talkedCount+1;return talkedCount==1 end', 1)
    harness = harness.replace('return inventory end', 'return not savedStart and inventory end', 1)
    harness = harness.replace("if savedStart then assert(starts==0 and takes==0 and gifts==0 and morality==0 and questions==0)end", "if savedStart then assert(takes==0 and gifts==0 and morality==0 and questions==0)end", 1)
    harness = harness.replace('if deadline==0 then\n  assert(finished and saved.LeavingRegion', 'if deadline==0 and not savedStart then\n  assert(finished and saved.LeavingRegion', 1)
    assert 'local draws,talkedCount=0,0' in harness and 'RetailRandModulo(modulus)' in harness

    results = {}
    cases = 0
    for style in ('draft', 'readable'):
        source = (ROOT / 'refs/script_recovery/lifted/SickChild' / style / 'FSE/V_SickChild/Entities/MansLover.lua').read_text()
        lua = LuaRuntime()
        lua.execute(source)
        for saved, roll, health, wait in itertools.product((False, True), (0, 1, 2, 3, 32766, 32767), (False, True), (0, 2)):
            key = (saved, roll, health, wait)
            result = lua.execute(harness)(0, health, wait, 0, 31, 'below', 0, saved, roll)
            draws, trace = result[:2]
            assert draws == int(saved)
            if saved and health:
                line = 'LEAVING_REGION_10' if roll % 2 == 0 else 'IN_LOVE_WITH_MAN'
                assert line in trace and trace.count('TEXT_QST_B10_MANS_LOVER_') == 1
            if saved and not health:
                assert 'TEXT_QST_B10_MANS_LOVER_' not in trace
            if style == 'draft':
                results[key] = result
            else:
                assert result == results[key], key
            cases += 1
    assert cases == 96
