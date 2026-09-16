import itertools
import unittest
from types import SimpleNamespace
from tools.script_recovery.test_native_affair_wife_hit_scopes import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_ECX,UC_X86_REG_EDI,UC_X86_REG_EBX,UC_X86_REG_EAX,UC_X86_REG_ESI
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_barrel_man_interactions import verify,recover
from tools.script_recovery.generate_barrel_man_resource_candidate import DRAFT

def native_trace(data,block,direct,special,excluded):
    machine=Uc(UC_ARCH_X86,UC_MODE_32)
    for address,size in [(0xDB5000,0x2000),(0x99E000,0x1000),(0x100000,0x10000),
                         (0x200000,0x1000),(0x210000,0x1000),(0x220000,0x1000)]:
        machine.mem_map(address,size)
    machine.mem_write(block['address'],data.bytes_at(block['address'],block['size']))
    def put(address,value):machine.mem_write(address,int(value).to_bytes(4,'little'))
    def get(address):return int.from_bytes(machine.mem_read(address,4),'little')
    put(0x200000,0x210000)
    targets={0x220010:('direct',direct,4),0x220020:('special',special,4),0x220030:('excluded',excluded,8)}
    for slot,target in [(0x54,0x220010),(0xA8,0x220020),(0xA4,0x220030)]:put(0x210000+slot,target)
    machine.reg_write(UC_X86_REG_ESP,0x108000)
    machine.reg_write(UC_X86_REG_EDI,0x200000)
    machine.reg_write(UC_X86_REG_EBX,0)
    live=[];identities={};events=[]
    def hook(uc,address,size,user):
        stack=uc.reg_read(UC_X86_REG_ESP);receiver=uc.reg_read(UC_X86_REG_ECX)
        if address==0x99EBF0:
            assert get(stack+4)==0x125D1C8 and get(stack+8)==0xFFFFFFFF
            assert receiver not in live
            identities[receiver]=len(identities)+1;live.append(receiver)
            events.append(('construct',identities[receiver]));pop=8;result=receiver
        elif address==0x99EAE0:
            assert live and live[-1]==receiver
            live.pop();events.append(('destroy',identities[receiver]));pop=0;result=0
        elif address in targets:
            name,result,pop=targets[address]
            assert receiver==0x200000
            if name=='excluded':assert get(stack+4)==14
            string=get(stack+(8 if name=='excluded' else 4))
            assert string in live
            events.append(('query',name));result=int(result)
        else:return
        destination=get(stack)
        uc.reg_write(UC_X86_REG_EAX,result)
        uc.reg_write(UC_X86_REG_ESP,stack+4+pop)
        uc.reg_write(UC_X86_REG_EIP,destination)
    machine.hook_add(UC_HOOK_CODE,hook)
    machine.emu_start(block['address'],block['address']+block['size'],count=1000)
    assert machine.reg_read(UC_X86_REG_EIP)==block['address']+block['size']
    assert not live
    assert machine.reg_read(UC_X86_REG_ESP)==0x108000
    return bool(machine.reg_read(UC_X86_REG_EAX)&255),events


def interaction_trace(data, block, phase, distance_result, talk_result):
    uc=Uc(UC_ARCH_X86,UC_MODE_32)
    for address,size in ((0xDB6000,0x1000),(0x99E000,0x1000),(0xCBE000,0x1000),
                         (0x13AC000,0x1000),(0x100000,0x10000),(0x200000,0x3000)):
        uc.mem_map(address,size)
    uc.mem_write(block['address'],data.bytes_at(block['address'],block['size']))
    def put(address,value):uc.mem_write(address,int(value).to_bytes(4,'little'))
    def get(address):return int.from_bytes(uc.mem_read(address,4),'little')
    stack,actor,thread,game,table=0x108000,0x200000,0x200100,0x200200,0x201000
    hero_getter,talk=0x202000,0x202010
    put(actor,table);put(thread+4,game);put(thread+0x20,phase);put(game,table)
    put(table+0x118,hero_getter);put(table+0x6C,talk)
    # A distinctive payload proves the load precedes GetHero and is not a fixed float.
    threshold=0x40D12345;put(0x13AC858,threshold)
    for register,value in ((UC_X86_REG_ESP,stack),(UC_X86_REG_EDI,actor),(UC_X86_REG_ESI,thread),(UC_X86_REG_EBX,0)):
        uc.reg_write(register,value)
    events=[];live=[]
    def hook(machine,address,size,user):
        esp=machine.reg_read(UC_X86_REG_ESP);receiver=machine.reg_read(UC_X86_REG_ECX)
        if address==hero_getter:
            assert receiver==game
            put(0x13AC858,0x7FC00000)
            events.append('hero');result=0;pop=0 # Preserve a null borrowed hero.
        elif address==0xCBE2FF:
            from unicorn.x86_const import UC_X86_REG_EDX
            assert receiver==actor and machine.reg_read(UC_X86_REG_EDX)==0 and get(esp+4)==threshold
            events.append('distance');result=distance_result;pop=4
        elif address==0x99EBF0:
            assert get(esp+4)==0x125D1C8 and get(esp+8)==0xFFFFFFFF
            live.append(receiver);events.append('construct');result=receiver;pop=8
        elif address==talk:
            assert receiver==actor and get(esp+4)==live[-1]
            events.append('talk');result=talk_result;pop=4
        elif address==0x99EAE0:
            assert live.pop()==receiver
            events.append('destroy');result=0xDEADBEEF;pop=0
        else:return
        machine.reg_write(UC_X86_REG_EAX,result)
        machine.reg_write(UC_X86_REG_ESP,esp+4+pop)
        machine.reg_write(UC_X86_REG_EIP,get(esp))
    uc.hook_add(UC_HOOK_CODE,hook)
    end=block['address']+block['size'];uc.emu_start(block['address'],end,count=200)
    assert not live and uc.reg_read(UC_X86_REG_EIP)==end and uc.reg_read(UC_X86_REG_ESP)==stack
    return bool(uc.reg_read(UC_X86_REG_EAX)&255),events


class BarrelInteractionTests(unittest.TestCase):
    def test_hit_query_order_and_reverse_cleanup(self):
        data=RData();block=verify(data)['blocks'][0]
        for direct,special,excluded in itertools.product((0,1,2,255),repeat=3):
            result,events=native_trace(data,block,direct,special,excluded)
            queries=['direct']+([] if direct else ['special']+(['excluded'] if special else []))
            expected=[]
            for index,query in enumerate(queries,1):expected.extend([('construct',index),('query',query)])
            expected.extend(('destroy',index) for index in range(len(queries),0,-1))
            self.assertEqual(events,expected)
            self.assertEqual(result,bool(direct or (special and not excluded)))

    def test_approach_short_circuit_and_talk_cleanup_match_emitted_lua(self):
        from lupa.lua54 import LuaRuntime
        data=RData();block=verify(data)['blocks'][1]
        run=LuaRuntime(unpack_returned_tuples=True).execute('''return function(phase, distance, talked)
            local events={}
            local __native_entity_state={GetStateInt=function() return phase end}
            local resources={IsHeroWithinBarrelApproachDistance=function()
                events[#events+1]='hero';events[#events+1]='distance';return distance end}
            local me={IsTalkedToByHero=function()
                events[#events+1]='construct';events[#events+1]='talk';events[#events+1]='destroy';return talked end}
            local bVar3
            '''+block['newLua']+''' return bVar3,table.concat(events,',') end''')
        for phase,distance,talk in itertools.product((0,1,3,5),(0,1,2,255),(0,1,2,255)):
            expected,events=interaction_trace(data,block,phase,distance,talk)
            result,trace=run(phase,distance!=0,talk!=0)
            self.assertEqual((result,trace),(expected,','.join(events)))

    def test_changed_native_and_source_reject(self):
        data=RData();source=DRAFT.read_text();output,_=recover(source,data)
        self.assertNotIn('ppuVar17 = (ppuStack_1c8 | 1)',output)
        with self.assertRaisesRegex(ValueError,'source correspondence'):
            recover(source.replace('me:MsgIsHitByHero()','me:MsgIsHitByHero(1)'),data)
        for site in (0xDB5E18,0xDB5E74,0xDB5E9A,0xDB60C7,0xDB60EF,0xDB6113):
            def read(address,size):
                raw=data.bytes_at(address,size)
                if raw is not None and address<=site<address+size:
                    raw=bytearray(raw);raw[site-address]^=1;raw=bytes(raw)
                return raw
            with self.assertRaisesRegex(ValueError,'native bytes'):
                verify(SimpleNamespace(bytes_at=read,string_at=data.string_at))

