import itertools
import unittest
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_villager_talk_key import verify
from tools.script_recovery.test_native_affair_wife_hit_scopes import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_EDI,UC_X86_REG_EAX,UC_X86_REG_ESI,UC_X86_REG_EBP


def native(data,start,literal,prefixSlot,resultSlot,heroValue,conversation,alias):
    uc=Uc(UC_ARCH_X86,UC_MODE_32)
    for address,size in ((0xDAE000,0x1000),(0x99E000,0x2000),(0x100000,0x10000),(0x200000,0x3000)):
        uc.mem_map(address,size)
    uc.mem_write(start,data.bytes_at(start,0xDAE5A2-start))
    def put(address,value):uc.mem_write(address,(value&0xFFFFFFFF).to_bytes(4,'little'))
    def get(address):return int.from_bytes(uc.mem_read(address,4),'little')
    stack,actor,thread,game,table=0x108000,0x200000,0x200100,0x200200,0x201000
    hero,line=0x202000,0x202010;put(thread+4,game);put(game,table);put(table+0x118,hero);put(table+0x5B8,line)
    for reg,value in ((UC_X86_REG_ESP,stack),(UC_X86_REG_EDI,actor),(UC_X86_REG_ESI,thread),(UC_X86_REG_EBP,conversation&0xFFFFFFFF)):
        uc.reg_write(reg,value)
    events=[];live=set();prefixResult=0x200500 if alias else stack+prefixSlot;concatResult=0x200600 if alias else stack+resultSlot
    def hook(machine,address,size,user):
        if address not in (hero,line,0x99EBF0,0x99F570,0x99EAE0):return
        esp=machine.reg_read(UC_X86_REG_ESP);receiver=machine.reg_read(UC_X86_REG_ECX);pop=0;result=0xDEADBEEF
        if address==hero:assert receiver==game;events.append('hero');result=heroValue
        elif address==0x99EBF0:
            assert receiver==stack+prefixSlot and get(esp+4)==literal and get(esp+8)==0xFFFFFFFF
            live.add(receiver);events.append('prefix');pop=8;result=prefixResult
        elif address==0x99F570:
            assert receiver==stack+resultSlot and machine.reg_read(UC_X86_REG_EDX)==prefixResult and get(esp+4)==stack+24
            assert stack+prefixSlot in live;live.add(receiver);events.append('concat');pop=4;result=concatResult
        elif address==line:
            assert receiver==game and [get(esp+i) for i in (4,8,12,16,20)]==[conversation&0xFFFFFFFF,concatResult,0,actor,heroValue]
            assert len(live)==2;events.append('line');pop=20
        else:
            assert receiver in live;live.remove(receiver)
            events.append('result.destroy' if receiver==stack+resultSlot else 'prefix.destroy')
        machine.reg_write(UC_X86_REG_EAX,result);machine.reg_write(UC_X86_REG_ESP,esp+4+pop);machine.reg_write(UC_X86_REG_EIP,get(esp))
    uc.hook_add(UC_HOOK_CODE,hook);uc.emu_start(start,0xDAE5A2,count=100)
    assert not live and uc.reg_read(UC_X86_REG_ESP)==stack
    return events


class VillagerTalkTemporaryTests(unittest.TestCase):
    def test_native_prefix_concat_line_and_reverse_destruction(self):
        data=RData();verify(data)
        branches=((0xDAE500,0x12D86B4,56,52),(0xDAE556,0x12D8694,64,60))
        for branch,hero,conversation,alias in itertools.product(branches,(0,0x200400),(-1,0,73),(False,True)):
            self.assertEqual(native(data,*branch,hero,conversation,alias),['hero','prefix','concat','line','result.destroy','prefix.destroy'])
