import itertools
import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.test_native_affair_wife_hit_scopes import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_ECX,UC_X86_REG_EDI,UC_X86_REG_EAX,UC_X86_REG_ESI,UC_X86_REG_EDX
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_barrel_overhear import REPLACEMENT


def native(data,phase,heard,random,divisor,close,cancel):
    uc=Uc(UC_ARCH_X86,UC_MODE_32)
    for address,size in ((0xDB6000,0x1000),(0xBFE000,0x1000),(0xCBE000,0x1000),(0xF35000,0x1000),(0x13AC000,0x1000),(0x100000,0x10000),(0x200000,0x3000)):
        uc.mem_map(address,size)
    uc.mem_write(0xDB687C,data.bytes_at(0xDB687C,0xDB68CE-0xDB687C))
    def put(address,value):uc.mem_write(address,(value&0xFFFFFFFF).to_bytes(4,'little'))
    def get(address):return int.from_bytes(uc.mem_read(address,4),'little')
    stack,actor,thread,game,table,hero=0x108000,0x200000,0x200100,0x200200,0x201000,0x200400
    put(thread+4,game);put(thread+0x20,phase);uc.mem_write(thread+0x1E,bytes([heard]));put(game,table);put(table+0x118,0x202000)
    # The rand boundary changes the divisor, proving the load follows the call.
    put(0x13AC854,0)
    for register,value in ((UC_X86_REG_ESP,stack),(UC_X86_REG_EDI,actor),(UC_X86_REG_ESI,thread)):
        uc.reg_write(register,value)
    events=[];out=[]
    def hook(machine,address,size,user):
        if address in (0xDB6933,0xDB6AFD,0xDB68CE):out.append(address);machine.emu_stop();return
        if address not in (0xBFEB16,0x202000,0xCBE2FF,0xF35B30):return
        esp=machine.reg_read(UC_X86_REG_ESP);pop=0
        if address==0xBFEB16:events.append('rand');put(0x13AC854,divisor);result=random
        elif address==0x202000:
            assert machine.reg_read(UC_X86_REG_ECX)==game;events.append('hero');result=hero
        elif address==0xCBE2FF:
            assert machine.reg_read(UC_X86_REG_ECX)==actor and machine.reg_read(UC_X86_REG_EDX)==hero and get(esp+4)==0x41700000
            events.append('distance');result=close;pop=4
        else:
            assert machine.reg_read(UC_X86_REG_ECX)==thread;events.append('term');result=cancel
        machine.reg_write(UC_X86_REG_EAX,int(result)&0xFFFFFFFF)
        machine.reg_write(UC_X86_REG_ESP,esp+4+pop);machine.reg_write(UC_X86_REG_EIP,get(esp))
    uc.hook_add(UC_HOOK_CODE,hook);uc.emu_start(0xDB687C,0xDB7000,count=100)
    assert uc.reg_read(UC_X86_REG_ESP)==stack and len(out)==1
    return events,out[0]


class BarrelOverhearTests(unittest.TestCase):
    def test_native_gate_and_lua_state_write_order(self):
        data=RData()
        run=LuaRuntime(unpack_returned_tuples=True).execute('''return function(source,phase,heard,eligible,cancel)
            local events={};local function event(s)events[#events+1]=s end
            local env={me='actor',__native_entity_state={
                GetStateInt=function(_,key)assert(key=='MyPhase');return phase end,
                GetStateBool=function(_,key)assert(key=='OverheardYet');return heard end,
                SetStateBool=function(_,key,value)assert(key=='OverheardYet' and value);event('state')end},
                quest={IsActiveThreadTerminating=function()event('term');return cancel end},
                resources={ShouldBarrelOverhear=function(_,actor,value)assert(actor=='actor' and value==heard);event('gate');return eligible end,
                AddBarrelConversation=function(_,actor,key)assert(actor=='actor' and key=='TEXT_QST_048_BARRELMAN_OVERHEAR');event('conversation')end}}
            local f=assert(load(source..'\\ndo return "frame" end\\n::LAB_00db6afd:: return "cancel"','overhear','t',env))
            local result=f();return table.concat(events,','),result
        end''')
        for phase,heard,random,divisor,close,cancel in itertools.product((0,1,5),(False,True),(-6,-1,0,6),(-3,2),(False,True),(False,True)):
            eligible=(not heard or random%divisor==0) and close
            expected=[];destination=0xDB6933
            if phase==0:
                if heard:expected.append('rand')
                if not heard or random%divisor==0:
                    expected+=['hero','distance']
                    if close:expected.append('term');destination=0xDB6AFD if cancel else 0xDB68CE
            self.assertEqual(native(data,phase,heard,random,divisor,close,cancel),(expected,destination))
            lua_events=[]
            if phase==0:
                lua_events.append('gate')
                if eligible:
                    lua_events.append('term')
                    if not cancel:lua_events+=['state','conversation']
            self.assertEqual(run(REPLACEMENT,phase,heard,eligible,cancel),(','.join(lua_events),'cancel' if destination==0xDB6AFD else 'frame'))
