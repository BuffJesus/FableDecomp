"""Retail unsigned maximum of cached chunk free sizes, with untouched globals."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EAX,UC_X86_REG_EIP
from pe_oracle import EXE,pe_sections
BASE=0x2004000;WIDTH=0x8000
def collect():
    raw=EXE.read_bytes();uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1200000)
    for _,va,vs,offset,size in pe_sections(raw):
        if size:uc.mem_write(0x400000+va,raw[offset:offset+size])
    uc.mem_map(0x2000000,0x20000);stack=0x2010000;stop=0x200f000;rows=[]
    for mode,address in enumerate([0xa64500]):
     for alignment,shift in [(4,2),(8,3),(16,4),(32,5),(64,6),(128,7)]:
      for length in [0,1,3,7]:
       for counts in [[0,1,7,32],[0xffffffff,1,0x80000000,0x7fffffff],[0x7fffffff]*4]:
        for pattern in [0x5a,0xa5]:
         initial=bytearray(bytes([pattern])*WIDTH);struct.pack_into('<3I',initial,0xc14,alignment,shift,0x2009000 if length else 0)
         expected=0
         for i in range(length):
            a=0x2009000+i*0x80;value=counts[i%len(counts)];expected=max(expected,value)
            struct.pack_into('<2I',initial,a-BASE+4,17+i,7+i);struct.pack_into('<I',initial,a-BASE+0x10,value)
            struct.pack_into('<2I',initial,a-BASE+0x20,a+0x80 if i+1<length else 0,a-0x80 if i else 0)
         uc.mem_write(BASE,bytes(initial));uc.mem_write(0x13d28d4,struct.pack('<2I',256,8))
         uc.mem_write(stack,struct.pack('<I',stop));uc.reg_write(UC_X86_REG_ECX,BASE);uc.reg_write(UC_X86_REG_ESP,stack)
         uc.emu_start(address,stop,count=2000)
         result=uc.reg_read(UC_X86_REG_EAX);globals_=struct.unpack('<2I',bytes(uc.mem_read(0x13d28d4,8)))
         assert uc.reg_read(UC_X86_REG_EIP)==stop and uc.reg_read(UC_X86_REG_ESP)==stack+4
         assert result==expected and globals_==(256,8)
         assert bytes(uc.mem_read(BASE,WIDTH))==initial
         rows.append(dict(args=(mode,result,*globals_),initial=bytes(initial)))
    return rows
