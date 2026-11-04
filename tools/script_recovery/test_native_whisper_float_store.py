import struct
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import Lifter,load_manifest,RData
from tools.script_recovery.native_evidence_lowering import finish_lua
from tools.script_recovery.build_readable_unit import readable_file

@pytest.mark.parametrize("readable",[False,True])
@pytest.mark.parametrize("a,b",[(0,0),(3,1),(-3,1),(1,-2**-24),(16777216,-1),(-1,2**-24),(1,2**-25),(1.0000001192092896,0)])
def test_whisper_height_native_float_store(readable,a,b):
    import unicorn as uc
    from unicorn import x86_const as x
    raw=RData().bytes_at(0xd527ca,0x18)
    assert raw.hex()=="d94308d86008680000e0408d9424840000008bcdd95c2428"
    vm=uc.Uc(uc.UC_ARCH_X86,uc.UC_MODE_32)
    for p in (0xd52000,0x200000,0x300000):vm.mem_map(p,4096)
    vm.mem_write(0xd527ca,raw)
    vm.mem_write(0x200008,struct.pack("<f",a));vm.mem_write(0x200108,struct.pack("<f",b))
    vm.reg_write(x.UC_X86_REG_EBX,0x200000);vm.reg_write(x.UC_X86_REG_EAX,0x200100)
    vm.reg_write(x.UC_X86_REG_ESP,0x300800);vm.reg_write(x.UC_X86_REG_FPCW,0x37f)
    vm.emu_start(0xd527ca,0xd527e2,count=10)
    assert vm.reg_read(x.UC_X86_REG_EIP)==0xd527e2
    native=struct.unpack("<f",vm.mem_read(0x300824,4))[0]
    lifter=Lifter(load_manifest(),{},"quest",False,"",RData());lifter.accessor_kinds=True
    body=finish_lua("\n".join(lifter.lift("Height","{\nf_stk_94 = puVar8.z - pCVar6.z;\n}",parameters={"puVar8":"vector","pCVar6":"vector"})))
    source="function Height(quest,puVar8,pCVar6)\n"+body+"\nreturn f_stk_94, math.abs(f_stk_94)>1.0\nend"
    if readable:source=readable_file(source)[0]
    lua=LuaRuntime(unpack_returned_tuples=True);lua.execute(source)
    value,branch=lua.globals().Height(lua.table(),lua.table(z=a),lua.table(z=b))
    assert value==native and branch==(abs(native)>1.0)
