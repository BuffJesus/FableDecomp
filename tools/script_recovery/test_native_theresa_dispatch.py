"""One recurring dispatch iteration; message/phase interiors are explicit boundaries."""
import itertools
import unittest
from pathlib import Path
from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_theresa_control import verify
from tools.script_recovery.test_native_affair_wife_hit_scopes import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP, UC_X86_REG_EIP, UC_X86_REG_ECX, UC_X86_REG_EAX, UC_X86_REG_EBP, UC_X86_REG_EBX, UC_X86_REG_EDX


def native(data,offer,gift,talk,hit,given,busy,near,cancel):
    uc=Uc(UC_ARCH_X86,UC_MODE_32)
    for a,n in ((0xdb9000,0x3000),(0x99e000,0x1000),(0xf35000,0x1000),(0x7e7000,0x1000),
                (0xcbe000,0x1000),(0x100000,0x10000),(0x200000,0x4000)):
        uc.mem_map(a,n)
    uc.mem_write(0xdb97a0,data.bytes_at(0xdb97a0,7013))
    def put(a,v):uc.mem_write(a,v.to_bytes(4,'little'))
    def get(a):return int.from_bytes(uc.mem_read(a,4),'little')
    stack,thread,game,table=0x108000,0x200000,0x201000,0x202000
    put(thread+4,game);put(game,table);put(table+0x118,0x203000)
    uc.mem_write(stack+23,bytes([given]))
    calls={0xf35b30:'term',0x99e4b0:'presented',0x99eae0:'presented.close',0x7e7450:'task',0x203000:'hero',0xcbe2ff:'distance'}
    for a in calls:uc.mem_write(a,b'\xc3')
    events=[];terms=0;finished=[]
    def hook(machine,a,n,user):
        nonlocal terms
        if a in (0xdba40f,0xdbb2e8,0xdbb0e4):
            finished.append({0xdba40f:'next',0xdbb2e8:'cancel',0xdbb0e4:'outro'}[a]);machine.emu_stop();return
        # Boundaries have independent phase/compiled adapter coverage.
        jumps={0xdb9ee0:('offer-query',0xdb9f94,offer),
               0xdba8ab:('talk-query',0xdba8d8,talk),0xdbabb3:('hit-query',0xdbac95,hit)}
        if a in jumps:
            name,target,value=jumps[a];events.append(name);uc.mem_write(stack+22,bytes([value]));machine.reg_write(UC_X86_REG_EAX,int(value));machine.reg_write(UC_X86_REG_EIP,target);return
        if a==0xdba490:
            events.append('classify');machine.reg_write(UC_X86_REG_EIP,{'chocolates':0xdba5a4,'other':0xdba4f5,'none':0xdba8ab}[gift]);return
        phases={0xdb9fa0:'offer',0xdba5a4:'accept',0xdba4f5:'reject',0xdba8e0:'talk',0xdbaca1:'hit'}
        if a in phases:
            name=phases[a];events.append(name)
            if name=='accept':uc.mem_write(stack+23,b'\x01')
            machine.reg_write(UC_X86_REG_EIP,0xdba3e3 if name=='accept' else 0xdba3db);return
        if a==0xdbae1e:events.append('skip');machine.reg_write(UC_X86_REG_EIP,0xdbae51);return
        if a not in calls:return
        name=calls[a];esp=machine.reg_read(UC_X86_REG_ESP);receiver=machine.reg_read(UC_X86_REG_ECX)
        pop,result=0,0xabcd0000
        if name=='term':assert receiver==thread;terms+=1;result|=int(terms==cancel)
        elif name in ('presented','presented.close'):assert receiver==stack+16
        elif name=='task':assert receiver==stack+24;result|=int(busy)
        elif name=='hero':assert receiver==game;result=0
        elif name=='distance':
            assert receiver==0 and machine.reg_read(UC_X86_REG_EDX)==stack+44 and get(esp+4)==0x40000000
            result|=int(near);pop=1
        events.append(name);machine.reg_write(UC_X86_REG_EAX,result)
        machine.reg_write(UC_X86_REG_ESP,esp+4+pop*4);machine.reg_write(UC_X86_REG_EIP,get(esp))
    uc.reg_write(UC_X86_REG_ESP,stack);uc.reg_write(UC_X86_REG_EBP,thread);uc.reg_write(UC_X86_REG_EBX,thread+8);uc.reg_write(UC_X86_REG_ECX,thread)
    uc.hook_add(UC_HOOK_CODE,hook);uc.emu_start(0xdb9eca,0xdbb305,count=2000)
    assert len(finished)==1 and uc.reg_read(UC_X86_REG_ESP)==stack
    return finished[0],events


def readable(offer,gift,talk,hit,given,busy,near,cancel):
    lua=LuaRuntime();events=[];active=False;done=False;terms=0;result='cancel'
    def log(name):
        if active:events.append(name)
    g=lua.globals()
    g.waitForHeroToApproachTheresa=lambda *args:True
    def meeting(q,me,r,c,state,progress):
        nonlocal done
        done=True;progress.givenChocolates=given;return True
    g.meetTheresa=meeting
    for name,event in (('offerTheresaChocolates','offer'),('rejectTheresaPresent','reject'),('talkToTheresa','talk'),('handleTheresaHit','hit')):
        g[name]=lambda *args,event=event:log(event) or True
    def accept(q,me,r,c,progress):log('accept');progress.givenChocolates=True;return True
    g.acceptPresentedTheresaChocolates=accept
    g.classifyTheresaPresentedItem=lambda *args:log('classify') or ('other-present' if gift=='other' else gift)
    def boundary(name):
        nonlocal result
        result=name;raise RuntimeError('BOUNDARY')
    g.finishTheresaChildhood=lambda *args:boundary('outro')
    q,r,state=lua.table(),lua.table(),lua.table()
    def state_get(self,key):
        nonlocal active
        if key=='DoneIntro' and done:active=True
        return done
    state.GetStateBool=state_get
    def term(*args):
        nonlocal terms
        if not active:return False
        log('term');terms+=1;return terms==cancel
    q.IsActiveThreadTerminating=term
    q.NewScriptFrame=lambda *args:boundary('next') if active else None
    r.NewResource=lambda *args:24;r.NewTheresaDepartureTrigger=lambda *args:44
    r.PrepareResource=lambda *args:None;r.TryAcquire=lambda *args:True
    r.NewPresentedItemOutput=lambda *args:log('presented') or 16
    r.DestroyPresentedItemOutput=lambda *args:log('presented.close')
    r.DestroyThing=lambda *args:None;r.ReleaseResource=lambda *args:None
    r.TheresaTalkOffersChocolates=lambda *args:log('offer-query') or offer
    r.WasVillagerTalkedTo=lambda *args:log('talk-query') or talk
    r.IsHitByHeroExceptAbility=lambda *args:log('hit-query') or hit
    r.IsPerformingScriptTask=lambda *args:log('task') or busy
    r.PlayTheresaSkip=lambda *args:log('skip')
    r.IsTheresaHeroNearTrigger=lambda *args:events.extend(['hero','distance']) or near
    phase=lua.execute(Path(__file__).with_name('theresa_main_body.lua').read_text())
    try:phase(q,17,r,state)
    except RuntimeError as error:
        assert str(error)=='BOUNDARY'
        if result=='outro':assert events.pop()=='presented.close' # synthetic unwind after phase boundary
    return result,events


class TheresaDispatchTests(unittest.TestCase):
    def test_native_dispatch_precedence_and_idle_departure(self):
        data=RData();verify(data)
        for case in itertools.product((False,True),('none','other','chocolates'),(False,True),(False,True),(False,True),(False,True),(False,True),(0,1,2)):
            with self.subTest(case=case):self.assertEqual(readable(*case),native(data,*case))
