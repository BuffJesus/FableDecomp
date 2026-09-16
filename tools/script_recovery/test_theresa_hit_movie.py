"""Original movie, x87 health, speech and wait versus structured Lua."""
import itertools
import re
import struct
import unittest
from pathlib import Path
from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_theresa_health import verify
from tools.script_recovery.test_native_affair_wife_hit_scopes import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP, UC_X86_REG_EIP, UC_X86_REG_ECX, UC_X86_REG_EAX, UC_X86_REG_EBP, UC_X86_REG_EBX


def native(data,bits,busy,cancel,prelude=False):
    uc=Uc(UC_ARCH_X86,UC_MODE_32)
    for address,size in ((0xdb9000,0x3000),(0x6e7000,0x1000),(0x7e7000,0x1000),(0x99e000,0x1000),
                         (0xdae000,0x1000),(0x4aa000,0x1000),(0xf35000,0x1000),(0x122d000,0x1000),(0x100000,0x10000),(0x200000,0x5000)):
        uc.mem_map(address,size)
    uc.mem_write(0xdb97a0,data.bytes_at(0xdb97a0,7013))
    def put(a,v):uc.mem_write(a,v.to_bytes(4,'little'))
    def get(a):return int.from_bytes(uc.mem_read(a,4),'little')
    stack,thread,game,table=0x108000,0x200000,0x201000,0x202000
    put(thread+4,game);put(thread+0x14,0x203800);put(game,table);put(0x122dedc,0);put(0x204000,bits)
    health=0x203f00
    uc.mem_write(health,b'\xd9\x05'+struct.pack('<I',0x204000)+b'\xc2\x04\x00');put(table+0x420,health)
    calls={0x6e7b60:'movie',0x6e7b80:'movie.close',0x99ebf0:'key',0x99eae0:'key.close',0x7e7490:'thing',
           0x4aa840:'thing.close',0x7e7390:'speak',0x7e7450:'task',0xf35b30:'term',0xdaea70:'deed'}
    for i,(slot,name) in enumerate(((0x5c8,'start'),(0x5ec,'pause'),(0x118,'hero'),(0x1c,'frame'),(0x95c,'ally'))):
        target=0x203000+16*i;put(table+slot,target);calls[target]=name
    for target in calls:uc.mem_write(target,b'\xc3')
    events=[];counts={'task':0,'term':0};finished=[]
    def hook(machine,address,size,user):
        if address in (0xdba3db,0xdbb0d6):finished.append(address==0xdba3db);machine.emu_stop();return
        if address==health:
            esp=machine.reg_read(UC_X86_REG_ESP)
            assert machine.reg_read(UC_X86_REG_ECX)==game and get(esp+4)==stack+452
            events.append('health');return
        if address not in calls:return
        name=calls[address];esp=machine.reg_read(UC_X86_REG_ESP);receiver=machine.reg_read(UC_X86_REG_ECX)
        args=lambda n:[get(esp+4+4*i) for i in range(n)]
        pop,result=0,0xabcd0000
        if name=='movie':assert receiver==stack+328
        elif name=='movie.close':assert receiver==stack+328
        elif name=='key':assert receiver==stack+120 and args(2)==[0x122d70e,0xffffffff];pop=2
        elif name=='key.close':assert receiver==stack+120
        elif name=='start':assert receiver==game and args(2)==[stack+120,stack+328];pop=2
        elif name=='pause':assert receiver==game;name='pause:'+str(args(1)[0]);pop=1
        elif name=='thing':assert receiver==stack+24 and args(1)==[stack+452];result=stack+452;pop=1
        elif name=='thing.close':assert receiver==stack+452
        elif name=='hero':assert receiver==game;result=0
        elif name=='ally':
            assert receiver==game and args(2)==([thread+8,0] if events.count('ally')==0 else [0,thread+8]);pop=2
        elif name=='deed':assert receiver==0x203800 and args(1)==[2];pop=1
        elif name=='speak':assert receiver==stack+24 and args(6)==[0,0x12d97e8,0,0,1,0];pop=6
        elif name=='task':assert receiver==stack+24;counts['task']+=1;result|=int(counts['task']<=busy)
        elif name=='term':assert receiver==thread;counts['term']+=1;result|=int(counts['term']==cancel)
        elif name=='frame':assert receiver==game
        events.append(name)
        machine.reg_write(UC_X86_REG_EAX,result);machine.reg_write(UC_X86_REG_ESP,esp+4+4*pop);machine.reg_write(UC_X86_REG_EIP,get(esp))
    uc.reg_write(UC_X86_REG_ESP,stack);uc.reg_write(UC_X86_REG_EBP,thread);uc.reg_write(UC_X86_REG_EBX,thread+8)
    uc.hook_add(UC_HOOK_CODE,hook);uc.emu_start(0xdbaca1 if prelude else 0xdbace6,0xdbb0d7,count=1000)
    assert len(finished)==1 and uc.reg_read(UC_X86_REG_ESP)==stack
    return finished[0],events


def lua_phase(bits,busy,cancel,prelude=False):
    lua=LuaRuntime();folder=Path(__file__).parent
    source='\n'.join(re.sub(r'\nreturn \w+\s*$','\n',(folder/name).read_text()) for name in ('theresa_speech_body.lua','theresa_hit_movie.lua','theresa_hit_body.lua'))
    phase=lua.execute(source+'\nreturn '+('handleTheresaHit' if prelude else 'playTheresaHitResponse'))
    events=[];counts={'task':0,'term':0};q,r=lua.table(),lua.table()
    def start(*args):events.extend(['movie','key','start','key.close']);return 328
    def health(*args):events.append('health');return struct.unpack('<f',struct.pack('<I',bits))[0]
    def query(name):
        counts[name]+=1;events.append(name)
        return counts[name]<=busy if name=='task' else counts[name]==cancel
    r.StartMovie=start;r.Pause=lambda self,v:events.append('pause:'+str(int(v)))
    r.DestroyMovie=lambda *args:events.append('movie.close')
    r.NewThingFromResource=lambda *args:events.append('thing') or 452
    r.ThingHealth=health;r.DestroyThing=lambda *args:events.append('thing.close')
    r.SpeakTheresa=lambda *args:events.extend(['hero','speak'])
    r.SetVillagerHeroAllies=lambda *args:events.extend(['hero','ally','hero','ally'])
    helpers=lua.table()
    def deed(quest,actor,amount):assert amount==2;events.append('deed')
    helpers.AddBadDeed=deed
    lua.globals().package.loaded['NewOakValeIntro.native_quest_helpers']=helpers
    r.IsPerformingScriptTask=lambda *args:query('task')
    q.NewScriptFrame=lambda *args:events.append('frame');q.IsActiveThreadTerminating=lambda *args:query('term')
    return phase(q,17,r,24),events


class TheresaHitMovieTests(unittest.TestCase):
    def test_native_hit_prelude_before_movie(self):
        data=RData();verify(data)
        for bits,busy,cancel in itertools.product((0,0x3f800000,0x7fc12345),(0,1,3),range(1,6)):
            with self.subTest(bits=hex(bits),busy=busy,cancel=cancel):
                self.assertEqual(lua_phase(bits,busy,cancel,True),native(data,bits,busy,cancel,True))

    def test_health_error_closes_inner_thing_before_movie_even_if_cleanup_fails(self):
        lua=LuaRuntime();folder=Path(__file__).parent
        source='\n'.join(re.sub(r'\nreturn \w+\s*$','\n',(folder/name).read_text()) for name in ('theresa_speech_body.lua','theresa_hit_movie.lua'))
        phase=lua.execute(source+'\nreturn playTheresaHitResponse')
        for cleanup_fails in (False,True):
            state=lua.execute('''
                local fails=...
                local events={}
                local function close(name)
                    table.insert(events,name)
                    if fails then error('cleanup failed') end
                end
                return {events=events,r={
                    StartMovie=function()return 1 end,
                    Pause=function(_,value)if not value then close('unpause') end end,
                    NewThingFromResource=function()return 2 end,
                    ThingHealth=function()error('health failed') end,
                    DestroyThing=function()close('thing') end,
                    DestroyMovie=function()close('movie') end
                }}
            ''',cleanup_fails)
            with self.assertRaisesRegex(Exception,'health failed'):phase(lua.table(),17,state.r,24)
            self.assertEqual(list(state.events.values()),['thing','unpause','movie'])

    def test_native_health_speech_wait_and_cleanup(self):
        data=RData();verify(data)
        for bits,busy,cancel in itertools.product((0,0x80000000,1,0x3f800000,0xbf800000,0x7f800000,0xff800000,0x7fc12345),(0,1,3),range(1,6)):
            with self.subTest(bits=hex(bits),busy=busy,cancel=cancel):
                self.assertEqual(lua_phase(bits,busy,cancel),native(data,bits,busy,cancel))
