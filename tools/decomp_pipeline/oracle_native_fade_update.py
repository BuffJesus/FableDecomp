"""Full fade object replay across timer limits and x87 special values."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32
from unicorn.x86_const import *
from pe_oracle import EXE,pe_sections
BASE=0x24000000
def collect():
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000);u.mem_map(BASE,0x10000);raw=EXE.read_bytes()
 for _,va,_,off,n in pe_sections(raw):
  if n:u.mem_write(0x400000+va,raw[off:off+n])
 inputs=bytearray();expected=bytearray();cases=0
 values=[0,0x80000000,0x3f800000,0xbf800000,0x3f000000,0x7f800000,0xff800000,0x7fc12345,0x7f812345,1,0x80000001]
 for timer,duration,elapsed,flags,control in itertools.product(values,values,values,range(8),[0x27f,0x37f]):
  arena=bytearray([cases&255])*28;arena[0]=flags&1;arena[12]=(flags>>1)&1;arena[13]=(flags>>2)&1
  for off,v in [(4,duration),(16,timer),(20,timer)]:struct.pack_into('<I',arena,off,v)
  inputs.extend(struct.pack('<2I',elapsed,control));inputs.extend(arena);u.mem_write(BASE,bytes(arena))
  sp=BASE+0xe000;stop=BASE+0xf000;u.mem_write(sp,struct.pack('<2I',stop,elapsed));u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,BASE);u.reg_write(UC_X86_REG_FPCW,control);u.reg_write(UC_X86_REG_FPTAG,0xffff);u.reg_write(UC_X86_REG_FPSW,0)
  u.emu_start(0x434870,stop,count=1000);assert u.reg_read(UC_X86_REG_ESP)==sp+8;expected.extend(u.mem_read(BASE,28));cases+=1
 return inputs,expected,dict(cases=cases,snapshots=cases)
