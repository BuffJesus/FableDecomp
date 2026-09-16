"""Native timer branches and conversation operands; API calls remain doubles."""
import hashlib
import itertools
import unittest
from pathlib import Path
from lupa.lua54 import LuaRuntime
from tools.script_recovery.test_native_affair_wife_hit_scopes import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP, UC_X86_REG_EIP, UC_X86_REG_ECX, UC_X86_REG_EAX, UC_X86_REG_ESI, UC_X86_REG_EDI
from tools.script_recovery.lift_native_lua import RData


def native(data,returned,broken,last,times,cancel):
    uc=Uc(UC_ARCH_X86,UC_MODE_32)
    for a,n in ((0xdb6000,0x2000),(0xf35000,0x1000),(0xcd2000,0x1000),(0x99e000,0x1000),(0x7e7000,0x1000),(0x143e000,0x1000),(0x100000,0x10000),(0x200000,0x6000)):
        uc.mem_map(a,n)
    uc.mem_write(0xdb6c60,data.bytes_at(0xdb6c60,0x1091))
    def put(a,v):uc.mem_write(a,(v&0xffffffff).to_bytes(4,'little'))
    def get(a):return int.from_bytes(uc.mem_read(a,4),'little')
    stack,thread,game,table,parent,actor,timer=0x108000,0x200000,0x201000,0x202000,0x204000,0x204200,0x205000
    put(thread+4,game);put(thread+0x14,parent);put(thread+0x20,last);put(game,table)
    put(parent+0x108,73);uc.mem_write(parent+0x73,bytes([returned]));uc.mem_write(parent+0x75,bytes([broken]))
    put(0x143e8f8,timer);put(timer,table);put(stack+0xf8,0x203ff0)
    calls={0xf35b30:'term',0xcd2770:'reset',0x7e74d0:'control.close',0x99ebf0:'key.new',0x99eae0:'key.close'}
    for index,(offset,name) in enumerate(((0x168,'timer'),(0x5b0,'conversation'),(0x118,'hero'),(0x5b4,'person'),(0x5b8,'line'))):
        a=0x203000+index*16;put(table+offset,a);calls[a]=name
    for a in (*calls,0x203ff0):uc.mem_write(a,b'\xc3')
    events=[];counts=dict(term=0,timer=0);finished=[];strings={}
    reads={0xdb7441-4,0xdb74a7,0xdb7511,0xdb757b,0xdb75e5,0xdb764f,0xdb76bd,0xdb7760,0xdb77cd,0xdb7837,0xdb78a2}
    def hook(machine,a,n,user):
        if a in (0xdb7924,0x203ff0):finished.append(a==0xdb7924);machine.emu_stop();return
        if a==0xdb73a3:events.append(('returned',returned))
        if a==0xdb73dd:events.append(('broken',broken))
        if a in reads:events.append(('last',last))
        if a==0xdb7907:events.append(('store',machine.reg_read(UC_X86_REG_EAX)))
        if a not in calls:return
        name=calls[a];esp=machine.reg_read(UC_X86_REG_ESP);receiver=machine.reg_read(UC_X86_REG_ECX)
        args=lambda n:[get(esp+4+i*4) for i in range(n)]
        result=0;pop=0;event=(name,)
        if name=='term':
            assert receiver==thread;counts['term']+=1;result=int(counts['term']==cancel);event=(name,bool(result))
        elif name=='timer':
            assert receiver==timer and args(1)==[73];pop=1
            result=times[min(counts['timer'],len(times)-1)];counts['timer']+=1;event=(name,result)
        elif name=='conversation':assert receiver==game and args(3)==[actor,0,0];pop=3;result=0x204300
        elif name=='hero':assert receiver==game;result=0x204400
        elif name=='person':assert receiver==game and args(2)==[0x204300,0x204400];pop=2
        elif name=='key.new':
            assert args(2)[1]==0xffffffff and stack+56<=receiver<=stack+116
            strings[receiver]=data.string_at(args(2)[0]);event=(name,strings[receiver]);pop=2
        elif name=='key.close':assert receiver in strings;event=(name,strings.pop(receiver))
        elif name=='line':
            av=args(5);assert receiver==game and av[0]==0x204300 and av[2:]==[0,actor,0x204400]
            event=(name,strings[av[1]]);pop=5
        elif name in ('reset','control.close'):assert receiver==stack+16
        events.append(event);machine.reg_write(UC_X86_REG_EAX,result&0xffffffff)
        machine.reg_write(UC_X86_REG_ESP,esp+4+4*pop);machine.reg_write(UC_X86_REG_EIP,get(esp))
    uc.reg_write(UC_X86_REG_ESP,stack);uc.reg_write(UC_X86_REG_ESI,thread);uc.reg_write(UC_X86_REG_EDI,actor)
    uc.hook_add(UC_HOOK_CODE,hook);uc.emu_start(0xdb73a0,0xdb7cf1,count=2000)
    assert len(finished)==1 and not strings
    assert uc.reg_read(UC_X86_REG_ESP)==stack+(0 if finished[0] else 0xfc)
    return finished[0],events,get(thread+0x20)


def readable(returned,broken,last,times,cancel):
    lua=LuaRuntime();phase=lua.execute(Path(__file__).with_name('barrel_thug_timed_remarks.lua').read_text())
    q,r,s=lua.table(),lua.table(),lua.table();events=[];counts=dict(term=0,timer=0);stored=[last&0xffffffff]
    def term(*args):counts['term']+=1;v=counts['term']==cancel;events.append(('term',v));return v
    def state(self,key):
        name='returned' if key=='BarrelManSpokenToHeroOnReturn' else 'broken';v=returned if name=='returned' else broken
        events.append((name,v));return v
    def timer(self,handle):
        assert handle==73;v=times[min(counts['timer'],len(times)-1)];counts['timer']+=1;events.append(('timer',v));return v
    def get_last(self,key):assert key=='LastTimeSpoken';events.append(('last',last));return last
    def store(self,key,value):assert key=='LastTimeSpoken';stored[0]=value&0xffffffff;events.append(('store',value&0xffffffff))
    def conversation(self,actor):assert actor==17;events.extend([('conversation',),('hero',),('person',)]);return 31
    def line(self,c,actor,key):assert (c,actor)==(31,17);events.extend([('key.new',key),('hero',),('line',key),('key.close',key)])
    q.GetStateBool=state;q.GetStateInt=lambda self,key:73 if key=='WatchTimer' else None;q.IsActiveThreadTerminating=term
    r.GetBarrelWatchTimer=timer;r.ResetResource=lambda *args:events.append(('reset',))
    r.NewBarrelThugRemarkConversation=conversation;r.AddBarrelThugRemark=line
    s.GetStateInt=get_last;s.SetStateInt=store
    result=phase(q,17,r,16,s)
    if not result:events.append(('control.close',))
    return result,events,stored[0]


class BarrelThugTimedRemarksTests(unittest.TestCase):
    def test_native_threshold_boundaries_repeated_reads_and_cancel(self):
        data=RData()
        self.assertEqual(hashlib.sha256(data.bytes_at(0xdb6c60,0x1091)).hexdigest(),'eafde7cb9a35b3f6d7c158c455496af2ea89444352b04e4f7b39dcbde39ada09')
        boundaries=(-1,0,1,9,10,19,20,24,25,29,30,33,34,35,37,38,44,45,46,9999)
        for returned,broken,last,timer,cancel in itertools.product((False,True),(False,True),(-1,10,25,35,45,46,9999),boundaries,range(4)):
            case=(returned,broken,last,(timer,),cancel)
            with self.subTest(case=case):self.assertEqual(readable(*case),native(data,*case))
        for times in ((46,44,37,33,29,24,19,9,-1),(46,46,46,46,46,46,44,7),(1,50,50,50,50,50,50,50)):
            for broken,cancel in itertools.product((False,True),range(4)):
                case=(False,broken,9999,times,cancel)
                with self.subTest(case=case):self.assertEqual(readable(*case),native(data,*case))
