import itertools
import unittest
from tools.script_recovery.test_native_affair_wife_hit_scopes import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_ECX,UC_X86_REG_EDI,UC_X86_REG_EAX,UC_X86_REG_ESI
from tools.script_recovery.lift_native_lua import RData


def trace(data,start,end,literal,conversation,alias):
    uc=Uc(UC_ARCH_X86,UC_MODE_32)
    for address,size in ((0xDB5000,0x2000),(0x99E000,0x1000),(0x6E7000,0x1000),(0x4AA000,0x1000),(0x100000,0x10000),(0x200000,0x3000)):
        uc.mem_map(address,size)
    uc.mem_write(start,data.bytes_at(start,end-start))
    def put(address,value):uc.mem_write(address,(value&0xFFFFFFFF).to_bytes(4,'little'))
    def get(address):return int.from_bytes(uc.mem_read(address,4),'little')
    stack,actor,thread,game,table=0x108000,0x200000,0x200100,0x200200,0x201000
    create,line=0x202000,0x202010
    put(thread+4,game);put(game,table);put(table+0x5B0,create);put(table+0x5B8,line)
    for register,value in ((UC_X86_REG_ESP,stack),(UC_X86_REG_EDI,actor),(UC_X86_REG_ESI,thread),(UC_X86_REG_ECX,game)):
        uc.reg_write(register,value)
    events=[];live={}
    def hook(machine,address,size,user):
        if address not in (create,line,0x99EBF0,0x6E7B40,0x4AA840,0x99EAE0):return
        esp=machine.reg_read(UC_X86_REG_ESP);receiver=machine.reg_read(UC_X86_REG_ECX)
        result=0xDEADBEEF
        if address==create:
            assert receiver==game and [get(esp+i) for i in (4,8,12)]==[actor,0,0]
            events.append('conversation');result=conversation;pop=12
        elif address==0x99EBF0:
            assert not live and get(esp+4)==literal and get(esp+8)==0xFFFFFFFF
            live['key']=receiver;events.append('key');pop=8
        elif address==0x6E7B40:
            assert set(live)=={'key'};live['listener']=receiver
            result=0x200500 if alias else receiver;live['target']=result;events.append('listener');pop=0
        elif address==line:
            assert receiver==game
            assert [get(esp+i) for i in (4,8,12,16,20)]==[conversation&0xFFFFFFFF,live['key'],0,actor,live['target']]
            events.append('line');pop=20
        elif address==0x4AA840:
            assert receiver==live.pop('listener');live.pop('target');events.append('listener.destroy');pop=0
        else:
            assert set(live)=={'key'} and receiver==live.pop('key');events.append('key.destroy');pop=0
        machine.reg_write(UC_X86_REG_EAX,result&0xFFFFFFFF)
        machine.reg_write(UC_X86_REG_ESP,esp+4+pop)
        machine.reg_write(UC_X86_REG_EIP,get(esp))
    uc.hook_add(UC_HOOK_CODE,hook);uc.emu_start(start,end,count=100)
    assert not live and uc.reg_read(UC_X86_REG_ESP)==stack and uc.reg_read(UC_X86_REG_EIP)==end
    return events


class BarrelConversationTraceTests(unittest.TestCase):
    def test_original_call_arguments_and_temporary_cleanup(self):
        data=RData()
        regions=((0xDB5AD1,0xDB5B35,0x12D91B0),(0xDB68CE,0xDB6933,0x12D9030))
        for region,conversation,alias in itertools.product(regions,(-1,0,73),(False,True)):
            self.assertEqual(trace(data,*region,conversation,alias),['conversation','key','listener','line','listener.destroy','key.destroy'])
