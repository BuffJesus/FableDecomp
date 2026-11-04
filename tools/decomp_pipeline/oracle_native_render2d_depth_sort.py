"""Retail depth callbacks under every x87 rounding and precision mode."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32
from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_ESP,UC_X86_REG_FPCW,UC_X86_REG_FPSW
from pe_oracle import EXE,pe_sections
VALUES=[0,0x80000000,0x3f800000,0xbf800000,0x3f800001,0x3f7fffff,0x7f800000,0xff800000,
        0x7fc00000,0xffc00001,0x7f800001,0x00000001,0x80000001,0x00800000,0x80800000,
        0x7f7fffff,0xff7fffff,0x4b800001,0xcb800000,0x3eaaaaab]
BASE=0x24000000
def collect():
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000);u.mem_map(BASE,0x10000);raw=EXE.read_bytes()
 for _,va,_,off,size in pe_sections(raw):
  if size:u.mem_write(0x400000+va,raw[off:off+size])
 inputs=bytearray();expected=bytearray();cases=0
 for left,right,rounding,precision in itertools.product(range(20),range(20),range(4),range(3)):
  data=bytearray((i*13+cases*7)&255 for i in range(192))
  for side,index in [(0,left),(96,right)]:
   struct.pack_into('<I',data,side+0x24,VALUES[index])
   for vertex,shift in enumerate((0,7,13)):
    struct.pack_into('<I',data,side+vertex*32+8,VALUES[(index+shift)%20])
  control=(0,0x10000,0x20000)[precision]|(rounding<<8)
  inputs.extend(struct.pack('<I',control));inputs.extend(data);u.mem_write(BASE,bytes(data))
  results=[]
  for address in (0x9d8c30,0x9d8cb0):
   u.reg_write(UC_X86_REG_FPCW,0x7f|(0x300,0x200,0)[precision]|(rounding<<10));u.reg_write(UC_X86_REG_FPSW,0)
   u.mem_write(BASE+0xe000,struct.pack('<3I',BASE+0xf000,BASE,BASE+96));u.reg_write(UC_X86_REG_ESP,BASE+0xe000)
   u.emu_start(address,BASE+0xf000,count=1000);assert u.reg_read(UC_X86_REG_ESP)==BASE+0xe004;results.append(u.reg_read(UC_X86_REG_EAX))
  expected.extend(struct.pack('<2I',*results));expected.extend(u.mem_read(BASE,192));cases+=1
 return inputs,expected,dict(cases=cases,comparisons=cases*2)
