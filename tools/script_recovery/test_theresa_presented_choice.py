"""Execute both original presented polls and actual native string comparisons."""
import itertools
import unittest
from pathlib import Path
from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_theresa_control import verify
from tools.script_recovery.test_native_affair_wife_hit_scopes import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP, UC_X86_REG_EIP, UC_X86_REG_ECX, UC_X86_REG_EAX, UC_X86_REG_EBX

CHOCOLATES=b'OBJECT_CHOCOLATE_BOX_UNGIVEABLE'


def native(data, polls):
    uc=Uc(UC_ARCH_X86,UC_MODE_32)
    for address,size in ((0xdb9000,0x3000),(0x99e000,0x1000),(0x411000,0x1000),
                         (0x122d000,0x1000),(0x12d8000,0x1000),(0x100000,0x10000),(0x200000,0x6000)):
        uc.mem_map(address,size)
    for address,size in ((0xdb97a0,7013),(0x99e960,65),(0x411570,160),(0x122d70e,64),(0x12d8f24,64)):
        uc.mem_write(address,data.bytes_at(address,size))
    def put(a,v):uc.mem_write(a,v.to_bytes(4,'little'))
    def get(a):return int.from_bytes(uc.mem_read(a,4),'little')
    stack,actor,table,target=0x108000,0x200000,0x201000,0x202000
    put(actor,table);put(table+0x8c,target);uc.mem_write(target,b'\xc3')
    events,finished=[],[]
    def hook(machine,address,size,user):
        exits={0xdba5a4:'chocolates',0xdba4f5:'other-present',0xdba8ab:'none'}
        if address in exits:finished.append(exits[address]);machine.emu_stop();return
        if address!=target:return
        esp=machine.reg_read(UC_X86_REG_ESP)
        assert machine.reg_read(UC_X86_REG_ECX)==actor and get(esp+4)==stack+16
        active,text=polls[len(events)];events.append((active,text))
        if text is None:put(stack+16,0)
        else:
            put(stack+16,0x204000);put(0x204000,0x204100)
            uc.mem_write(0x204100,text+b'\0')
        machine.reg_write(UC_X86_REG_EAX,0xabcd0000|int(active))
        machine.reg_write(UC_X86_REG_ESP,esp+8);machine.reg_write(UC_X86_REG_EIP,get(esp))
    uc.reg_write(UC_X86_REG_ESP,stack);uc.reg_write(UC_X86_REG_EBX,actor)
    uc.hook_add(UC_HOOK_CODE,hook);uc.emu_start(0xdba490,0xdba8ac,count=3000)
    assert len(finished)==1 and uc.reg_read(UC_X86_REG_ESP)==stack
    return finished[0],events


def lua_phase(polls):
    lua=LuaRuntime();phase=lua.execute(Path(__file__).with_name('theresa_presented_choice.lua').read_text())
    events=[];text=None;r=lua.table()
    def poll(self,output):
        nonlocal text
        assert output==16
        active,text=polls[len(events)];events.append((active,text));return active
    def matches(self,output,key):
        assert output==16 and key.encode()==CHOCOLATES
        return (text or b'')==CHOCOLATES
    r.PollPresentedItem=poll;r.PresentedItemMatches=matches
    return phase(r,16),events


class TheresaPresentedChoiceTests(unittest.TestCase):
    def test_same_output_repolled_with_native_string_comparisons(self):
        data=RData();verify(data)
        values=(None,b'',CHOCOLATES,CHOCOLATES.lower(),b'OBJECT_TEDDY_BEAR',CHOCOLATES+b'X',b'\xff')
        variants=tuple(itertools.product((False,True),values))
        for polls in itertools.product(variants,repeat=2):
            with self.subTest(polls=polls):self.assertEqual(lua_phase(polls),native(data,polls))
