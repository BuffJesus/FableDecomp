"""Full retail fixed allocator snapshots, including untouched base arrays."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EAX,UC_X86_REG_EIP
from pe_oracle import EXE,pe_sections
CASES=[(chunk,area,alignment,flags,internal) for alignment in [4,8,16]
       for chunk,area,flags,internal in [(4096,1,0,0),(32,0,1,2),(0,255,0xffffffff,0x80000000),(65536,96,0x12345678,7),(1,0xffffffe0,3,0xffffffff)]]

def collect():
    raw=EXE.read_bytes();uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1200000)
    for _,va,vs,offset,size in pe_sections(raw):
        if size:uc.mem_write(0x400000+va,raw[offset:offset+size])
    uc.mem_map(0x2000000,0x20000);owner=0x2004000;stack=0x2010000;stop=0x200f000;rows=[]
    for args in CASES:
        for pattern in [0x5a,0xa5]:
            uc.mem_write(owner-4,b'\x17'*4+bytes([pattern])*0xc4c+b'\x31'*4)
            uc.mem_write(stack,struct.pack('<7I',stop,0x12345678,*args));uc.reg_write(UC_X86_REG_ECX,owner);uc.reg_write(UC_X86_REG_ESP,stack);uc.emu_start(0xa65ff0,stop,count=10000)
            assert uc.reg_read(UC_X86_REG_EIP)==stop and uc.reg_read(UC_X86_REG_ESP)==stack+28 and uc.reg_read(UC_X86_REG_EAX)==owner
            assert bytes(uc.mem_read(owner-4,4))==b'\x17'*4 and bytes(uc.mem_read(owner+0xc4c,4))==b'\x31'*4
            before=bytes(uc.mem_read(owner,0xc4c))
            uc.mem_write(owner,struct.pack('<I',0xdeadbeef));uc.mem_write(stack,struct.pack('<I',stop));uc.reg_write(UC_X86_REG_ECX,owner);uc.reg_write(UC_X86_REG_ESP,stack);uc.emu_start(0xa65d20,stop,count=100)
            after=bytes(uc.mem_read(owner,0xc4c));assert after==before and uc.reg_read(UC_X86_REG_ESP)==stack+4
            rows.append(dict(args=args,pattern=pattern,bytes=before.hex(),destructor_bytes=after.hex()))
    return rows
