"""Execute retail base construction and retain the full object snapshot."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EAX,UC_X86_REG_EIP
from pe_oracle import EXE,pe_sections
ALIGNMENTS=[0,1,2,3,4,5,8,16,32,0xffffffff]

def collect():
    raw=EXE.read_bytes();uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1200000)
    for _,va,vs,offset,size in pe_sections(raw):
        if size:uc.mem_write(0x400000+va,raw[offset:offset+size])
    uc.mem_map(0x2000000,0x20000);owner=0x2004000;stack=0x2010000;stop=0x200f000;rows=[]
    for alignment in ALIGNMENTS:
        for pattern in [0x5a,0xa5]:
            uc.mem_write(owner-4,b'\x17'*4+bytes([pattern])*0xc1c+b'\x31'*4);uc.mem_write(stack,struct.pack('<II',stop,alignment));uc.reg_write(UC_X86_REG_ECX,owner);uc.reg_write(UC_X86_REG_ESP,stack);uc.emu_start(0xa65ad0,stop,count=10000)
            assert uc.reg_read(UC_X86_REG_EIP)==stop and uc.reg_read(UC_X86_REG_ESP)==stack+8 and uc.reg_read(UC_X86_REG_EAX)==owner
            assert bytes(uc.mem_read(owner-4,4))==b'\x17'*4 and bytes(uc.mem_read(owner+0xc1c,4))==b'\x31'*4
            rows.append(dict(alignment=alignment,pattern=pattern,bytes=bytes(uc.mem_read(owner,0xc1c)).hex()))
    return rows
