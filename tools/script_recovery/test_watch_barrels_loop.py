"""Native break decisions and consequence caller operands; engine APIs and snapshot cleanup are boundaries."""
import hashlib
import itertools
import unittest
from pathlib import Path
from lupa.lua54 import LuaRuntime
from tools.script_recovery.test_native_affair_wife_hit_scopes import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_ECX,UC_X86_REG_EAX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBX
from tools.script_recovery.lift_native_lua import RData


def native(data,total,breaks,attack,cancel):
    uc=Uc(UC_ARCH_X86,UC_MODE_32)
    for a,n in ((0xdbe000,0x2000),(0xcb7000,0x1000),(0xdae000,0x1000),(0x100000,0x10000),(0x200000,0x4000),(0x99e000,0x1000),(0x4aa000,0x1000)):uc.mem_map(a,n)
    uc.mem_write(0xdbe890,data.bytes_at(0xdbe890,647))
    def put(a,v):uc.mem_write(a,(v&0xffffffff).to_bytes(4,'little'))
    def get(a):return int.from_bytes(uc.mem_read(a,4),'little')
    stack,quest,game,table=0x108000,0x200000,0x201000,0x202000
    put(quest+0x40,game);put(game,table);put(table+0x1c,0x203000)
    reward_calls={0x99ebf0:'key.new',0x99eae0:'key.close',0x4aa840:'thing.close',
                  0x203010:'lookup',0x203020:'gold',0x203030:'create',0x203040:'health'}
    for slot,address in ((0x120,0x203010),(0x924,0x203020),(0x16c,0x203030),(0x428,0x203040)):put(table+slot,address)
    for a in (0x203000,0xcb7940,0xdaea70,*reward_calls):uc.mem_write(a,b'\xc3')
    events=[];counts=dict(term=0,frame=0);ended=[];keys={};owned=[]
    def hook(machine,a,n,user):
        if a in (0xdbead4,0xdbeb07):ended.append(a);machine.emu_stop();return
        if a in (0xdbe960,0xdbe98d):events.append(('clear',))
        if a==0xdbe973:
            value=counts['frame']>=attack;uc.mem_write(quest+0x50,bytes([value]));events.append(('attack',value))
        if a==0xdbe97e:
            value=breaks[counts['frame']%len(breaks)];uc.mem_write(quest+0x74,bytes([value]));events.append(('broken',value))
        if a in (0xdbe9e2,0xdbea4b):
            events.append(('gold' if a==0xdbe9e2 else 'beetle',))
        if a in reward_calls:
            name=reward_calls[a];esp=machine.reg_read(UC_X86_REG_ESP);receiver=machine.reg_read(UC_X86_REG_ECX)
            args=lambda n:[get(esp+4+i*4) for i in range(n)]
            pop,result=0,0
            if name=='key.new':
                assert args(2)[1]==0xffffffff and receiver not in keys
                keys[receiver]=data.string_at(args(2)[0]);pop=2
            elif name=='key.close':
                assert receiver==list(keys)[-1];del keys[receiver]
            elif name=='lookup':
                assert receiver==game and args(2)==[stack+40,stack+12] and keys=={stack+12:'NOVI_Barrel'}
                owned.append(stack+40);result=0x203300;pop=2
            elif name=='gold':
                assert receiver==game and args(2)==[stack+40,stack+16] and keys=={stack+16:'OBJECT_GOLD_1'}
                assert owned==[stack+40];pop=2
            elif name=='create':
                assert receiver==game and args(5)==[stack+52,stack+20,quest+0x76,stack+24,0]
                assert keys=={stack+24:'NOVI_CreatedBeetle',stack+20:'CREATURE_OAKVALE_STAG_BEETLE'}
                owned.append(stack+52);result=0x203300;pop=5
            elif name=='health':
                assert receiver==game and args(3)==[stack+52,0x40000000,1] and not keys and owned==[stack+52];pop=3
            elif name=='thing.close':assert not keys and owned.pop()==receiver
            machine.reg_write(UC_X86_REG_EAX,result);machine.reg_write(UC_X86_REG_ESP,esp+4+4*pop);machine.reg_write(UC_X86_REG_EIP,get(esp));return
        if a not in (0xcb7940,0xdaea70,0x203000):return
        esp=machine.reg_read(UC_X86_REG_ESP);pop=0;result=0
        if a==0xcb7940:
            assert machine.reg_read(UC_X86_REG_ECX)==quest
            counts['term']+=1;value=counts['term']==cancel;events.append(('term',value));result=int(value)
        elif a==0xdaea70:
            assert machine.reg_read(UC_X86_REG_ECX)==quest and get(esp+4)==0;events.append(('baddeed',0));pop=1
        else:
            assert machine.reg_read(UC_X86_REG_ECX)==game;counts['frame']+=1;events.append(('frame',))
        machine.reg_write(UC_X86_REG_EAX,result);machine.reg_write(UC_X86_REG_ESP,esp+4+4*pop);machine.reg_write(UC_X86_REG_EIP,get(esp))
    uc.reg_write(UC_X86_REG_ESP,stack);uc.reg_write(UC_X86_REG_ESI,quest);uc.reg_write(UC_X86_REG_ECX,quest);uc.reg_write(UC_X86_REG_EBX,total)
    uc.hook_add(UC_HOOK_CODE,hook);uc.emu_start(0xdbe960,0xdbeb17,count=5000)
    assert len(ended)==1 and not keys and not owned and uc.reg_read(UC_X86_REG_ESP)==stack
    return events


def readable(total,breaks,attack,cancel):
    lua=LuaRuntime();phase=lua.execute(Path(__file__).with_name('watch_barrels_loop.lua').read_text())
    q,r=lua.table(),lua.table();events=[];counts=dict(term=0,frame=0)
    def get_state(self,key):
        if key=='AttackOver':value=counts['frame']>=attack;events.append(('attack',value))
        else:assert key=='BarrelBrokenInstantaneous';value=breaks[counts['frame']%len(breaks)];events.append(('broken',value))
        return value
    def set_state(self,key,value):assert key=='BarrelBrokenInstantaneous' and value is False;events.append(('clear',))
    def term(*args):counts['term']+=1;value=counts['term']==cancel;events.append(('term',value));return value
    def frame(*args):counts['frame']+=1;events.append(('frame',))
    q.GetStateBool=get_state;q.SetStateBool=set_state;q.IsActiveThreadTerminating=term;q.NewScriptFrame=frame
    r.RewardRemainingBarrel=lambda *args:events.append(('gold',))
    q.GetStateFloat=lambda self,key:{'BarrelBrokenPos_x':1.0,'BarrelBrokenPos_y':2.0,'BarrelBrokenPos_z':3.0}[key]
    def beetle(self,position):
        assert tuple(position[k] for k in ('x','y','z'))==(1.0,2.0,3.0)
        events.append(('beetle',))
    r.SpawnBarrelBeetle=beetle
    phase(q,r,total,lambda deed:events.append(('baddeed',deed)))
    return events


class WatchBarrelsLoopTests(unittest.TestCase):
    def test_native_rewards_flags_frame_and_termination_order(self):
        data=RData()
        self.assertEqual(hashlib.sha256(data.bytes_at(0xdbe890,647)).hexdigest(),'3c48ce5c57e30e903c5e6790f7dfd2d4a5cf9b7664f433134d6909840fd06bf2')
        for case in itertools.product(range(0,9),((False,),(True,),(False,True),(True,False,True)),(0,1,3,8,12),(0,1,2,5,10)):
            with self.subTest(case=case):self.assertEqual(readable(*case),native(data,*case))
