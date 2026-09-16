"""Native outro composition; actor-map construction is a separately tested boundary."""
import itertools
import re
import unittest
from pathlib import Path
from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_theresa_control import verify
from tools.script_recovery.test_native_affair_wife_hit_scopes import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP, UC_X86_REG_EIP, UC_X86_REG_ECX, UC_X86_REG_EDX, UC_X86_REG_EAX, UC_X86_REG_EBP


def native(data,cancel,acquired,hero,ids):
    uc=Uc(UC_ARCH_X86,UC_MODE_32)
    for address,size in ((0xdb9000,0x3000),(0x7e7000,0x1000),(0x6e7000,0x1000),(0x99e000,0x1000),
                         (0xcdb000,0x1000),(0xcbf000,0x1000),(0xf35000,0x1000),
                         (0x100000,0x10000),(0x200000,0x5000)):
        uc.mem_map(address,size)
    uc.mem_write(0xdb97a0,data.bytes_at(0xdb97a0,7013))
    def put(a,v): uc.mem_write(a,(v&0xffffffff).to_bytes(4,'little'))
    def get(a): return int.from_bytes(uc.mem_read(a,4),'little')
    stack,thread,game,table,parent=0x108000,0x200000,0x201000,0x202000,0x204000
    put(thread+4,game);put(game,table);put(thread+0x14,parent)
    for off,value in zip((0x64,0x5c,0x60),ids):put(parent+off,value)
    calls={0xf35b30:'term',0x7e72a0:'control',0x7e74d0:'release',0x6e7b60:'movie',0x6e7b80:'movie.close',
           0x99ebf0:'text',0x99eae0:'text.close',0xcbfb7d:'macro',0xcdbfb0:'map.close'}
    for i,(slot,name) in enumerate(((0x118,'hero'),(0x20,'acquire'),(0x5c8,'start'),(0x5ec,'pause'),
                                   (0x504,'display'),(0x548,'remove'),(0x5c4,'avi'),(0x5d4,'fade'),(0xae0,'music'))):
        target=0x203000+i*16;put(table+slot,target);calls[target]=name
    for target in calls:uc.mem_write(target,b'\xc3')
    events,strings,finished=[],{},[]
    def hook(machine,address,size,user):
        if address in (0xdbb0d6,0xdbb2d8):finished.append(address==0xdbb2d8);machine.emu_stop();return
        if address==0xdbb1a6:
            events.append(('actors',));machine.reg_write(UC_X86_REG_EIP,0xdbb218);return
        if address==0xdbb2a7:events.append(('attack',))
        if address not in calls:return
        name=calls[address];esp=machine.reg_read(UC_X86_REG_ESP);receiver=machine.reg_read(UC_X86_REG_ECX)
        args=lambda n:[get(esp+4+i*4) for i in range(n)]
        pop,result=0,0xabcd0000;event=(name,)
        if name=='term':assert receiver==thread;result|=int(cancel)
        elif name=='text':
            assert args(2)[1]==0xffffffff
            strings[receiver]=data.bytes_at(args(1)[0],100).split(b'\0')[0].decode();pop=2
            event=('text',strings[receiver])
        elif name=='text.close':event=('text.close',strings.pop(receiver))
        elif name in ('control','release'):assert receiver==stack+328
        elif name in ('movie','movie.close'):assert receiver==stack+384
        elif name=='map.close':assert receiver==stack+60
        elif name=='hero':assert receiver==game;result=hero
        elif name=='acquire':
            assert receiver==game and args(3)==[hero,stack+328,4];pop=3;result|=int(acquired)
        elif name=='start':assert receiver==game and args(2)==[stack+56,stack+384];pop=2
        elif name in ('display','pause'):assert receiver==game;event=(name,args(1)[0]);pop=1
        elif name=='remove':assert receiver==game;event=(name,args(1)[0]);pop=1
        elif name=='macro':
            assert strings[receiver]=='CS_OAKVALE_INTRO_THERESA'
            assert machine.reg_read(UC_X86_REG_EDX)==stack+60 and args(4)==[0,0,0,1];pop=4
        elif name=='avi':assert receiver==game;event=('avi',strings[args(1)[0]]);pop=1
        elif name=='fade':assert receiver==game and args(3)==[0x3f000000,0,0xff000000];pop=3
        elif name=='music':assert receiver==game and args(3)==[25,0,0];pop=3
        events.append(event)
        machine.reg_write(UC_X86_REG_EAX,result);machine.reg_write(UC_X86_REG_ESP,esp+4+4*pop)
        machine.reg_write(UC_X86_REG_EIP,get(esp))
    uc.reg_write(UC_X86_REG_ESP,stack);uc.reg_write(UC_X86_REG_EBP,thread)
    uc.hook_add(UC_HOOK_CODE,hook);uc.emu_start(0xdbb0e4,0xdbb2e9,count=2000)
    assert len(finished)==1 and not strings and uc.reg_read(UC_X86_REG_ESP)==stack
    return finished[0],bool(uc.mem_read(parent+0x50,1)[0]),events


def lua_phase(cancel,acquired,hero,ids):
    lua=LuaRuntime();folder=Path(__file__).parent
    source='\n'.join(re.sub(r'\nreturn \w+\s*$','\n',(folder/name).read_text()) for name in
                     ('theresa_cutscene_actors.lua','theresa_outro_body.lua'))
    phase=lua.execute(source+'\nreturn finishTheresaChildhood')
    q,r=lua.table(),lua.table();events=[];state={};roles=[]
    fields=dict(zip(('GUIBullyHealthCounter','GUIGoodDeedCounter','GUIBarrelCounter'),ids))
    q.IsActiveThreadTerminating=lambda *args:events.append(('term',)) or cancel
    q.DisplayQuestInfo=lambda self,v:events.append(('display',int(v)))
    q.GetStateInt=lambda self,key:fields[key]
    q.RemoveQuestInfoElement=lambda self,value:events.append(('remove',value&0xffffffff))
    def start(self,name):
        assert name=='';events.extend([('movie',),('text',''),('start',),('text.close','')]);return 384
    r.StartMovie=start;r.Pause=lambda self,v:events.append(('pause',int(v)))
    r.NewResource=lambda *args:events.append(('control',)) or 328
    def acquire(self,control,priority):
        assert (control,priority)==(328,4);events.extend([('hero',),('acquire',)]);return acquired
    r.TryAcquireTheresaHero=acquire
    r.NewActorMap=lambda *args:events.append(('actors',)) or 60
    def actor(self,map_id,key,control):assert map_id==60;roles.append((key,control))
    r.SetActor=actor
    def macro(self,name,map_id,setup,skippable):
        assert (name,map_id,setup,skippable)==('CS_OAKVALE_INTRO_THERESA',60,False,True)
        assert roles==[('HERO',328),('Theresa',24)]
        events.extend([('text',name),('macro',),('text.close',name)])
    r.RunMacro=macro
    def avi(self,name):events.extend([('text',name),('avi',name),('text.close',name)])
    q.PlayAVIMovie=avi
    def fade(self,time,hold):assert (time,hold)==(0.5,0);events.append(('fade',))
    def music(self,kind,cutscene,force):assert (kind,cutscene,force)==(25,False,False);events.append(('music',))
    q.FadeScreenOut=fade;q.OverrideMusic=music
    def attack(self,key,value):assert key=='AttackOver' and value;state[key]=value;events.append(('attack',))
    q.SetStateBool=attack
    r.DestroyActorMap=lambda self,value:events.append(('map.close',))
    r.ReleaseResource=lambda self,value:events.append(('release',))
    r.DestroyMovie=lambda self,value:events.append(('movie.close',))
    result=phase(q,17,r,24)
    return result,state.get('AttackOver',False),events


class TheresaOutroTests(unittest.TestCase):
    def test_lua_partial_construction_and_cleanup_errors(self):
        for failure in ('control','acquire','map','actor','macro','avi','fade','music','attack','map.close','release','unpause','movie.close'):
            with self.subTest(failure=failure):
                lua=LuaRuntime();folder=Path(__file__).parent
                source='\n'.join(re.sub(r'\nreturn \w+\s*$','\n',(folder/name).read_text()) for name in
                                 ('theresa_cutscene_actors.lua','theresa_outro_body.lua'))
                check=lua.execute(source+'''
return function(failure)
    local events={}
    local function emit(name)
        events[#events+1]=name
        if name==failure then error("primary:"..name,0) end
        if name=="movie.close" then error("secondary",0) end
    end
    local q={
        IsActiveThreadTerminating=function() return false end,
        DisplayQuestInfo=function() end, GetStateInt=function() return 0 end,
        RemoveQuestInfoElement=function() end,
        PlayAVIMovie=function() emit("avi") end,
        FadeScreenOut=function() emit("fade") end,
        OverrideMusic=function() emit("music") end,
        SetStateBool=function() emit("attack") end,
    }
    local r={
        StartMovie=function() return 384 end,
        Pause=function(_,on) if not on then emit("unpause") end end,
        NewResource=function() emit("control"); return 328 end,
        TryAcquireTheresaHero=function() emit("acquire"); return false end,
        NewActorMap=function() emit("map"); return 60 end,
        SetActor=function() emit("actor") end,
        RunMacro=function() emit("macro") end,
        DestroyActorMap=function() emit("map.close") end,
        ReleaseResource=function() emit("release") end,
        DestroyMovie=function() emit("movie.close") end,
    }
    local ok,err=pcall(finishTheresaChildhood,q,17,r,24)
    assert(not ok and err=="primary:"..failure,tostring(err))
    assert(events[#events-1]=="unpause" and events[#events]=="movie.close")
    local mapClose,release
    for i,name in ipairs(events) do
        if name=="map.close" then assert(not mapClose); mapClose=i end
        if name=="release" then assert(not release); release=i end
    end
    assert((release~=nil)==(failure~="control"))
    assert((mapClose~=nil)==(failure~="control" and failure~="acquire" and failure~="map"))
    assert(not mapClose or mapClose<release)
end
''')
                check(failure)

    def test_native_outro_operands_state_and_cleanup(self):
        data=RData();verify(data)
        for case in itertools.product((False,True),(False,True),(0,0x204800),((-999,0,17),(901,33,-1))):
            with self.subTest(case=case):self.assertEqual(lua_phase(*case),native(data,*case))
