import struct,unittest
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from tools.script_recovery.bully_animation import prove
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.bully_full_resource_candidate import generate

def native_storage(value):
    data=RData();prove(data);u=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in (0x903000,0x99a000,0x99e000):u.mem_map(page,4096)
    u.mem_map(0x200000,0x10000);u.mem_write(0x9034f0,data.bytes_at(0x9034f0,117))
    u.mem_write(0x99a2f0,b'\xc3');u.mem_write(0x99ec30,b'\xc3')
    stack=0x20f000;action=0x201000;done=0x202000
    args=(0x203000,0,0,0xffffffff,0,9,0,1,value,0,0)
    u.mem_write(stack,struct.pack('<12I',done,*args));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,action)
    def hook(uc,pc,size,user):
        if pc not in (0x99a2f0,0x99ec30):return
        sp=uc.reg_read(UC_X86_REG_ESP);ret=struct.unpack('<I',uc.mem_read(sp,4))[0]
        uc.reg_write(UC_X86_REG_ESP,sp+4+(4 if pc==0x99ec30 else 0));uc.reg_write(UC_X86_REG_EIP,ret)
    u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0x9034f0,done,count=1000)
    assert u.reg_read(UC_X86_REG_EIP)==done
    return bytes(u.mem_read(action+29,1))[0]

class BullyAnimationTests(unittest.TestCase):
    def test_original_constructor_preserves_noncanonical_fifth_flag(self):
        for value in (0,1,2,127,128,255):self.assertEqual(value,native_storage(value))

    def test_changed_forwarder_or_raw_byte_storage_rejected(self):
        class Changed(RData):
            def bytes_at(self,address,size):
                raw=super().bytes_at(address,size)
                if address<=self.changed<address+size:
                    raw=bytearray(raw);raw[self.changed-address]^=1;return bytes(raw)
                return raw
        for pc in (0x7e73d9,0x904aa3,0x903536,0x903548):
            data=Changed();data.changed=pc
            with self.assertRaises(ValueError):prove(data)

    def test_composed_calls_supply_seven_native_flags(self):
        source,report=generate()
        self.assertEqual(source.count('resources:PlayAnimationWithNativeArgument5(control,'),1)
        self.assertNotIn('ReadAnimationArgument5()',source)
        self.assertIn('false, false, false, true, false, false)',source)
        self.assertEqual(report['animation']['rawByteStorage']['actionOffset'],29)

if __name__=='__main__':unittest.main()
