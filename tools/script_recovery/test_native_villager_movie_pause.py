import unittest
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_villager_movie_scope import verify
from tools.script_recovery.test_native_affair_wife_hit_scopes import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_ECX,UC_X86_REG_EDI,UC_X86_REG_EAX,UC_X86_REG_ESI,UC_X86_REG_EBX,UC_X86_REG_EBP


def run(data,start,sex,termination,ebx):
    uc=Uc(UC_ARCH_X86,UC_MODE_32)
    for address,size in ((0xDAE000,0x1000),(0x99E000,0x1000),(0x6E7000,0x1000),(0xF35000,0x1000),(0x100000,0x10000),(0x200000,0x3000)):
        uc.mem_map(address,size)
    uc.mem_write(0xDAE1B3,data.bytes_at(0xDAE1B3,0xDAEA49-0xDAE1B3))
    def put(address,value):uc.mem_write(address,(value&0xFFFFFFFF).to_bytes(4,'little'))
    def get(address):return int.from_bytes(uc.mem_read(address,4),'little')
    stack,actor,thread,game,table=0x108000,0x200000,0x200100,0x200200,0x201000
    movie,pause,gender=0x202000,0x202010,0x202020
    put(thread+4,game);put(game,table);put(table+0x5C8,movie);put(table+0x5EC,pause);put(table+0x804,gender)
    for reg,value in ((UC_X86_REG_ESP,stack),(UC_X86_REG_EDI,actor),(UC_X86_REG_ESI,thread),(UC_X86_REG_EBP,game),(UC_X86_REG_EBX,ebx)):
        uc.reg_write(reg,value)
    events=[]
    def hook(machine,address,size,user):
        if address in (0xDAEA49,0xDAE98E):machine.emu_stop();return
        if address not in (movie,pause,gender,0x99EBF0,0x99EAE0,0xF35B30,0x6E7B80):return
        esp=machine.reg_read(UC_X86_REG_ESP);receiver=machine.reg_read(UC_X86_REG_ECX);result=0xDEADBEEF;pop=0
        if address==movie:assert receiver==game and get(esp+8)==stack+108;pop=8
        elif address==pause:assert receiver==game;events.append(('pause',get(esp+4)));pop=4
        elif address==gender:assert receiver==game and get(esp+4)==actor;result=sex;pop=4
        elif address==0x99EBF0:pop=8
        elif address==0xF35B30:result=termination
        elif address==0x6E7B80:assert receiver==stack+108;events.append(('destroy',108))
        machine.reg_write(UC_X86_REG_EAX,result);machine.reg_write(UC_X86_REG_ESP,esp+4+pop);machine.reg_write(UC_X86_REG_EIP,get(esp))
    uc.hook_add(UC_HOOK_CODE,hook);uc.emu_start(start,0xDAEB00,count=150)
    assert uc.reg_read(UC_X86_REG_ESP)==stack
    return events


class VillagerMoviePauseTests(unittest.TestCase):
    def test_early_cancellation_carries_zero_from_constructor(self):
        data=RData();verify(data)
        for sex in (0,1):
            for termination in (1,2,255):
                self.assertEqual(run(data,0xDAE1B3,sex,termination,0xBADBADFF),[('pause',1),('pause',0),('destroy',108)])

    def test_normal_and_late_cancellation_ignore_stale_ebx(self):
        data=RData()
        for start in (0xDAE2D3,0xDAE39A,0xDAEA10,0xDAEA28):
            for ebx in (0,1,0xBADBADFF):
                self.assertEqual(run(data,start,0,1,ebx),[('pause',0),('destroy',108)])
