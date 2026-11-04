"""Concrete engine teardown with actual pool traversal and six definition owners."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from pe_oracle import EXE,pe_sections
BASE=0x24000000
def collect():
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000);u.mem_map(BASE,0x10000);raw=EXE.read_bytes()
 for _,va,_,off,n in pe_sections(raw):
  if n:u.mem_write(0x400000+va,raw[off:off+n])
 inputs=bytearray();expected=bytearray();cases=snapshots=0
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def event(kind,self,a=0):
  nonlocal snapshots
  expected.extend(struct.pack('<3I',kind,self,a));expected.extend(u.mem_read(BASE,4096));snapshots+=1
 def hook(u,a,n,_):
  if a not in (0xb25d80,0xbfea14,0x99a300,0x99a410,0xbfe9bc,BASE+0x9100,BASE+0x9200):return
  sp=u.reg_read(UC_X86_REG_ESP);self=u.reg_read(UC_X86_REG_ECX);cleanup=0
  if a==0xb25d80:
   assert self==BASE;event(1,self)
   if mode&1:put(BASE+0x168,0);put(BASE+0x15c,BASE+0xe00)
  elif a==0xbfea14:
   self=get(sp+4);assert BASE+0xa00<=self<BASE+0xf00;event(2,self);put(self,get(self)^0x12345678)
   if mode&2:put(BASE+0x14c,0)
  elif a==BASE+0x9100:
   assert BASE+0x500<=self<BASE+0x600 and get(sp+4)==1;event(3,self,1);cleanup=4
   if mode&1:put(self+0x14,0)
  elif a==0x99a300:
   assert self==BASE+0x130;event(4,self);put(self,0x129a7c4)
   if mode&1:put(BASE+0x118,0)
  elif a==BASE+0x9200:
   assert BASE+0x800<=self<BASE+0x900;event(5,self)
   if mode&2:put(BASE+0x118,0)
  elif a==0x99a410:
   assert self==BASE;event(6,self);put(BASE,0x129a7cc);put(BASE+0xf00,0x87654321)
  else:
   self=get(sp+4);assert self==BASE;event(7,self);put(BASE+0xf04,0x76543210)
  u.reg_write(UC_X86_REG_EAX,self);u.reg_write(UC_X86_REG_ESP,sp+4+cleanup);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook);put(BASE+0x8000,BASE+0x9100);put(BASE+0xa004,BASE+0x9200)
 for fill,state,refs,mode,op in itertools.product((0,1,0x55,0xff),range(8),range(5),range(4),range(7)):
  arena=bytearray([fill])*4096
  def field(off,value):struct.pack_into('<I',arena,off,value)
  for off,data in [(0x168,0xa00),(0x15c,0xb00),(0x14c,0xc00),(0x3c,0xd00)]:
   start=BASE+data if state&1 else 0;field(off,start);field(off+4,start+56 if start else 0);field(off+8,start+112 if start else 0)
  for i,off in enumerate((0x118,0x11c,0x120,0x124,0x128,0x12c)):
   data=BASE+0x800+(0 if refs==4 else i*32);field(off,data if state&2 else 0)
   field(0x800+i*32,BASE+0xa000);field(0x804+i*32,(0,1,2,0xffffffff,6)[refs])
  count=refs%3+1;field(0x134,BASE+0x500 if state&4 else 0)
  for i in range(3):
   off=0x500+i*32;field(off,BASE+0x8000);arena[off+12]=0 if refs==3 else 1;field(off+20,BASE+off+32 if i+1<count else 0)
  if op>=5:field(0,0x12a0f3c)
  inputs.extend(struct.pack('<2I',mode,op));inputs.extend(arena);u.mem_write(BASE,bytes(arena));sp=BASE+0xe000;stop=BASE+0xf000;put(sp,stop);flags=(0,0,1,2,0xffffffff,0,0)[op];put(sp+4,flags);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,0 if op==5 else BASE)
  u.emu_start(0xb261d0 if op==0 else 0xb23990 if op>=5 else 0xb2ab10,stop,count=10000);assert u.reg_read(UC_X86_REG_ESP)==sp+(4 if op==0 or op>=5 else 8);event(8,0 if op==0 or op>=5 else u.reg_read(UC_X86_REG_EAX));cases+=1
 return inputs,expected,dict(cases=cases,snapshots=snapshots)
