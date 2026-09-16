"""Original entry/dispatcher/predicate/outer cleanup with separately tested phases abstracted."""
import hashlib
import itertools
import unittest
from pathlib import Path
from lupa.lua54 import LuaRuntime
from tools.script_recovery.test_native_affair_wife_hit_scopes import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP, UC_X86_REG_EIP, UC_X86_REG_ECX, UC_X86_REG_EAX, UC_X86_REG_ESI, UC_X86_REG_EDI, UC_X86_REG_EBP
from tools.script_recovery.lift_native_lua import RData


def native(data,done,talk,ordinary,any_ability,excluded,cancel,failed_phase):
    uc=Uc(UC_ARCH_X86,UC_MODE_32)
    for a,n in ((0xdb6000,0x2000),(0xf35000,0x1000),(0x99a000,0x1000),(0x99e000,0x1000),(0x7e7000,0x1000),(0x100000,0x10000),(0x200000,0x5000)):
        uc.mem_map(a,n)
    uc.mem_write(0xdb6c60,data.bytes_at(0xdb6c60,0x1091))
    def put(a,v):uc.mem_write(a,(v&0xffffffff).to_bytes(4,'little'))
    def get(a):return int.from_bytes(uc.mem_read(a,4),'little')
    stack,thread,game,table,actor,actor_table=0x108000,0x200000,0x201000,0x202000,0x200008,0x204000
    put(thread+4,game);put(game,table);put(actor,actor_table);uc.mem_write(thread+0x1c,bytes([done]))
    put(stack+0xf8,0x203ff0);put(stack+0x24,0)
    calls={0xf35b30:'term',0x99a380:'new',0x99a430:'base.close',0x7e74d0:'close',0x99ebf0:'key.new',0x99eae0:'key.close',0x203000:'frame'}
    put(table+0x1c,0x203000)
    for i,(offset,name) in enumerate(((0x6c,'talk'),(0x54,'ordinary'),(0xa8,'any'),(0xa4,'excluded'))):
        a=0x203010+i*16;put(actor_table+offset,a);calls[a]=name
    for a in (*calls,0x203ff0):uc.mem_write(a,b'\xc3')
    phases={0xdb6cfb:('intro',0xdb6f4e),0xdb6f86:('conversation',0xdb73a0),0xdb73a0:('remarks',0xdb7924),0xdb79fc:('hit',0xdb7bb0)}
    events=[];counts=dict(term=0,frame=0);keys=[];ended=[]
    def hook(machine,a,n,user):
        if a==0x203ff0:ended.append(True);machine.emu_stop();return
        # Phase boundaries preserve only their caller-visible success/cancel contract.
        if a in phases:
            name,join=phases[a];events.append(('phase',name))
            if name=='intro' and failed_phase!=name:uc.mem_write(thread+0x1c,b'\x01')
            machine.reg_write(UC_X86_REG_EIP,0xdb7cdd if failed_phase==name else join);return
        if a==0xdb6cf0:events.append(('done',bool(uc.mem_read(thread+0x1c,1)[0])))
        if a not in calls:return
        name=calls[a];esp=machine.reg_read(UC_X86_REG_ESP);receiver=machine.reg_read(UC_X86_REG_ECX)
        args=lambda n:[get(esp+4+i*4) for i in range(n)]
        pop=0;result=0xabcd0000;event=(name,)
        if name=='frame':assert receiver==game;counts['frame']+=1
        elif name=='term':
            assert receiver==thread;counts['term']+=1
            value=counts['term']==cancel or counts['frame']>=2;event=('term',value);result|=int(value)
        elif name=='new':assert receiver==stack+16
        elif name=='close':assert receiver==stack+16
        elif name=='base.close':
            assert receiver==stack+16 and get(stack+16)==0x126008c and get(stack+24)==0 and get(stack+28)==0
            event=('close',)
        elif name=='key.new':
            assert args(2)==[0x125d1c8,0xffffffff];keys.append(receiver);pop=2;event=('key.new',)
        elif name=='key.close':assert keys.pop()==receiver
        else:
            assert receiver==actor
            if name=='excluded':assert args(2)==[14,keys[-1]];pop=2
            else:assert args(1)==[keys[-1]];pop=1
            value={'talk':talk,'ordinary':ordinary,'any':any_ability,'excluded':excluded}[name]
            result|=int(value);event=(name,value)
        events.append(event);machine.reg_write(UC_X86_REG_EAX,result)
        machine.reg_write(UC_X86_REG_ESP,esp+4+4*pop);machine.reg_write(UC_X86_REG_EIP,get(esp))
    uc.reg_write(UC_X86_REG_ESP,stack);uc.reg_write(UC_X86_REG_ESI,thread);uc.reg_write(UC_X86_REG_EDI,actor);uc.reg_write(UC_X86_REG_EBP,0)
    uc.hook_add(UC_HOOK_CODE,hook);uc.emu_start(0xdb6cb0,0xdb7cf1,count=2000)
    assert ended==[True] and not keys and uc.reg_read(UC_X86_REG_ESP)==stack+0xfc
    return events


def readable(done,talk,ordinary,any_ability,excluded,cancel,failed_phase):
    lua=LuaRuntime();events=[];counts=dict(term=0,frame=0);state=[done]
    def phase(name):
        def run(*args):
            events.append(('phase',name))
            if name=='intro' and failed_phase!=name:state[0]=True
            return name!=failed_phase
        return run
    for name,fn in (('intro','introduceBarrelThug'),('conversation','runBarrelThugConversation'),('remarks','updateBarrelThugTimedRemarks'),('hit','handleBarrelThugHit')):lua.globals()[fn]=phase(name)
    main=lua.execute(Path(__file__).with_name('barrel_thug_main_body.lua').read_text())
    q,r,s=lua.table(),lua.table(),lua.table()
    def frame(*args):counts['frame']+=1;events.append(('frame',))
    def term(*args):
        counts['term']+=1;v=counts['term']==cancel or counts['frame']>=2;events.append(('term',v));return v
    def done_query(self,key):assert key=='DoneIntro';events.append(('done',state[0]));return state[0]
    def talk_query(*args):events.extend([('key.new',),('talk',talk),('key.close',)]);return talk
    def hit_query(self,actor,ability):
        assert actor==17 and ability==14
        events.extend([('key.new',),('ordinary',ordinary)]);live=1;v=ordinary
        if not ordinary:
            live+=1;events.extend([('key.new',),('any',any_ability)]);v=False
            if any_ability:
                live+=1;events.extend([('key.new',),('excluded',excluded)]);v=not excluded
        events.extend([('key.close',)]*live);return v
    q.NewScriptFrame=frame;q.IsActiveThreadTerminating=term;s.GetStateBool=done_query
    r.NewResource=lambda *args:events.append(('new',)) or 16;r.ReleaseResource=lambda *args:events.append(('close',))
    r.WasVillagerTalkedTo=talk_query;r.IsHitByHeroExceptAbility=hit_query
    main(q,17,r,s);return events


class BarrelThugDispatchTests(unittest.TestCase):
    def test_original_entry_predicates_phase_order_and_outer_cleanup(self):
        data=RData()
        self.assertEqual(hashlib.sha256(data.bytes_at(0xdb6c60,0x1091)).hexdigest(),'eafde7cb9a35b3f6d7c158c455496af2ea89444352b04e4f7b39dcbde39ada09')
        self.assertEqual(data.string_at(0x125d1c8),'SCRIPT_NAME_HERO')
        for case in itertools.product((False,True),(False,True),(False,True),(False,True),(False,True),(0,1,2),('','intro','conversation','remarks','hit')):
            with self.subTest(case=case):self.assertEqual(readable(*case),native(data,*case))
