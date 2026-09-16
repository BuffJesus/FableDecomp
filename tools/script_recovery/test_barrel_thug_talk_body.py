"""Original conversation branches, movie/acquisition lifetime and x87 speech wait.

Engine calls are doubles. The composed run executes the actual cancellation
epilogues, including both inline movie-destruction stages on acquire cancellation.
Those stages are normalized to movie.close after checking order and receivers.
Lua StartMovie's internal constructor/string steps and the caller's control close
are modeled here; full runtime owner integration remains a separate gate.
"""
import hashlib
import itertools
import struct
import unittest
from pathlib import Path
from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.test_native_affair_wife_hit_scopes import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP, UC_X86_REG_EIP, UC_X86_REG_ECX, UC_X86_REG_EAX, UC_X86_REG_ESI, UC_X86_REG_EDI


def native(data,returned,broken,bits,busy,cancel,scope=None,hit=False):
    uc=Uc(UC_ARCH_X86,UC_MODE_32)
    for a,n in ((0xdb6000,0x2000),(0x7e7000,0x1000),(0x4aa000,0x1000),(0xf35000,0x1000),
                (0x122d000,0x1000),(0x100000,0x10000),(0x200000,0x5000),
                (0x99a000,0x1000),(0x99e000,0x1000),(0x6e7000,0x1000),(0xcd2000,0x1000),(0xdae000,0x1000)):
        uc.mem_map(a,n)
    uc.mem_write(0xdb6c60,data.bytes_at(0xdb6c60,0x1091))
    def put(a,v):uc.mem_write(a,v.to_bytes(4,'little'))
    def get(a):return int.from_bytes(uc.mem_read(a,4),'little')
    stack,thread,game,table,parent=0x108000,0x200000,0x201000,0x202000,0x204000
    put(thread+4,game);put(thread+0x14,parent);put(game,table)
    uc.mem_write(parent+0x73,bytes([returned]));uc.mem_write(parent+0x75,bytes([broken]))
    health=0x203f00;put(0x204100,bits);put(0x122dedc,0)
    uc.mem_write(health,b'\xd9\x05'+struct.pack('<I',0x204100)+b'\xc2\x04\x00');put(table+0x420,health)
    put(table+0x118,0x203000);put(table+0x1c,0x203010)
    calls={0xf35b30:'term',0x7e7490:'thing',0x4aa840:'thing.close',0x7e7390:'speak',0x7e7450:'task',0x203000:'hero',0x203010:'frame'}
    if scope is not None:
        pending,held=scope
        calls.update({0x99a380:'movie.new',0x99ebf0:'key.new',0x99eae0:'key.close',
                      0x203020:'movie.start',0x203030:'pause',0xcd23b9:'has',0xcd2770:'reset',
                      0x203040:'acquire',0x6e7b80:'movie.close',0x6e7ab0:'movie.imp.close',
                      0x99a430:'movie.base.close',0x7e74d0:'control.close'})
        for offset,address in ((0x5c8,0x203020),(0x5ec,0x203030),(0x20,0x203040)):put(table+offset,address)
        put(stack+0xf8,0x203ff0);uc.mem_write(0x203ff0,b'\xc3')
    if hit:
        assert scope is not None
        calls.update({0x6e7b60:'movie.new',0x203050:'ally',0xdaea70:'baddeed'})
        put(table+0x95c,0x203050)
    for a in calls:uc.mem_write(a,b'\xc3')
    movie_slot,key_slot=(128,76) if hit else (40,64)
    slot=200 if hit else {(True,True):188,(True,False):212,(False,True):236,(False,False):224}[(returned,broken)]
    events=[];counts=dict(term=0,task=0,acquire=0);finished=[];inline=[]
    def hook(machine,a,n,user):
        stops=((0xdb7bb0 if hit else 0xdb73a0),0x203ff0) if scope is not None else (0xdb738a,0xdb7c7e,0xdb7c90,0xdb71e5,0xdb7ca2)
        if a in stops:finished.append(a in (0xdb738a,0xdb73a0,0xdb7bb0));machine.emu_stop();return
        if a==0xdb7052:events.append(('state','BarrelManSpokenToHeroOnReturn'))
        if a in (0xdb706f,0xdb7208):events.append(('state','BarrelBrokenPersistent'))
        if a==health:
            assert get(machine.reg_read(UC_X86_REG_ESP)+4)==stack+slot
            events.append(('health',));return
        if a not in calls:return
        name=calls[a];esp=machine.reg_read(UC_X86_REG_ESP);receiver=machine.reg_read(UC_X86_REG_ECX)
        args=lambda n:[get(esp+4+i*4) for i in range(n)]
        pop,result=0,0xabcd0000;event=(name,)
        if name in ('term','task'):
            assert receiver==(thread if name=='term' else stack+16)
            counts[name]+=1;value=counts[name]==cancel if name=='term' else counts[name]<=busy
            result|=int(value);event=(name,value)
        elif name=='thing':assert receiver==stack+16 and args(1)==[stack+slot];result=stack+slot;pop=1
        elif name=='thing.close':assert receiver==stack+slot
        elif name=='hero':assert receiver==game;result=0
        elif name=='frame':assert receiver==game
        elif name=='speak':
            a=args(6);assert receiver==stack+16 and a[0]==0 and a[2:]==[0 if hit or returned else 2,0,1,0]
            event=('speak',data.string_at(a[1]),a[2]);pop=6
        elif name=='movie.new':assert receiver==stack+movie_slot
        elif name=='key.new':
            assert receiver==stack+key_slot and args(2)==[0x122d70e,0xffffffff]
            event=('key.new','');pop=2
        elif name=='key.close':assert receiver==stack+key_slot
        elif name=='movie.start':
            assert receiver==game and args(2)==[stack+key_slot,stack+movie_slot]
            if not hit:assert get(stack+40)==0x1260ef4 and get(stack+48)==0 and get(stack+52)==0
            pop=2
        elif name=='pause':
            assert receiver==game and args(1)[0] in (0,1)
            event=('pause',bool(args(1)[0]));pop=1
        elif name in ('has','reset'):
            assert receiver==stack+16
            if name=='has':result=int(held);event=('has',held)
        elif name=='acquire':
            assert receiver==game and args(3)==[0x204200,stack+16,4]
            counts['acquire']+=1;result=int(counts['acquire']>pending)
            event=('acquire',bool(result));pop=3
        elif name=='movie.close':assert receiver==stack+movie_slot
        elif name=='ally':
            assert receiver==game
            previous=sum(e[0]=='ally' for e in events)
            assert args(2)==([0x204200,0] if previous==0 else [0,0x204200])
            event=('ally',previous);pop=2
        elif name=='baddeed':assert receiver==parent and args(1)==[2];event=('baddeed',2);pop=1
        elif name=='movie.imp.close':
            assert receiver==stack+48;inline.append(name);event=None
        elif name=='movie.base.close':
            assert receiver==stack+40 and get(stack+40)==0x126008c
            assert inline==['movie.imp.close'];inline.append(name);event=('movie.close',)
        elif name=='control.close':assert receiver==stack+16
        if event is not None:events.append(event)
        machine.reg_write(UC_X86_REG_EAX,result)
        machine.reg_write(UC_X86_REG_ESP,esp+4+4*pop);machine.reg_write(UC_X86_REG_EIP,get(esp))
    uc.reg_write(UC_X86_REG_ESP,stack);uc.reg_write(UC_X86_REG_ESI,thread)
    uc.reg_write(UC_X86_REG_EDI,0x204200)
    uc.hook_add(UC_HOOK_CODE,hook);uc.emu_start(0xdb79fc if hit else (0xdb6f86 if scope is not None else 0xdb7052),0xdb7cf1,count=4000)
    assert len(finished)==1
    assert uc.reg_read(UC_X86_REG_ESP)==stack+(0xfc if scope is not None and not finished[0] else 0)
    if inline:assert inline==['movie.imp.close','movie.base.close']
    return finished[0],events


def readable(returned,broken,bits,busy,cancel,scope=None,hit=False):
    lua=LuaRuntime();source=Path(__file__).with_name('barrel_thug_talk_body.lua').read_text()
    if scope is not None:
        source=source.rsplit('return speakBarrelThugConversation',1)[0]+Path(__file__).with_name('barrel_thug_conversation.lua').read_text()
    if hit:source=Path(__file__).with_name('barrel_thug_hit.lua').read_text()
    phase=lua.execute(source)
    events=[];counts=dict(term=0,task=0,acquire=0);q,r=lua.table(),lua.table()
    def state(self,key):events.append(('state',key));return returned if key=='BarrelManSpokenToHeroOnReturn' else broken
    def query(name):
        counts[name]+=1;value=counts[name]==cancel if name=='term' else counts[name]<=busy
        events.append((name,value));return value
    q.GetStateBool=state;q.IsActiveThreadTerminating=lambda *args:query('term')
    q.NewScriptFrame=lambda *args:events.append(('frame',))
    r.NewThingFromResource=lambda *args:events.append(('thing',)) or 1
    r.ThingHealth=lambda *args:events.append(('health',)) or struct.unpack('<f',struct.pack('<I',bits))[0]
    r.DestroyThing=lambda *args:events.append(('thing.close',))
    r.SpeakBarrelThug=lambda self,c,line,selection:events.extend([('hero',),('speak',line,selection)])
    r.IsPerformingScriptTask=lambda *args:query('task')
    if scope is not None:
        pending,held=scope
        def start(self,key):
            assert key==''
            events.extend([('movie.new',),('key.new',''),('movie.start',),('key.close',)])
            return 40
        def prepare(self,control):
            assert control==16
            events.append(('has',held))
            if held:events.append(('reset',))
        def acquire(self,control,actor,priority):
            assert (control,actor,priority)==(16,17,4)
            counts['acquire']+=1;value=counts['acquire']>pending
            events.append(('acquire',value));return value
        r.StartMovie=start;r.PrepareResource=prepare;r.TryAcquire=acquire
        r.Pause=lambda self,value:events.append(('pause',value))
        r.DestroyMovie=lambda self,movie:events.append(('movie.close',))
    if hit:
        r.SetVillagerHeroAllies=lambda self,actor:events.extend([('hero',),('ally',0),('hero',),('ally',1)])
        lua.globals().baddeed=lambda quest,actor,deed:events.append(('baddeed',deed))
        lua.execute('package.loaded["NewOakValeIntro.native_quest_helpers"]={AddBadDeed=baddeed}')
    result=phase(q,17,r,16)
    if scope is not None and not result:events.append(('control.close',))
    return result,events


class BarrelThugTalkTests(unittest.TestCase):
    def test_hit_consequences_movie_acquisition_speech_and_cancellation(self):
        data=RData()
        self.assertEqual(hashlib.sha256(data.bytes_at(0xdb6c60,0x1091)).hexdigest(),'eafde7cb9a35b3f6d7c158c455496af2ea89444352b04e4f7b39dcbde39ada09')
        for bits,busy,cancel,pending,held in itertools.product((0,0x3f800000,0xbf800000,0x7fc12345),(0,2),range(10),(0,1,3),(False,True)):
            case=(False,False,bits,busy,cancel,(pending,held),True)
            with self.subTest(case=case):self.assertEqual(readable(*case),native(data,*case))

    def test_four_dialogue_branches_health_and_cancellation(self):
        data=RData()
        self.assertEqual(hashlib.sha256(data.bytes_at(0xdb6c60,0x1091)).hexdigest(),'eafde7cb9a35b3f6d7c158c455496af2ea89444352b04e4f7b39dcbde39ada09')
        for case in itertools.product((False,True),(False,True),(0,0x3f800000,0xbf800000,0x7fc12345),(0,1,3),range(1,8)):
            with self.subTest(case=case):self.assertEqual(readable(*case),native(data,*case))

    def test_composed_movie_acquisition_speech_and_real_cancel_epilogues(self):
        data=RData()
        self.assertEqual(hashlib.sha256(data.bytes_at(0xdb6c60,0x1091)).hexdigest(),'eafde7cb9a35b3f6d7c158c455496af2ea89444352b04e4f7b39dcbde39ada09')
        for case in itertools.product((False,True),(False,True),(0,0x3f800000,0xbf800000,0x7fc12345),(0,2),range(0,10)):
            for scope in itertools.product((0,1,3),(False,True)):
                with self.subTest(case=case,scope=scope):
                    self.assertEqual(readable(*case,scope),native(data,*case,scope))

    def test_bridge_errors_still_attempt_movie_cleanup_and_preserve_first_error(self):
        source=Path(__file__).with_name('barrel_thug_talk_body.lua').read_text().rsplit('return speakBarrelThugConversation',1)[0]
        source+=Path(__file__).with_name('barrel_thug_conversation.lua').read_text()
        for failing in ('prepare','acquire','health','speak','task','frame'):
            for cleanup_failure in (False,True):
                with self.subTest(failing=failing,cleanup_failure=cleanup_failure):
                    lua=LuaRuntime();lua.globals().phase=lua.execute(source)
                    lua.globals().failing=failing;lua.globals().cleanupFailure=cleanup_failure
                    lua.execute('''
                        local events = {}
                        local function call(name)
                            events[#events+1] = name
                            if name == failing then error("original:"..name, 0) end
                        end
                        local quest = {
                            IsActiveThreadTerminating = function() return false end,
                            GetStateBool = function() return false end,
                            NewScriptFrame = function() call("frame") end,
                        }
                        local resources = {
                            StartMovie = function() call("movie.new"); return 40 end,
                            Pause = function(_, paused)
                                if paused then call("pause") else
                                    call("unpause")
                                    if cleanupFailure then error("cleanup:pause", 0) end
                                end
                            end,
                            PrepareResource = function() call("prepare") end,
                            TryAcquire = function() call("acquire"); return true end,
                            NewThingFromResource = function() call("thing"); return 1 end,
                            ThingHealth = function() call("health"); return 1 end,
                            DestroyThing = function() call("thing.close") end,
                            SpeakBarrelThug = function() call("speak") end,
                            IsPerformingScriptTask = function() call("task"); return true end,
                            DestroyMovie = function(_, movie)
                                assert(movie == 40); call("movie.close")
                                if cleanupFailure then error("cleanup:movie", 0) end
                            end,
                        }
                        local ok, err = pcall(phase, quest, 17, resources, 16)
                        assert(not ok and err == "original:"..failing)
                        assert(events[#events-1] == "unpause" and events[#events] == "movie.close")
                        local things, movies = 0, 0
                        for _, name in ipairs(events) do
                            if name == "thing" then things = things + 1 end
                            if name == "thing.close" then things = things - 1 end
                            if name == "movie.close" then movies = movies + 1 end
                        end
                        assert(things == 0 and movies == 1)
                    ''')
