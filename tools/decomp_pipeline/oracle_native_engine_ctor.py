"""Retail CEngine construction, retaining all object bytes and callback mutations."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from pe_oracle import EXE,pe_sections
BASE=0x24000000
def collect(composed=False):
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000);u.mem_map(BASE,0x10000);raw=EXE.read_bytes()
 for _,va,_,off,n in pe_sections(raw):
  if n:u.mem_write(0x400000+va,raw[off:off+n])
 inputs=bytearray();expected=bytearray();cases=snapshots=0
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def event(kind,self,a=0,b=0):
  nonlocal snapshots
  expected.extend(struct.pack('<4I',kind,self,a,b));expected.extend(u.mem_read(BASE,4096));snapshots+=1
 def hook(u,a,n,_):
  if a not in (0x99a310,0xa5c570,0xbfea1a):return
  sp=u.reg_read(UC_X86_REG_ESP);self=u.reg_read(UC_X86_REG_ECX);cleanup=0
  if a==0xbfea1a:
   assert get(sp+4)==0x178;event(4,0,0x178);self=BASE if present else 0
  elif a==0x99a310:
   assert self==BASE;event(1,self);put(self,0x129a7cc);u.mem_write(self+4,b'\0')
   if mode&1:u.mem_write(BASE+8,bytes([0xa5])*0x170)
   if mode&2:put(BASE+0x500,0x87654321)
  else:
   assert self==BASE+0x130;assert(get(sp+4),get(sp+8))==(0,0x1000);event(2,self,get(sp+4),get(sp+8));cleanup=8
   for i in range(7):put(self+i*4,0x12340000+i)
   if mode&1:put(BASE+0x14,0xdeadbeef);put(BASE+0xc0,0x87654321)
   if mode&2:put(BASE+0x14c,0xabcdef01);put(BASE+0x84,0xabcdef01)
   if mode&4:put(BASE+0xc,0x65432100);put(BASE+0x600,0x10293847)
  u.reg_write(UC_X86_REG_EAX,self);u.reg_write(UC_X86_REG_ESP,sp+4+cleanup);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook)
 for fill,mode,present in itertools.product(range(256),range(8),range(2)if composed else (1,)):
  arena=bytes([fill])*4096;inputs.extend(struct.pack('<2I',mode,present));inputs.extend(arena);u.mem_write(BASE,arena);sp=BASE+0xe000;stop=BASE+0xf000;put(sp,stop);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,BASE)
  u.emu_start(0xb26340 if composed else 0xb260b0,stop,count=1000);assert u.reg_read(UC_X86_REG_ESP)==sp+4;event(3,u.reg_read(UC_X86_REG_EAX));cases+=1
 return inputs,expected,dict(cases=cases,snapshots=snapshots)
