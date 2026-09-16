"""Native outer meeting versus Lua; actor-map/acceptance bodies are boundaries.

Question construction is abstracted, but original signed answer polling executes.
Engine APIs and CString bodies remain doubles, as in the individual phase tests.
"""
import itertools
import re
import unittest
from pathlib import Path
from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_theresa_control import verify
from tools.script_recovery.test_theresa_chocolate_question import Scenario
from tools.script_recovery.test_native_affair_wife_hit_scopes import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP, UC_X86_REG_EIP, UC_X86_REG_ECX, UC_X86_REG_EDX, UC_X86_REG_EAX, UC_X86_REG_EBP


def native(data, scenario, has, acquired, hero):
    uc = Uc(UC_ARCH_X86, UC_MODE_32)
    for address, size in ((0xdb9000,0x3000),(0x7e7000,0x1000),(0x6e7000,0x1000),(0x99e000,0x1000),
                          (0xcdb000,0x1000),(0xcdf000,0x1000),(0xcbf000,0x1000),(0xf35000,0x1000),
                          (0x100000,0x10000),(0x200000,0x4000)):
        uc.mem_map(address,size)
    uc.mem_write(0xdb97a0,data.bytes_at(0xdb97a0,7013))
    def put(a,v): uc.mem_write(a,v.to_bytes(4,'little'))
    def get(a): return int.from_bytes(uc.mem_read(a,4),'little')
    stack,thread,game,table=0x108000,0x200000,0x201000,0x202000
    put(thread+4,game);put(game,table)
    calls={0xf35b30:'term',0x7e72a0:'control',0x7e74d0:'release',0x6e7b60:'movie',0x6e7b80:'movie.close',
           0x99ebf0:'text',0x99eae0:'text.close',0xcbfb7d:'macro',0xcdbfb0:'map.close'}
    for i,(slot,name) in enumerate(((0x118,'hero'),(0x20,'acquire'),(0x5c8,'movie.start'),(0x5ec,'pause'),
                                  (0x5cc,'camera'),(0x2e0,'has'),(0x9c,'answer'),(0x1c,'frame'))):
        target=0x203000+i*16;put(table+slot,target);calls[target]=name
    for target in calls:uc.mem_write(target,b'\xc3')
    strings,finished={},[]
    def hook(machine,address,size,user):
        if address in (0xdba40f,0xdbb2e8):finished.append(address==0xdba40f);machine.emu_stop();return
        if address==0xdb9a10:
            scenario.events.append(('actors',));machine.reg_write(UC_X86_REG_EIP,0xdb9a9d);return
        if address==0xdb9b8e:
            scenario.events.append(('question',));machine.reg_write(UC_X86_REG_EIP,0xdb9c25);return
        if address==0xdb9c87:
            scenario.events.append(('accepted',));machine.reg_write(UC_X86_REG_EIP,0xdb9e86);return
        if address==0xdb9e92:scenario.events.append(('done',))
        if address not in calls:return
        name=calls[address];esp=machine.reg_read(UC_X86_REG_ESP);receiver=machine.reg_read(UC_X86_REG_ECX)
        args=lambda n:[get(esp+4+4*i) for i in range(n)]
        pop,result=0,0xabcd0000
        if name=='term':
            assert receiver==thread;result|=int(scenario.term())
        elif name=='text':
            assert args(2)[1]==0xffffffff
            strings[receiver]=data.bytes_at(args(2)[0],100).split(b'\0')[0].decode();pop=2
        elif name=='text.close':strings.pop(receiver)
        elif name=='control':assert receiver==stack+344;scenario.events.append(('control',))
        elif name=='release':assert receiver==stack+344;scenario.events.append(('release',))
        elif name=='movie':assert receiver==stack+204;scenario.events.append(('movie',))
        elif name=='movie.close':assert receiver==stack+204;scenario.events.append(('movie.close',))
        elif name=='map.close':assert receiver==stack+228;scenario.events.append(('map.close',))
        elif name=='macro':
            assert strings[receiver]=='CS_OAKVALE_INTRO_THERESA_MEET'
            assert machine.reg_read(UC_X86_REG_EDX)==stack+228 and args(4)==[0,0,0,1]
            scenario.events.append(('macro',));pop=4
        else:
            assert receiver==game
            if name=='hero':result=hero;scenario.events.append(('hero',))
            elif name=='acquire':
                assert args(3)==[hero,stack+344,4];pop=3;result|=int(acquired);scenario.events.append(('acquire',))
            elif name=='movie.start':
                assert strings[args(2)[0]]=='' and args(2)[1]==stack+204;pop=2;scenario.events.append(('movie.start',))
            elif name in ('pause','camera'):scenario.events.append((name,bool(args(1)[0])));pop=1
            elif name=='has':
                assert strings[args(2)[0]]=='OBJECT_CHOCOLATE_BOX_UNGIVEABLE' and args(2)[1]==hero
                pop=2;result|=int(has);scenario.events.append(('has',))
            elif name=='answer':result=scenario.answer()&0xffffffff
            else:scenario.events.append(('frame',))
        machine.reg_write(UC_X86_REG_EAX,result);machine.reg_write(UC_X86_REG_ESP,esp+4+4*pop)
        machine.reg_write(UC_X86_REG_EIP,get(esp))
    uc.reg_write(UC_X86_REG_ESP,stack);uc.reg_write(UC_X86_REG_EBP,thread)
    uc.hook_add(UC_HOOK_CODE,hook);uc.emu_start(0xdb99ca,0xdbb2e9,count=3000)
    assert len(finished)==1 and not strings and uc.reg_read(UC_X86_REG_ESP)==stack
    assert bool(uc.mem_read(thread+0x1d,1)[0])==finished[0]
    return finished[0]


def lua_phase(scenario,has,acquired):
    lua=LuaRuntime();folder=Path(__file__).parent
    source='\n'.join(re.sub(r'\nreturn \w+\s*$','\n',(folder/name).read_text()) for name in
                     ('theresa_chocolate_question.lua','theresa_meeting_body.lua'))
    lua.globals().actors=lambda *args:scenario.events.append(('actors',)) or 228
    lua.globals().accepted=lambda *args:scenario.events.append(('accepted',))
    phase=lua.execute('local newTheresaCutsceneActors=actors\nlocal acceptTheresaChocolates=accepted\n'+source+'\nreturn meetTheresa')
    q,r,state=lua.table(),lua.table(),lua.table()
    q.IsActiveThreadTerminating=lambda self:scenario.term()
    q.MsgIsQuestionAnsweredYesOrNo=lambda self:scenario.answer()
    q.NewScriptFrame=lambda *args:scenario.events.append(('frame',))
    q.FixMovieSequenceCamera=lambda self,value:scenario.events.append(('camera',value))
    state.SetStateBool=lambda *args:scenario.events.append(('done',))
    r.NewResource=lambda *args:scenario.events.append(('control',)) or 344
    def acquire(*args):scenario.events.extend([('hero',),('acquire',)]);return acquired
    def movie(*args):scenario.events.extend([('movie',),('movie.start',)]);return 204
    def possession(*args):scenario.events.extend([('hero',),('has',)]);return has
    r.TryAcquireTheresaHero=acquire;r.StartMovie=movie;r.DoesTheresaHeroHaveChocolates=possession
    r.Pause=lambda self,value:scenario.events.append(('pause',value))
    r.RunMacro=lambda *args:scenario.events.append(('macro',))
    r.ShowTheresaChocolateQuestion=lambda *args:scenario.events.append(('question',))
    r.DestroyMovie=lambda *args:scenario.events.append(('movie.close',))
    r.DestroyActorMap=lambda *args:scenario.events.append(('map.close',))
    r.ReleaseResource=lambda *args:scenario.events.append(('release',))
    return phase(q,17,r,24,state,lua.table())


class NativeTheresaMeetingTests(unittest.TestCase):
    def test_outer_native_meeting_and_cleanup(self):
        data=RData();verify(data)
        for has,acquired,answer,cancel,hero in itertools.product((False,True),(False,True),(0,1,2),range(1,9),(0,0x202800)):
            with self.subTest(has=has,acquired=acquired,answer=answer,cancel=cancel,hero=hero):
                expected,actual=Scenario((-1,answer),cancel),Scenario((-1,answer),cancel)
                self.assertEqual(lua_phase(actual,has,acquired),native(data,expected,has,acquired,hero))
                self.assertEqual(actual.events,expected.events)
