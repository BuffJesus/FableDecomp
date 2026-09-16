"""Native Main entry/acquisition until first state dispatch, or real cancellation.

Condition registration precedes this entry. Trigger lookup is a compiled-adapter
boundary. Active paths stop at dispatch; their synthetic Lua unwind is excluded.
"""
import itertools
import unittest
from pathlib import Path
from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_theresa_control import verify
from tools.script_recovery.test_native_affair_wife_hit_scopes import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP, UC_X86_REG_EIP, UC_X86_REG_ECX, UC_X86_REG_EAX, UC_X86_REG_EBP


def native(data,pending,has,cancel):
    uc=Uc(UC_ARCH_X86,UC_MODE_32)
    for a,n in ((0xdb9000,0x3000),(0x99a000,0x1000),(0xcd2000,0x1000),(0xf35000,0x1000),
                (0x4aa000,0x1000),(0x7e7000,0x1000),(0x100000,0x10000),(0x200000,0x4000)):
        uc.mem_map(a,n)
    uc.mem_write(0xdb97a0,data.bytes_at(0xdb97a0,7013))
    def put(a,v):uc.mem_write(a,v.to_bytes(4,'little'))
    def get(a):return int.from_bytes(uc.mem_read(a,4),'little')
    stack,thread,game,table=0x108000,0x200000,0x201000,0x202000
    put(thread+4,game);put(game,table);put(table+0x1c,0x203000);put(table+0x20,0x203010)
    put(stack+0x200,0x203f00)
    uc.mem_write(0x203f00,b'\xc3')
    calls={0x203000:'frame',0x203010:'acquire',0xf35b30:'term',0x99a380:'control',
           0xcd23b9:'prepare',0xcd2770:'reset',0x99a2e0:'trigger.close',0x99a430:'control.close',
           0x4aa840:'trigger.close',0x7e74d0:'control.close'}
    for a in calls:uc.mem_write(a,b'\xc3')
    events=[];counts=dict(term=0,acquire=0);finished=[]
    def hook(machine,a,n,user):
        if a in (0xdb98ca,0x203f00):finished.append(a==0xdb98ca);machine.emu_stop();return
        if a==0xdb9810:
            events.append('trigger');machine.reg_write(UC_X86_REG_EIP,0xdb9853);return
        if a not in calls:return
        name=calls[a];esp=machine.reg_read(UC_X86_REG_ESP);receiver=machine.reg_read(UC_X86_REG_ECX)
        pop,result=0,0xabcd0000
        if name=='term':assert receiver==thread;counts[name]+=1;result|=int(counts[name]==cancel)
        elif name=='acquire':
            assert receiver==game and [get(esp+4+i*4) for i in range(3)]==[thread+8,stack+24,4]
            counts[name]+=1;result|=int(counts[name]>pending);pop=3
        elif name=='frame':assert receiver==game
        elif name=='prepare':assert receiver==stack+24;result|=int(has)
        elif name=='trigger.close':assert receiver==stack+44
        else:assert receiver==stack+24
        events.append(name);machine.reg_write(UC_X86_REG_EAX,result)
        machine.reg_write(UC_X86_REG_ESP,esp+4+pop*4);machine.reg_write(UC_X86_REG_EIP,get(esp))
    uc.reg_write(UC_X86_REG_ESP,stack);uc.reg_write(UC_X86_REG_EBP,thread)
    uc.hook_add(UC_HOOK_CODE,hook);uc.emu_start(0xdb97f0,0x203f01,count=2000)
    assert len(finished)==1
    return finished[0],events


def readable(pending,has,cancel):
    lua=LuaRuntime();phase=lua.execute(Path(__file__).with_name('theresa_main_body.lua').read_text())
    events=[];counts=dict(term=0,acquire=0);q,r,state=lua.table(),lua.table(),lua.table()
    def term(*args):events.append('term');counts['term']+=1;return counts['term']==cancel
    def acquire(self,c,me,priority):
        assert (c,me,priority)==(24,17,4)
        events.append('acquire');counts['acquire']+=1;return counts['acquire']>pending
    def boundary(*args):raise RuntimeError('DISPATCH_BOUNDARY')
    state.GetStateBool=boundary
    q.NewScriptFrame=lambda *args:events.append('frame');q.IsActiveThreadTerminating=term
    r.NewResource=lambda *args:events.append('control') or 24
    r.NewTheresaDepartureTrigger=lambda *args:events.append('trigger') or 44
    r.PrepareResource=lambda *args:events.extend(['prepare','reset'] if has else ['prepare'])
    r.TryAcquire=acquire
    r.DestroyThing=lambda *args:events.append('trigger.close')
    r.ReleaseResource=lambda *args:events.append('control.close')
    try:phase(q,17,r,state)
    except RuntimeError as error:
        assert str(error)=='DISPATCH_BOUNDARY' and events[-2:]==['trigger.close','control.close']
        return True,events[:-2]
    return False,events


class TheresaAcquisitionTests(unittest.TestCase):
    def test_original_entry_retry_and_cancellation(self):
        data=RData();verify(data)
        for case in itertools.product((0,1,3),(False,True),range(1,8)):
            with self.subTest(case=case):self.assertEqual(readable(*case),native(data,*case))
