"""Original offer control flow versus composed Lua.

Guard construction and the separately tested gift-commit tail are explicit
boundaries. Question, health, speech waits and native cleanup execute as bytes.
"""
import itertools
import re
import struct
import unittest
from pathlib import Path

from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_theresa_control import verify
from tools.script_recovery.test_native_affair_wife_hit_scopes import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP, UC_X86_REG_EIP, UC_X86_REG_ECX, UC_X86_REG_EAX, UC_X86_REG_EBP, UC_X86_REG_EDX
from tools.script_recovery.test_theresa_chocolate_question import KEYS


def run_native(data, answer, pending, bits, busy, cancel, presented=False, talk=None):
    movie_slot = (400 if answer==1 else 384) if presented else 288
    key_slot = (180 if answer==1 else 76) if presented else 200
    thing_slot = (500 if answer==1 else 416) if presented else (464 if answer==1 else 428)
    guard_slot = 316 if presented else 304
    success = (0xdba814 if answer==1 else 0xdba3db) if presented else 0xdba3db
    exits = {success,0xdbb0d6}
    exits.update((0xdbafff,0xdbb030,0xdbb056,0xdbb07c) if presented else (0xdbaf45,0xdbaf6a,0xdbaf9c,0xdbafce))
    if talk is not None:
        movie_slot,key_slot=252,88
        thing_slot=440 if talk[0] else (488 if talk[1] else 476)
        success=0xdba3db
        exits={success,0xdbb0d6,0xdbaadb,0xdbb0a2}
    uc = Uc(UC_ARCH_X86, UC_MODE_32)
    for address, size in ((0xdb9000,0x3000),(0x6e7000,0x1000),(0x7e7000,0x1000),
                          (0x99e000,0x1000),(0x4aa000,0x1000),(0x8ac000,0x1000),
                          (0xcbe000,0x1000),(0xf35000,0x1000),(0x122d000,0x1000),
                          (0x100000,0x10000),(0x200000,0x5000)):
        uc.mem_map(address,size)
    uc.mem_write(0xdb97a0,data.bytes_at(0xdb97a0,7013))
    def put(a,v): uc.mem_write(a,v.to_bytes(4,'little'))
    def get(a): return int.from_bytes(uc.mem_read(a,4),'little')
    stack,thread,game,table = 0x108000,0x200000,0x201000,0x202000
    put(thread+4,game); put(game,table); put(0x122dedc,0); put(0x204000,bits)
    if talk is not None:
        uc.mem_write(stack+0x17,bytes([int(talk[0])]))
        uc.mem_write(thread+0x1c,bytes([int(talk[1])]))
    health = 0x203f00
    uc.mem_write(health,b'\xd9\x05'+struct.pack('<I',0x204000)+b'\xc2\x04\x00')
    put(table+0x420,health)
    calls = {0x6e7b60:'movie',0x6e7b80:'movie.close',0x99ebf0:'key',0x99eae0:'key.close',
             0x7e7490:'thing',0x4aa840:'thing.close',0x7e7390:'speak',0x7e7450:'task',
             0xf35b30:'term',0xcbed82:'guards.remove',0x8ac970:'guards.close'}
    for i,(slot,name) in enumerate(((0x5c8,'start'),(0x5ec,'pause'),(0x118,'hero'),
                                   (0x1c,'frame'),(0x1c8,'question'),(0x9c,'answer'))):
        target=0x203000+i*16; put(table+slot,target); calls[target]=name
    for target in calls: uc.mem_write(target,b'\xc3')
    events,strings,finished = [],{},[]
    counts = dict(term=0,task=0,answer=0)
    given = False
    def hook(machine,address,size,user):
        nonlocal given
        if address in exits:
            finished.append(address==success); machine.emu_stop(); return
        if address==(0xdba60a if presented else 0xdba10f):
            events.append(('guards',)); machine.reg_write(UC_X86_REG_EIP,0xdba659 if presented else 0xdba15e); return
        if address==(0xdba70b if presented else 0xdba213):
            events.extend([('state',),('gift',),('clear',)]); given=True
            machine.reg_write(UC_X86_REG_EIP,0xdba7f0 if presented else 0xdba301); return
        if address==health:
            assert get(machine.reg_read(UC_X86_REG_ESP)+4)==stack+thing_slot
            events.append(('health',)); return
        if address not in calls: return
        name=calls[address]; esp=machine.reg_read(UC_X86_REG_ESP); receiver=machine.reg_read(UC_X86_REG_ECX)
        args=lambda n:[get(esp+4+i*4) for i in range(n)]
        pop,result=0,0xabcd0000
        event=(name,)
        if name in ('movie','movie.close'): assert receiver==stack+movie_slot
        elif name=='key':
            assert args(2)[1]==0xffffffff
            key=data.bytes_at(args(1)[0],96).split(b'\0')[0].decode()
            strings[receiver]=key; event=('key',key); pop=2
        elif name=='key.close': event=('key.close',strings.pop(receiver))
        elif name=='start': assert receiver==game and args(2)==[stack+key_slot,stack+movie_slot]; pop=2
        elif name=='pause': assert receiver==game; event=('pause',args(1)[0]); pop=1
        elif name=='question':
            assert receiver==game and [strings[x] for x in args(4)]==list(reversed(KEYS)) and args(5)[4]==1
            pop=5
        elif name=='answer':
            counts[name]+=1; value=-0x80000000 if counts[name]<=pending else answer
            result=value&0xffffffff; event=('answer',value)
        elif name in ('term','task'):
            assert receiver==(thread if name=='term' else stack+24)
            counts[name]+=1
            value=counts[name]==cancel if name=='term' else counts[name]<=busy
            result|=int(value); event=(name,value)
        elif name=='thing':
            assert receiver==stack+24 and args(1)==[stack+thing_slot]
            result=args(1)[0]; pop=1
        elif name=='thing.close': assert receiver==stack+thing_slot
        elif name=='hero': assert receiver==game; result=0
        elif name=='speak':
            line='TEXT_QST_048_THERESA_HELLO' if answer==1 else 'TEXT_QST_048_THERESA_REALLY_GET_PRESENT'
            if presented and answer!=1: line='TEXT_QST_048_THERESA_BETTER_PRESENT'
            if talk is not None:
                line='TEXT_QST_048_THERESA_'+('HELLO' if talk[0] else ('REALLY_GET_PRESENT' if talk[1] else 'GET_PRESENT'))
            a=args(6); assert receiver==stack+24 and a[0]==0 and a[2:]==[0,0,1,0]
            assert data.bytes_at(a[1],96).split(b'\0')[0].decode()==line
            event=('speak',line); pop=6
        elif name=='guards.remove':
            assert receiver==game and machine.reg_read(UC_X86_REG_EDX)==stack+guard_slot and args(1)==[0]; pop=1
        elif name=='guards.close': assert receiver==stack+guard_slot
        elif name=='frame': assert receiver==game
        events.append(event)
        machine.reg_write(UC_X86_REG_EAX,result); machine.reg_write(UC_X86_REG_ESP,esp+4+pop*4)
        machine.reg_write(UC_X86_REG_EIP,get(esp))
    uc.reg_write(UC_X86_REG_ESP,stack); uc.reg_write(UC_X86_REG_EBP,thread)
    entry=(0xdba5a4 if answer==1 else 0xdba4f5) if presented else 0xdb9fa0
    if talk is not None: entry=0xdba8e0
    uc.hook_add(UC_HOOK_CODE,hook); uc.emu_start(entry,0xdbb2e9,count=3000)
    assert len(finished)==1 and not strings and uc.reg_read(UC_X86_REG_ESP)==stack
    if talk is not None: return finished[0],bool(uc.mem_read(thread+0x1c,1)[0]),events
    return finished[0],given,events


def run_lua(answer,pending,bits,busy,cancel,presented=False,talk=None):
    lua=LuaRuntime(); folder=Path(__file__).parent
    source='\n'.join(re.sub(r'\nreturn [\w, ]+\s*$','\n',(folder/name).read_text()) for name in
                     ('theresa_offer_choice.lua','theresa_speech_body.lua','theresa_gift_commit.lua','theresa_offer_body.lua','theresa_presented_gift.lua','theresa_talk_body.lua'))
    name=('acceptPresentedTheresaChocolates' if answer==1 else 'rejectTheresaPresent') if presented else 'offerTheresaChocolates'
    if talk is not None: name='talkToTheresa'
    phase=lua.execute(source+'\nreturn '+name)
    q,r,progress=lua.table(),lua.table(),lua.table(givenChocolates=False)
    events=[]; counts=dict(term=0,task=0,answer=0)
    def query(name):
        counts[name]+=1
        value=(counts[name]==cancel if name=='term' else counts[name]<=busy) if name!='answer' else (-0x80000000 if counts[name]<=pending else answer)
        events.append((name,value)); return value
    def start(*args): events.extend([('movie',),('key',''),('start',),('key.close','')]); return 288
    def question(*args):
        events.extend(('key',key) for key in KEYS); events.append(('question',))
        events.extend(('key.close',key) for key in reversed(KEYS))
    def guards(*args):
        events.append(('guards',)); g=lua.table()
        g.RemoveLivingGuards=lambda *args:events.append(('guards.remove',))
        g.Close=lambda *args:events.append(('guards.close',)); return g
    def health(*args): events.append(('health',)); return struct.unpack('<f',struct.pack('<I',bits))[0]
    r.StartMovie=start; r.ShowTheresaChocolateQuestion=question; r.NewTheresaGuardVector=guards
    r.Pause=lambda self,v:events.append(('pause',int(v)))
    r.DestroyMovie=lambda *args:events.append(('movie.close',))
    r.NewThingFromResource=lambda *args:events.append(('thing',)) or 1
    r.ThingHealth=health; r.DestroyThing=lambda *args:events.append(('thing.close',))
    r.SpeakTheresa=lambda self,control,line:events.extend([('hero',),('speak',line)])
    r.IsPerformingScriptTask=lambda *args:query('task')
    q.IsActiveThreadTerminating=lambda *args:query('term')
    q.MsgIsQuestionAnsweredYesOrNo=lambda *args:query('answer')
    q.NewScriptFrame=lambda *args:events.append(('frame',))
    q.SetStateBool=lambda *args:events.append(('state',))
    r.TakeTheresaChocolatesAndUpdateObjective=lambda *args:events.append(('gift',))
    r.ClearTheresaInformation=lambda *args:events.append(('clear',))
    if talk is not None:
        progress.givenChocolates=talk[0]; state=lua.table(askedForPresent=talk[1])
        return phase(q,17,r,24,progress,state),state.askedForPresent,events
    result=phase(q,17,r,24,progress)
    return result,progress.givenChocolates,events


class TheresaOfferBodyTests(unittest.TestCase):
    def test_talk_uses_main_gift_flag_and_persistent_request_flag(self):
        data=RData(); verify(data)
        for given,asked,bits,busy,cancel in itertools.product((False,True),(False,True),(0,0x3f800000,0x7fc12345),(0,1,3),range(1,8)):
            case=(0,0,bits,busy,cancel,False,(given,asked))
            with self.subTest(case=case): self.assertEqual(run_lua(*case),run_native(data,*case))

    def test_presented_gift_response_without_question(self):
        data=RData(); verify(data)
        for answer,bits,busy,cancel in itertools.product((0,1),(0,0x3f800000,0x7fc12345),(0,1,3),range(1,7)):
            case=(answer,0,bits,busy,cancel,True)
            with self.subTest(case=case): self.assertEqual(run_lua(*case),run_native(data,*case))

    def test_original_offer_composes_question_speech_and_cleanup(self):
        data=RData(); verify(data)
        for case in itertools.product((0,1,2),(0,2),(0,0x3f800000,0x7fc12345),(0,2),range(1,10)):
            with self.subTest(case=case): self.assertEqual(run_lua(*case),run_native(data,*case))

    def test_lua_failures_close_nested_owners_and_preserve_primary_error(self):
        # Engine exceptions are not emulated by the original-byte comparison.
        # These cases check the explicit Lua error policy, separately.
        for failure in ('remove','health','speak','gift','clear','guards.close','unpause','movie.close'):
            with self.subTest(failure=failure):
                lua=LuaRuntime(); folder=Path(__file__).parent
                source='\n'.join(re.sub(r'\nreturn \w+\s*$','\n',(folder/name).read_text()) for name in
                                 ('theresa_offer_choice.lua','theresa_speech_body.lua','theresa_gift_commit.lua','theresa_offer_body.lua'))
                check=lua.execute(source+'''
return function(failure)
    local events, progress = {}, {givenChocolates=false}
    local function event(name)
        events[#events+1]=name
        if name==failure then error("primary:"..name,0) end
        -- Secondary cleanup failure must not replace an earlier body failure.
        if name=="movie.close" then error("secondary",0) end
    end
    local q = {
        IsActiveThreadTerminating=function() return false end,
        MsgIsQuestionAnsweredYesOrNo=function() return 1 end,
        SetStateBool=function() event("state") end,
    }
    local r = {
        StartMovie=function() return 288 end,
        Pause=function(_,on) if not on then event("unpause") end end,
        DestroyMovie=function() event("movie.close") end,
        ShowTheresaChocolateQuestion=function() end,
        NewTheresaGuardVector=function() return {
            RemoveLivingGuards=function() event("remove") end,
            Close=function() event("guards.close") end,
        } end,
        NewThingFromResource=function() event("thing"); return 1 end,
        ThingHealth=function() event("health"); return 1 end,
        DestroyThing=function() event("thing.close") end,
        SpeakTheresa=function() event("speak") end,
        IsPerformingScriptTask=function() return false end,
        TakeTheresaChocolatesAndUpdateObjective=function() event("gift") end,
        ClearTheresaInformation=function() event("clear") end,
    }
    local ok,err=pcall(offerTheresaChocolates,q,17,r,24,progress)
    assert(not ok and err=="primary:"..failure, tostring(err))
    assert(events[#events-2]=="guards.close")
    assert(events[#events-1]=="unpause" and events[#events]=="movie.close")
    local thing,closed
    for i,name in ipairs(events) do
        if name=="thing" then thing=i end
        if name=="thing.close" then assert(not closed); closed=i end
    end
    assert(not thing or (closed and closed < #events-2))
    assert(progress.givenChocolates == (failure=="clear" or failure=="guards.close" or failure=="unpause" or failure=="movie.close"))
end
''')
                check(failure)
