import struct
import unittest
from types import SimpleNamespace
from lupa.lua54 import LuaRuntime
from tools.script_recovery.test_native_affair_wife_hit_scopes import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_ECX,UC_X86_REG_EAX,UC_X86_REG_EBX
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_barrel_man_health_branches import verify,recover
from tools.script_recovery.generate_barrel_man_resource_candidate import DRAFT


def native(data,region,bits):
    u=Uc(UC_ARCH_X86,UC_MODE_32)
    for address,size in ((region['address'] & ~0xFFF,0x2000),(0x4AA000,0x1000),(0x122D000,0x1000),
                         (0x100000,0x10000),(0x200000,0x1000)):
        u.mem_map(address,size)
    u.mem_write(region['address'],data.bytes_at(region['address'],region['size']))
    u.mem_write(0x200100,struct.pack('<I',bits));u.mem_write(0x122DEDC,b'\0'*4)
    prefix=b'\xd9\x05'+struct.pack('<I',0x200100)+b'\xe9'+struct.pack('<I',(region['address']-(0x200000+11))&0xFFFFFFFF)
    u.mem_write(0x200000,prefix)
    stack=0x108000;u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_EBX,0xA5A5A5A5)
    events=[]
    def hook(machine,address,size,user):
        if address!=0x4AA840:return
        assert machine.reg_read(UC_X86_REG_ECX)==stack+region['output']
        events.append('destroy');esp=machine.reg_read(UC_X86_REG_ESP)
        destination=int.from_bytes(machine.mem_read(esp,4),'little')
        machine.reg_write(UC_X86_REG_EAX,0xDEADBEEF)
        machine.reg_write(UC_X86_REG_ESP,esp+4);machine.reg_write(UC_X86_REG_EIP,destination)
    u.hook_add(UC_HOOK_CODE,hook)
    end=region['address']+region['size'];u.emu_start(0x200000,end,count=100)
    assert events==['destroy'] and u.reg_read(UC_X86_REG_ESP)==stack and u.reg_read(UC_X86_REG_EIP)==end
    return bool(u.reg_read(UC_X86_REG_EAX if region['result']=='al' else UC_X86_REG_EBX)&255)


class BarrelHealthBranchTests(unittest.TestCase):
    def test_seven_native_results_and_inverse_lua_including_unordered(self):
        data=RData();witness=verify(data)
        lua=LuaRuntime();positive=lua.eval('function(health) return health > 0.0 end')
        inverse=lua.eval('function(health) return not (health > 0.0) end')
        for region in witness['branches']:
            for bits in (0,0x80000000,0x3F800000,0xBF800000,1,0x80000001,0x7F800000,0xFF800000,0x7FC12345):
                value=struct.unpack('<f',struct.pack('<I',bits))[0]
                expected=native(data,region,bits)
                self.assertEqual(positive(value),expected)
                self.assertEqual(inverse(value),not expected)

    def test_mutated_branch_threshold_and_source_reject(self):
        data=RData();source=DRAFT.read_text();out,_=recover(source,data)
        self.assertIn('if not (fVar19 > fVar20) then',out)
        with self.assertRaisesRegex(ValueError,'source correspondence'):
            recover(source.replace('if fVar19 <= fVar20 then','if fVar19 < fVar20 then'),data)
        for site in [0x122DEDC]+[r['address'] for r in verify(data)['branches']]:
            def read(address,size):
                raw=data.bytes_at(address,size)
                if raw is not None and address<=site<address+size:
                    raw=bytearray(raw);raw[site-address]^=1;raw=bytes(raw)
                return raw
            with self.assertRaises(ValueError):verify(SimpleNamespace(bytes_at=read))
