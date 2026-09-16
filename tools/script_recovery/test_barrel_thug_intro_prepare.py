"""Original intro acquisition/wait and placement ABI; engine calls are doubles."""
import hashlib
import itertools
import struct
import unittest
from pathlib import Path

from lupa.lua54 import LuaRuntime
from tools.script_recovery.test_native_affair_wife_hit_scopes import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP, UC_X86_REG_EIP, UC_X86_REG_ECX, UC_X86_REG_EAX, UC_X86_REG_ESI, UC_X86_REG_EDI
from tools.script_recovery.lift_native_lua import RData


def native(data,pending,waiting,held,cancel,full=None):
    uc=Uc(UC_ARCH_X86,UC_MODE_32)
    for address,size in ((0xdb6000,0x2000),(0xf35000,0x1000),(0xcd2000,0x1000),
                         (0x99e000,0x1000),(0x4aa000,0x1000),(0x7e7000,0x1000),
                         (0x100000,0x10000),(0x200000,0x5000),(0x6e7000,0x1000),(0x122d000,0x1000)):
        uc.mem_map(address,size)
    uc.mem_write(0xdb6c60,data.bytes_at(0xdb6c60,0x1091))
    def put(a,v):uc.mem_write(a,(v&0xffffffff).to_bytes(4,'little'))
    def get(a):return int.from_bytes(uc.mem_read(a,4),'little')
    stack,thread,game,table,parent,actor=0x108000,0x200000,0x201000,0x202000,0x204000,0x204200
    put(thread+4,game);put(thread+0x14,parent);put(game,table);put(stack+0xf8,0x203ff0)
    calls={0xf35b30:'term',0xcd23b9:'has',0xcd2770:'reset',0x99ebf0:'key.new',
           0x99eae0:'key.close',0x4aa840:'thing.close',0x7e74d0:'control.close'}
    for index,(offset,name) in enumerate(((0x20,'acquire'),(0x1c,'frame'),(0x120,'lookup'),
                                         (0x760,'teleport'),(0x118,'hero'),(0x76c,'face'),(0x5e0,'pause'))):
        a=0x203000+index*16;put(table+offset,a);calls[a]=name
    for a in (*calls,0x203ff0):uc.mem_write(a,b'\xc3')
    health=0x203f00
    if full is not None:
        bits,busy=full
        put(0x204100,bits);put(0x122dedc,0)
        uc.mem_write(health,b'\xd9\x05'+struct.pack('<I',0x204100)+b'\xc2\x04\x00');put(table+0x420,health)
        calls.update({0x6e7b60:'movie.new',0x6e7b80:'movie.close',0x7e7490:'thing',
                      0x7e7390:'speak',0x7e7450:'task',0x7e7320:'follow',0x203070:'movie.start',0x203080:'movie.pause'})
        put(table+0x5c8,0x203070);put(table+0x5ec,0x203080)
        for a in calls:uc.mem_write(a,b'\xc3')
    events=[];counts=dict(term=0,acquire=0,state=0,task=0);finished=[]
    def hook(machine,a,n,user):
        endpoint=0xdb6f4e if full is not None else 0xdb6e20
        if a in (endpoint,0x203ff0):
            finished.append(a==endpoint);machine.emu_stop();return
        if a==0xdb6f1a:events.append(('done',True))
        if a==health:
            assert get(machine.reg_read(UC_X86_REG_ESP)+4)==stack+176
            events.append(('health',));return
        if a in (0xdb6d71,0xdb6d92):
            counts['state']+=1;value=counts['state']>waiting
            uc.mem_write(parent+0x72,bytes([value]));events.append(('state',value))
        if a not in calls:return
        name=calls[a];esp=machine.reg_read(UC_X86_REG_ESP);receiver=machine.reg_read(UC_X86_REG_ECX)
        args=lambda n:[get(esp+4+i*4) for i in range(n)]
        event=(name,);pop=0;result=0xabcd0000
        if name=='term':
            assert receiver==thread
            counts['term']+=1;value=counts['term']==cancel
            result|=int(value);event=(name,value)
        elif name in ('has','reset','control.close'):
            assert receiver==stack+16
            if name=='has':result|=int(held);event=(name,held)
        elif name=='acquire':
            assert receiver==game and args(3)==[actor,stack+16,4]
            counts['acquire']+=1;value=counts['acquire']>pending
            result|=int(value);pop=3;event=(name,value)
        elif name=='frame':assert receiver==game
        elif name=='key.new':
            assert (receiver,args(2)) in ((stack+124,[0x12d9208,0xffffffff]),(stack+120,[0x122d70e,0xffffffff]))
            if args(2)[0]==0x122d70e:
                assert data.bytes_at(0x122d70e,1)==b'\x00'
                text=''
            else:text=data.string_at(args(2)[0])
            event=(name,text);pop=2
        elif name=='lookup':
            assert receiver==game and args(2)==[stack+164,stack+124]
            # The earlier push 0 belongs to the later teleport, not this lookup.
            assert get(esp+12)==0
            result=0x204300;pop=2
        elif name=='teleport':
            assert receiver==game and args(3)==[actor,0x204300,0];pop=3
        elif name=='thing.close':assert receiver in (stack+164,stack+176)
        elif name=='key.close':assert receiver in (stack+124,stack+120)
        elif name=='hero':assert receiver==game;result=0x204400
        elif name=='face':
            assert receiver==game and args(3)==[actor,0x204400,0];pop=3
        elif name=='pause':
            assert receiver==game and args(1)==[0x40400000];pop=1;event=(name,3.0)
        elif name in ('movie.new','movie.close'):assert receiver==stack+148
        elif name=='movie.start':assert receiver==game and args(2)==[stack+120,stack+148];pop=2
        elif name=='movie.pause':
            assert receiver==game and args(1)[0] in (0,1);pop=1;event=(name,bool(args(1)[0]))
        elif name=='thing':assert receiver==stack+16 and args(1)==[stack+176];result=stack+176;pop=1
        elif name=='speak':
            assert receiver==stack+16 and args(6)==[0x204400,0x12d94d0,0,0,1,0]
            pop=6;event=(name,data.string_at(0x12d94d0),0)
        elif name=='task':
            assert receiver==stack+16;counts['task']+=1
            value=counts['task']<=busy;result|=int(value);event=(name,value)
        elif name=='follow':
            assert receiver==stack+16 and args(3)==[0x204400,0x3f800000,1]
            assert bytes(uc.mem_read(thread+0x1c,1))==b'\x01';pop=3
        events.append(event);machine.reg_write(UC_X86_REG_EAX,result)
        machine.reg_write(UC_X86_REG_ESP,esp+4+4*pop);machine.reg_write(UC_X86_REG_EIP,get(esp))
    uc.reg_write(UC_X86_REG_ESP,stack);uc.reg_write(UC_X86_REG_ESI,thread);uc.reg_write(UC_X86_REG_EDI,actor)
    uc.hook_add(UC_HOOK_CODE,hook);uc.emu_start(0xdb6cfb,0xdb7cf1,count=4000)
    assert len(finished)==1 and uc.reg_read(UC_X86_REG_ESP)==stack+(0 if finished[0] else 0xfc)
    return finished[0],events


def readable(pending,waiting,held,cancel,full=None):
    lua=LuaRuntime();source=Path(__file__).with_name('barrel_thug_intro_prepare.lua').read_text()
    if full is not None:source=source.rsplit('return prepareBarrelThugIntroduction',1)[0]+Path(__file__).with_name('barrel_thug_intro.lua').read_text()
    phase=lua.execute(source)
    q,r,state=lua.table(),lua.table(),lua.table();events=[];counts=dict(term=0,acquire=0,state=0,task=0)
    def query(name):
        counts[name]+=1
        value=counts[name]==cancel if name=='term' else (counts[name]<=full[1] if name=='task' else counts[name]>(pending if name=='acquire' else waiting))
        events.append((name,value));return value
    def state(self,key):assert key=='BarrelManLeftHeroInCharge';return query('state')
    def prepare(self,control):
        assert control==16;events.append(('has',held))
        if held:events.append(('reset',))
    def acquire(self,control,actor,priority):
        assert (control,actor,priority)==(16,17,4);return query('acquire')
    def place(self,actor):
        assert actor==17
        events.extend([('key.new','M_WHouse_ManStart'),('lookup',),('teleport',),('thing.close',),('key.close',),('hero',),('face',)])
    q.IsActiveThreadTerminating=lambda *args:query('term');q.GetStateBool=state
    q.NewScriptFrame=lambda *args:events.append(('frame',));q.Pause=lambda self,seconds:events.append(('pause',seconds))
    r.PrepareResource=prepare;r.TryAcquire=acquire;r.PlaceBarrelThugAtStart=place
    if full is not None:
        def start(self,key):
            assert key=='';events.extend([('movie.new',),('key.new',''),('movie.start',),('key.close',)]);return 148
        def done(self,key,value):assert (key,value)==('DoneIntro',True);events.append(('done',True))
        r.StartMovie=start;r.Pause=lambda self,value:events.append(('movie.pause',value))
        r.DestroyMovie=lambda self,movie:events.append(('movie.close',))
        r.NewThingFromResource=lambda *args:events.append(('thing',)) or 176
        r.ThingHealth=lambda *args:events.append(('health',)) or struct.unpack('<f',struct.pack('<I',full[0]))[0]
        r.DestroyThing=lambda *args:events.append(('thing.close',))
        r.SpeakBarrelThug=lambda self,control,line,selection:events.extend([('hero',),('speak',line,selection)])
        r.IsPerformingScriptTask=lambda *args:query('task')
        state.SetStateBool=done
        r.FollowBarrelThugHero=lambda *args:events.extend([('hero',),('follow',)])
    result=phase(q,17,r,16,state)
    if not result:events.append(('control.close',))
    return result,events


class BarrelThugIntroPrepareTests(unittest.TestCase):
    def test_placement_callee_stack_contracts(self):
        data=RData()
        for slot,address in ((0x120,0x8a7d60),(0x760,0x88e540)):
            self.assertEqual(data.bytes_at(0x1260f0c+slot,4),address.to_bytes(4,'little'))
        for address in (0x8a7dd8,0x8a7df4):self.assertEqual(data.bytes_at(address,3),b'\xc2\x08\x00')
        self.assertEqual(data.bytes_at(0x88e593,3),b'\xc2\x0c\x00')

    def test_original_acquisition_wait_placement_and_cancel_return(self):
        data=RData()
        self.assertEqual(hashlib.sha256(data.bytes_at(0xdb6c60,0x1091)).hexdigest(),'eafde7cb9a35b3f6d7c158c455496af2ea89444352b04e4f7b39dcbde39ada09')
        for case in itertools.product((0,1,3),(0,1,3),(False,True),range(0,11)):
            with self.subTest(case=case):self.assertEqual(readable(*case),native(data,*case))

    def test_complete_intro_movie_speech_follow_and_cancel(self):
        data=RData()
        self.assertEqual(hashlib.sha256(data.bytes_at(0xdb6c60,0x1091)).hexdigest(),'eafde7cb9a35b3f6d7c158c455496af2ea89444352b04e4f7b39dcbde39ada09')
        for case in itertools.product((0,2),(0,2),(False,True),range(0,11)):
            for full in itertools.product((0,0x3f800000,0xbf800000,0x7fc12345),(0,2)):
                with self.subTest(case=case,full=full):self.assertEqual(readable(*case,full),native(data,*case,full))
