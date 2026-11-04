"""Retail null internal allocation: aligned wrapper writes low memory after unlock."""
import struct,itertools
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_MEM_WRITE_UNMAPPED,UcError
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_EAX
from pe_oracle import EXE,pe_sections
def collect():
    raw=EXE.read_bytes();rows=[]
    for size,alignment,present in itertools.product([0,1,236,0xffffffff],[1,2,4,8,16,256,4096],[0,1]):
        uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1200000)
        for _,va,vs,offset,count in pe_sections(raw):
            if count:uc.mem_write(0x400000+va,raw[offset:offset+count])
        uc.mem_map(0x2000000,0x20000);events=[];faults=[];depth=0
        def word(a,v):uc.mem_write(a,struct.pack('<I',v&0xffffffff))
        def read(a):return struct.unpack('<I',uc.mem_read(a,4))[0]
        def hook(uc,a,n,user):
            nonlocal depth
            sp=uc.reg_read(UC_X86_REG_ESP);cleanup=0
            if a==0x9f2870:events.append((4,0));word(0x13caa90,0x2004000);cleanup=4
            elif a==0x200d000:events.append((1,depth));depth+=1;cleanup=8
            elif a==0x200d100:events.append((2,depth));depth-=1;cleanup=8
            elif a==0x9f2a40:
                assert read(sp+4)==(size+alignment+3)&0xffffffff and read(sp+8)==0 and read(sp+12)==0
                events.append((5,depth));uc.reg_write(UC_X86_REG_EAX,0);cleanup=16
            if cleanup:uc.reg_write(UC_X86_REG_EIP,read(sp));uc.reg_write(UC_X86_REG_ESP,sp+cleanup)
        def fault(uc,access,address,count,value,user):faults.append((address,count,value));return False
        uc.hook_add(UC_HOOK_CODE,hook);uc.hook_add(UC_HOOK_MEM_WRITE_UNMAPPED,fault)
        word(0x143fe0c,0x200d000);word(0x143fe10,0x200d100);word(0x13caa90,0x2004000 if present else 0);word(0x2010000,0x200f000)
        uc.reg_write(UC_X86_REG_ESP,0x2010000);uc.reg_write(UC_X86_REG_ECX,size);uc.reg_write(UC_X86_REG_EDX,alignment)
        try:uc.emu_start(0x9f2be0,0x200f000,count=1000)
        except UcError:pass
        address=(((alignment+3)&~(alignment-1))-4)&0xffffffff
        assert faults==[(address,4,0)] and depth==0 and events==([]if present else[(4,0)])+[(1,0),(5,1),(2,1)]
        rows.append((size,alignment,present,address))
    return rows
