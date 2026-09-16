"""Original Init state writes and ABI versus readable Init; engine methods doubled."""
import hashlib
import itertools
import unittest
from pathlib import Path
from lupa.lua54 import LuaRuntime
from tools.script_recovery.test_native_affair_wife_hit_scopes import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP, UC_X86_REG_EIP, UC_X86_REG_ECX
from tools.script_recovery.lift_native_lua import RData

class BarrelThugInitTests(unittest.TestCase):
    def test_original_state_and_actor_order(self):
        data=RData();body=data.bytes_at(0xdb6bf0,65)
        self.assertEqual(hashlib.sha256(body).hexdigest(),'d2dc4a56b4f8265bafa812550a582da8e8b2b0e989bee90c0e8bf32f2b45d8ba')
        for done,last in itertools.product((0,1),(0,9999,0xffffffff)):
            uc=Uc(UC_ARCH_X86,UC_MODE_32)
            for a in (0xdb6000,0x100000,0x200000):uc.mem_map(a,0x1000)
            uc.mem_write(0xdb6bf0,body)
            def put(a,v):uc.mem_write(a,v.to_bytes(4,'little'))
            def get(a):return int.from_bytes(uc.mem_read(a,4),'little')
            thread,game,table,stack=0x200000,0x200100,0x200200,0x100800
            put(thread+4,game);put(game,table);put(thread+0x20,last);uc.mem_write(thread+0x1c,bytes([done]));put(stack,0x200ff0)
            calls={0x200c00:('damage',2),0x200c10:('kill',3),0x200c20:('combo',2)}
            for slot,a in zip((0x810,0x814,0x838),calls):put(table+slot,a)
            for a in (*calls,0x200ff0):uc.mem_write(a,b'\xc3')
            events=[]
            def hook(machine,a,n,user):
                if a==0x200ff0:machine.emu_stop();return
                if a==0xdb6bf9:events.append(('done',False))
                if a==0xdb6bfd:events.append(('last',9999))
                if a not in calls:return
                name,count=calls[a];esp=machine.reg_read(UC_X86_REG_ESP)
                assert machine.reg_read(UC_X86_REG_ECX)==game
                args=[get(esp+4+i*4) for i in range(count)]
                assert args==[thread+8]+[0]*(count-1)
                assert get(thread+0x20)==9999 and bytes(uc.mem_read(thread+0x1c,1))==b'\x00'
                events.append((name,));machine.reg_write(UC_X86_REG_ESP,esp+4+4*count);machine.reg_write(UC_X86_REG_EIP,get(esp))
            uc.reg_write(UC_X86_REG_ESP,stack);uc.reg_write(UC_X86_REG_ECX,thread);uc.hook_add(UC_HOOK_CODE,hook)
            uc.emu_start(0xdb6bf0,0xdb6c31,count=100)
            self.assertEqual(uc.reg_read(UC_X86_REG_ESP),stack+4)
            lua=LuaRuntime();phase=lua.execute(Path(__file__).with_name('barrel_thug_init.lua').read_text())
            actual=[];s,r=lua.table(),lua.table()
            s.SetStateBool=lambda self,key,value:actual.append(('done',value))
            s.SetStateInt=lambda self,key,value:actual.append(('last',value))
            r.InitializeBarrelThugActor=lambda *args:actual.extend([('damage',),('kill',),('combo',)])
            phase(lua.table(),17,r,s)
            self.assertEqual(actual,events)
