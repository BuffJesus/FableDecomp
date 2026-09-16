import itertools
import json
import unittest
from pathlib import Path
from types import SimpleNamespace
from tools.script_recovery.test_native_affair_wife_hit_scopes import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_ECX,UC_X86_REG_EDI,UC_X86_REG_EAX,UC_X86_REG_ESI,UC_X86_REG_EDX
from tools.script_recovery.lift_native_lua import RData


def native(data,visible,near,heroes):
    uc=Uc(UC_ARCH_X86,UC_MODE_32)
    for address,size in ((0xDB5000,0x2000),(0xCBE000,0x1000),(0x100000,0x10000),(0x200000,0x3000)):
        uc.mem_map(address,size)
    uc.mem_write(0xDB5A83,data.bytes_at(0xDB5A83,0xDB5AC2-0xDB5A83))
    def put(address,value):uc.mem_write(address,int(value).to_bytes(4,'little'))
    def get(address):return int.from_bytes(uc.mem_read(address,4),'little')
    stack,actor,thread,game,table=0x108000,0x200000,0x200100,0x200200,0x201000
    hero_getter,seen=0x202000,0x202010
    put(thread+4,game);put(game,table);put(table+0x118,hero_getter);put(table+0x9E0,seen)
    for register,value in ((UC_X86_REG_ESP,stack),(UC_X86_REG_EDI,actor),(UC_X86_REG_ESI,thread)):
        uc.reg_write(register,value)
    events=[];pending=list(heroes);destination=[]
    def hook(machine,address,size,user):
        if address in (0xDB5C28,0xDB5AC2):destination.append(address);machine.emu_stop();return
        if address not in (hero_getter,seen,0xCBE2FF):return
        esp=machine.reg_read(UC_X86_REG_ESP)
        if address==hero_getter:
            assert machine.reg_read(UC_X86_REG_ECX)==game
            result=pending.pop(0);events.append(('hero',result));pop=0
        elif address==seen:
            assert machine.reg_read(UC_X86_REG_ECX)==game
            events.append(('seen',get(esp+4),get(esp+8)));result=visible;pop=8
        else:
            events.append(('distance',machine.reg_read(UC_X86_REG_ECX),machine.reg_read(UC_X86_REG_EDX),get(esp+4)))
            result=near;pop=4
        machine.reg_write(UC_X86_REG_EAX,int(result))
        machine.reg_write(UC_X86_REG_ESP,esp+4+pop)
        machine.reg_write(UC_X86_REG_EIP,get(esp))
    uc.hook_add(UC_HOOK_CODE,hook);uc.emu_start(0xDB5A83,0xDB6000,count=100)
    assert uc.reg_read(UC_X86_REG_ESP)==stack
    return events,destination


class BarrelReturnEncounterTests(unittest.TestCase):
    def test_original_branches_and_borrowed_hero_order(self):
        data=RData()
        for visible,near,first,second in itertools.product((False,True), (False,True), (0,0x200300), (0,0x200400)):
            expected=[('hero',first),('seen',first,0x200000)]
            if not visible:expected += [('hero',second),('distance',0x200000,second,0x41200000)]
            self.assertEqual(native(data,visible,near,(first,second)),(expected,[0xDB5C28 if visible or near else 0xDB5AC2]))
