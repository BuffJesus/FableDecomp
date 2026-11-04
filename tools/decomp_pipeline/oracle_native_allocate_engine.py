"""Retail engine allocation with observable allocation and construction boundaries."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from pe_oracle import EXE,pe_sections
BASE=0x24000000
def collect():
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000);u.mem_map(BASE,0x10000)
 raw=EXE.read_bytes()
 for _,va,_,off,n in pe_sections(raw):
  if n:u.mem_write(0x400000+va,raw[off:off+n])
 inputs=bytearray();expected=bytearray();cases=snapshots=0
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def put(a,v):u.mem_write(a,struct.pack('<I',v))
 def event(kind,value):
  nonlocal snapshots
  expected.extend(struct.pack('<2I',kind,value));expected.extend(u.mem_read(BASE,1024));snapshots+=1
 def hook(u,a,n,_):
  if a not in (0xbfea1a,0xb260b0):return
  sp=u.reg_read(UC_X86_REG_ESP)
  if a==0xbfea1a:
   size=get(sp+4);assert size==0x178;event(1,size);value=BASE+offset if present else 0
  else:
   self=u.reg_read(UC_X86_REG_ECX);assert self==BASE+offset;event(2,self)
   if mode==1:u.mem_write(self,bytes((i*17+fill)&255 for i in range(0x178)))
   elif mode==2:u.mem_write(BASE,bytes((i*3+fill)&255 for i in range(1024)))
   value=self
  u.reg_write(UC_X86_REG_EAX,value);u.reg_write(UC_X86_REG_ESP,sp+4);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook)
 for fill,present,offset,mode in itertools.product(range(256),range(2),(32,512),range(3)):
  arena=bytes([fill])*1024;inputs.extend(struct.pack('<4I',fill,present,offset,mode));inputs.extend(arena);u.mem_write(BASE,arena)
  sp=BASE+0xe000;stop=BASE+0xf000;put(sp,stop);u.reg_write(UC_X86_REG_ESP,sp);u.emu_start(0xb26340,stop,count=100)
  assert u.reg_read(UC_X86_REG_ESP)==sp+4;event(3,u.reg_read(UC_X86_REG_EAX));cases+=1
 return inputs,expected,dict(cases=cases,snapshots=snapshots)
