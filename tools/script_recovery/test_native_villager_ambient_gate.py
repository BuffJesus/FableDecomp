"""Execute the original timer/random/proximity gate and conversation setup."""
import itertools
import unittest
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_villager_control_resource import verify
from tools.script_recovery.test_native_affair_wife_hit_scopes import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_EDI,UC_X86_REG_EAX,UC_X86_REG_ESI,UC_X86_REG_EBX


def native(data,timer,random,near,terminating,hero,id):
    uc=Uc(UC_ARCH_X86,UC_MODE_32)
    for address,size in ((0xDAE000,0x1000),(0xBFE000,0x2000),(0xCBE000,0x1000),(0xF35000,0x1000),
                         (0x143E000,0x1000),(0x100000,0x10000),(0x200000,0x7000)):uc.mem_map(address,size)
    uc.mem_write(0xDAE614,data.bytes_at(0xDAE614,0xDAE6BC-0xDAE614))
    def put(a,v):uc.mem_write(a,(v&0xFFFFFFFF).to_bytes(4,'little'))
    def get(a):return int.from_bytes(uc.mem_read(a,4),'little')
    actor,thread,game,parent,table,timerA,timerB=0x200000,0x200100,0x200200,0x201000,0x202000,0x203000,0x203100
    put(thread+4,game);put(thread+0x14,parent);put(parent+0x104,id);put(game,table)
    put(timerA,0x204000);put(timerB,0x204400);put(0x143E8F8,timerA)
    getTimer,setTimer,getHero,conversation,person=0x205000,0x205010,0x205020,0x205030,0x205040
    put(0x204000+0x168,getTimer);put(0x204400+0x164,setTimer)
    put(table+0x118,getHero);put(table+0x5b0,conversation);put(table+0x5b4,person)
    for reg,value in ((UC_X86_REG_ESP,0x108000),(UC_X86_REG_EDI,actor),(UC_X86_REG_ESI,thread)):uc.reg_write(reg,value)
    events=[];finished=[]
    def hook(machine,address,size,user):
        if address in (0xDAE98E,0xDAEA49,0xDAE6BC):
            finished.append({0xDAE98E:'skip',0xDAEA49:'cancel',0xDAE6BC:'select'}[address]);machine.emu_stop();return
        if address not in (getTimer,setTimer,getHero,conversation,person,0xBFEB16,0xCBE2FF,0xF35B30):return
        esp=machine.reg_read(UC_X86_REG_ESP);ecx=machine.reg_read(UC_X86_REG_ECX)
        args=lambda n:[get(esp+4+i*4) for i in range(n)]
        pop=0;result=0xBADBAD00
        if address==getTimer:assert ecx==timerA and args(1)==[id&0xFFFFFFFF];events.append('get');pop=4;result=timer
        elif address==0xBFEB16:events.append('rand');result=random
        elif address==getHero:assert ecx==game;events.append('hero');result=hero
        elif address==0xCBE2FF:
            assert ecx==actor and machine.reg_read(UC_X86_REG_EDX)==hero and args(1)==[0x40A00000]
            events.append('distance');pop=4;result=0xABCD0080 if near else 0xABCD0000
        elif address==0xF35B30:
            assert ecx==thread;events.append('term');result=0xABCD0080 if terminating else 0xABCD0000
            put(0x143E8F8,timerB);put(parent+0x104,id+11)
        elif address==setTimer:assert ecx==timerB and args(2)==[(id+11)&0xFFFFFFFF,3];events.append('set');pop=8
        elif address==conversation:assert ecx==game and args(3)==[actor,0,0];events.append('conversation');pop=12;result=-7
        elif address==person:assert ecx==game and args(2)==[(-7)&0xFFFFFFFF,hero];events.append('person');pop=8
        machine.reg_write(UC_X86_REG_EAX,result&0xFFFFFFFF);machine.reg_write(UC_X86_REG_ESP,esp+4+pop);machine.reg_write(UC_X86_REG_EIP,get(esp))
    uc.hook_add(UC_HOOK_CODE,hook);uc.emu_start(0xDAE614,0xDAEFFF,count=150)
    assert len(finished)==1 and uc.reg_read(UC_X86_REG_ESP)==0x108000
    if finished[0]=='select':assert uc.reg_read(UC_X86_REG_EBX)==(-7)&0xFFFFFFFF
    return finished[0],events


class NativeVillagerAmbientGateTests(unittest.TestCase):
    def test_native_short_circuits_and_live_timer_reload(self):
        data=RData();verify(data)
        for case in itertools.product((-1,0,1),(-2147483648,-100,-1,0,100,2147483647),(False,True),(False,True),(0,0x206000),(-1,0,73)):
            timer,random,near,terminating,hero,id=case
            events=['get'];outcome='skip'
            if timer==0:
                events.append('rand')
                if random%100==0:
                    events+=['hero','distance']
                    if near:
                        events.append('term');outcome='cancel' if terminating else 'select'
                        if not terminating:events+=['set','conversation','hero','person']
            with self.subTest(case=case):self.assertEqual(native(data,*case),(outcome,events))
