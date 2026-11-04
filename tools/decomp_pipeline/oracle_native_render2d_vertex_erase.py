"""Full-range and overlapping partial erasure of 32-byte renderer vertices."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_EAX,UC_X86_REG_ESP
from pe_oracle import EXE,pe_sections
BASE=0x24000000;WIDTH=0x1000
def collect():
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000);u.mem_map(BASE,0x10000);raw=EXE.read_bytes()
 for _,va,_,off,size in pe_sections(raw):
  if size:u.mem_write(0x400000+va,raw[off:off+size])
 inputs=bytearray();expected=bytearray();cases=0
 for count in range(17):
  for first in range(count+1):
   for last in range(first,count+1):
    arena=bytearray((i*17+cases*13)&255 for i in range(WIDTH))
    struct.pack_into('<3I',arena,0,BASE+0x100,BASE+0x100+count*32,BASE+0x100+16*32)
    inputs.extend(struct.pack('<2I',first,last));inputs.extend(arena);u.mem_write(BASE,bytes(arena))
    stack=BASE+0xe000;stop=BASE+0xf000;u.mem_write(stack,struct.pack('<3I',stop,BASE+0x100+first*32,BASE+0x100+last*32));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,BASE)
    u.emu_start(0x9e1440,stop,count=10000);assert u.reg_read(UC_X86_REG_ESP)==stack+12
    expected.extend(struct.pack('<I',u.reg_read(UC_X86_REG_EAX)));expected.extend(u.mem_read(BASE,WIDTH));cases+=1
 return inputs,expected,dict(cases=cases,snapshots=cases)
