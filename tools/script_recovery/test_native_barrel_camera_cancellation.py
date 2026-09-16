import json
import unittest
from pathlib import Path
from types import SimpleNamespace

from tools.script_recovery.test_native_affair_wife_hit_scopes import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX, UC_X86_REG_EBP, UC_X86_REG_ECX, UC_X86_REG_EDI, UC_X86_REG_ESI, UC_X86_REG_ESP, UC_X86_REG_EIP
from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_barrel_camera_cancellation import recover

ROOT = Path(__file__).resolve().parents[2]
WITNESS = json.loads(Path(__file__).with_name('native_barrel_camera_cancellation_witness.json').read_text())


def native(data, visible, terminating):
    uc = Uc(UC_ARCH_X86, UC_MODE_32)
    for address, size in ((0xDB5000, 0x2000), (0xF35000, 0x1000), (0x4AA000, 0x1000),
                          (0x100000, 0x10000), (0x200000, 0x2000)):
        uc.mem_map(address, size)
    start, end = WITNESS['address'], WITNESS['address'] + WITNESS['size']
    uc.mem_write(start, data.bytes_at(start, end-start))
    def put(address, value): uc.mem_write(address, value.to_bytes(4, 'little'))
    def get(address): return int.from_bytes(uc.mem_read(address, 4), 'little')
    stack, thread, game, table, teleport, actor = 0x108000, 0x200000, 0x200100, 0x200200, 0x201000, 0x200800
    put(thread+4, game); put(thread+0x20, 2); put(game, table); put(table+0x760, teleport)
    for reg, value in ((UC_X86_REG_ESP, stack), (UC_X86_REG_EBP, 0),
                       (UC_X86_REG_ESI, thread), (UC_X86_REG_EDI, actor), (UC_X86_REG_EAX, visible)):
        uc.reg_write(reg, value)
    events = []
    def hook(machine, address, size, user):
        if address == 0xDB6A1F:
            events.append('cancel'); machine.emu_stop(); return
        esp = machine.reg_read(UC_X86_REG_ESP)
        if address == 0xF35B30:
            assert machine.reg_read(UC_X86_REG_ECX) == thread
            events.append('termination'); machine.reg_write(UC_X86_REG_EAX, terminating); popped=0
        elif address == teleport:
            assert machine.reg_read(UC_X86_REG_ECX) == game and get(esp+4) == actor and get(esp+12) == 0
            events.append(('teleport', get(esp+8)-stack)); popped=12
        elif address == 0x4AA840:
            events.append(('destroy', machine.reg_read(UC_X86_REG_ECX)-stack)); popped=0
        else: return
        machine.reg_write(UC_X86_REG_EIP, get(esp)); machine.reg_write(UC_X86_REG_ESP, esp+4+popped)
    uc.hook_add(UC_HOOK_CODE, hook)
    uc.emu_start(start, end, count=100)
    assert uc.reg_read(UC_X86_REG_ESP) == stack
    return events, get(thread+0x20)


class BarrelCameraCancellationTests(unittest.TestCase):
    def test_recovered_lua_matches_both_native_destinations_and_cancellation(self):
        data=RData(); lua=LuaRuntime(unpack_returned_tuples=True)
        # Use exactly the emitted branch. The cleanup label is represented by a
        # return at this phase boundary; actual marker ownership is verified separately.
        branch=WITNESS['new'].replace('goto LAB_00db6afd', 'return false')
        run=lua.execute('return function(quest, cVar2) local alive; local r4=208; local r5=220; local native_arg_teleport_point; '+branch+' return native_arg_teleport_point end')
        for visible in (0,1,2,255):
            for terminating in (0,1,2,255):
                events=[]
                quest=lua.table()
                quest.IsActiveThreadTerminating=lambda _: events.append('termination') or terminating != 0
                result=run(quest, visible != 0)
                if result is False: events.append('cancel'); phase=2
                else: events.extend([('teleport',result),('destroy',220),('destroy',208)]); phase=3
                with self.subTest(visible=visible, terminating=terminating):
                    self.assertEqual(native(data,visible,terminating),(events,phase))

    def test_changed_source_and_native_branch_reject(self):
        data=RData(); source=(ROOT/'refs/script_recovery/lifted/NewOakValeIntro/FSE/NewOakValeIntro/Entities/NOVI_BarrelMan.lua').read_text()
        output, report=recover(source,data)
        self.assertEqual(report['status'],'recovered')
        self.assertNotIn(WITNESS['old'],output)
        with self.assertRaisesRegex(ValueError,'source correspondence'):
            recover(source.replace(WITNESS['old'],WITNESS['old']+'\n').replace('if not cVar2 then','if cVar2 then'),data)
        for site in (0xDB583A,0xDB5850,0xDB5840,0xDB5856):
            def read(address,size):
                raw=data.bytes_at(address,size)
                if raw is not None and address <= site < address+size:
                    raw=bytearray(raw); raw[site-address]^=1; raw=bytes(raw)
                return raw
            with self.assertRaisesRegex(ValueError,'native instructions'):
                recover(source,SimpleNamespace(bytes_at=read))
