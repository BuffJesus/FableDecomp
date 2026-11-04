"""Execute the actual setter/getter pair and check independent singleton expectations."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32
from unicorn.x86_const import *
GLOBAL=0x13b8a1c; MANAGER=0x2000000; STACK=0x3000000; STOP=0x4000000

def run(code,manager,definition,receiver,previous):
    u=Uc(UC_ARCH_X86,UC_MODE_32)
    for a,n in [(0x4c7000,0x1000),(GLOBAL&~0xfff,0x1000),(MANAGER,0x4000),(STACK,0x10000),(STOP,0x1000)]:u.mem_map(a,n)
    def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
    def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
    u.mem_write(GLOBAL-8,b'\xa5'*20);put(GLOBAL,previous)
    u.mem_write(MANAGER,b'\x5a'*0x4000);put(manager+0x20,definition)
    before=bytes(u.mem_read(MANAGER,0x4000))
    for address,b in code.items():u.mem_write(address,bytes(b))
    saved=(UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP)
    def call(addr,this):
        sp=STACK+0x8000;put(sp,STOP);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,this)
        for i,r in enumerate(saved):u.reg_write(r,0x123400+i)
        u.emu_start(addr,STOP,count=100)
        assert u.reg_read(UC_X86_REG_ESP)==sp+4
        assert all(u.reg_read(r)==0x123400+i for i,r in enumerate(saved))
        return u.reg_read(UC_X86_REG_EAX)
    call(0x4c74f0,manager);assert get(GLOBAL)==manager
    result=call(0x4c7990,receiver);assert result==definition
    put(manager+0x20,definition^0x10)
    rebound=call(0x4c7990,receiver);assert rebound==definition^0x10
    put(manager+0x20,definition);assert bytes(u.mem_read(MANAGER,0x4000))==before
    assert bytes(u.mem_read(GLOBAL-8,8))==b'\xa5'*8 and bytes(u.mem_read(GLOBAL+4,8))==b'\xa5'*8
    return get(GLOBAL),result,rebound

def compare(candidate,native):
    cases=0
    for manager in (MANAGER,MANAGER+0x40,MANAGER+0x103):
        for definition in (0,MANAGER+0x2000,0x12345678,0xffffffff):
            for receiver in (0,MANAGER+0x3000,0xffffffff):
                for previous in (0,MANAGER,0xffffffff):
                    assert run(candidate,manager,definition,receiver,previous)==run(native,manager,definition,receiver,previous)
                    cases+=1
    return cases
