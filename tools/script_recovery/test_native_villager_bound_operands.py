import itertools
import json
import unittest
from pathlib import Path
from types import SimpleNamespace
from tools.script_recovery.test_native_affair_wife_hit_scopes import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_ECX,UC_X86_REG_EDI,UC_X86_REG_EAX,UC_X86_REG_ESI
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_villager_bound_operands import recover
DRAFT = Path("refs/script_recovery/lifted/NewOakValeIntro/FSE/NewOakValeIntro/Entities/NOVI_Villager.lua")


def native(data,witness,heroes):
    uc=Uc(UC_ARCH_X86,UC_MODE_32)
    for address,size in ((0xDAE000,0x1000),(0x100000,0x10000),(0x200000,0x3000)):
        uc.mem_map(address,size)
    uc.mem_write(witness['address'],data.bytes_at(witness['address'],witness['size']))
    def put(address,value):uc.mem_write(address,int(value).to_bytes(4,'little'))
    def get(address):return int.from_bytes(uc.mem_read(address,4),'little')
    stack,actor,thread,game,table=0x108000,0x200000,0x200100,0x200200,0x201000
    hero_getter,ally=0x202000,0x202010
    put(thread+4,game);put(game,table);put(table+0x118,hero_getter);put(table+0x95C,ally)
    for register,value in ((UC_X86_REG_ESP,stack),(UC_X86_REG_EDI,actor),(UC_X86_REG_ESI,thread)):
        uc.reg_write(register,value)
    events=[];pending=list(heroes)
    def hook(machine,address,size,user):
        if address not in (hero_getter,ally):return
        esp=machine.reg_read(UC_X86_REG_ESP)
        assert machine.reg_read(UC_X86_REG_ECX)==game
        if address==hero_getter:
            result=pending.pop(0);events.append(('hero',result));pop=0
        else:
            events.append(('ally',get(esp+4),get(esp+8)));result=0xDEADBEEF;pop=8
        machine.reg_write(UC_X86_REG_EAX,result)
        machine.reg_write(UC_X86_REG_ESP,esp+4+pop)
        machine.reg_write(UC_X86_REG_EIP,get(esp))
    uc.hook_add(UC_HOOK_CODE,hook)
    end=witness['address']+witness['size'];uc.emu_start(witness['address'],end,count=100)
    assert not pending and uc.reg_read(UC_X86_REG_EIP)==end and uc.reg_read(UC_X86_REG_ESP)==stack
    return events


class VillagerBoundOperandTests(unittest.TestCase):
    def test_native_alliance_directions_and_changed_operands(self):
        data=RData();source=DRAFT.read_text();output,report=recover(source,data)
        self.assertIn('quest:EntitySetThingAsAllyOfThing(uVar7, me)',output)
        self.assertIn('quest:EntityGetSex(me)',output)
        for first,second in itertools.product((0,0x200300,0x200400),repeat=2):
            self.assertEqual(native(data,report['regions'][0],(first,second)),
                [('hero',first),('ally',0x200000,first),('hero',second),('ally',second,0x200000)])
        for region in report['regions']:
            def changed(address,size):
                return bytes(size) if address==region['address'] else data.bytes_at(address,size)
            with self.assertRaises(ValueError):recover(source,SimpleNamespace(bytes_at=changed))
