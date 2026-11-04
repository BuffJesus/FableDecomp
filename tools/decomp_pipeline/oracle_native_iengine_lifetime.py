"""Engine interface lifecycle, including release through actual scalar deletion."""
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
 def event(kind,self,a=0,b=0):
  nonlocal snapshots
  expected.extend(struct.pack('<4I',kind,self,a,b));expected.extend(u.mem_read(BASE,512));snapshots+=1
 def hook(u,a,n,_):
  if a not in (0x99a350,0x99a410,0x99eae0,0xbfe9bc,BASE+0x9100):return
  sp=u.reg_read(UC_X86_REG_ESP);self=u.reg_read(UC_X86_REG_ECX);cleanup=0;value=0
  if a==0x99a350:
   assert self==BASE;event(1,self);u.mem_write(BASE+4,bytes([parameter]));value=parameter
   if mode&1:put(BASE+0x40,0x87654321)
  elif a==BASE+0x9100:
   assert self==BASE and get(sp+4)==1;event(2,self,1);cleanup=4
   if mode&1:put(BASE+8,0xabcdef01)
   if mode&2:put(BASE,0x12345678)
  elif a==0x99eae0:
   data=get(self);event(3,1,data);put(self,0)
   if data and mode&1:put(data,get(data)^0x12345678)
   if mode&2:put(BASE+0x40,0x98765432)
  elif a==0x99a410:
   assert self==BASE;event(4,self);put(BASE,0x129a7cc)
   if mode&1:put(BASE+8,0x87654321)
   if mode&2:put(BASE+0x40,0x76543210)
  else:
   self=get(sp+4);assert self==BASE;event(5,self)
   if mode&1:put(BASE+0x44,0x10293847)
  u.reg_write(UC_X86_REG_EAX,value);u.reg_write(UC_X86_REG_ESP,sp+4+cleanup);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook);put(BASE+0x8000,BASE+0x9100)
 for fill,op,parameter,mode in itertools.product(range(256),range(4),range(4),range(8)):
  arena=bytearray([fill])*512;struct.pack_into('<I',arena,0,0x12a0d24 if op==1 and mode&4 else BASE+0x8000)
  inputs.extend(struct.pack('<3I',op,parameter,mode));inputs.extend(arena);u.mem_write(BASE,bytes(arena));sp=BASE+0xe000;stop=BASE+0xf000
  arg=[BASE+0x100,0,(0,BASE+0x100,BASE+0x180,BASE+0x1f0)[parameter],(0,1,2,0xffffffff)[parameter]][op]
  put(sp,stop);put(sp+4,arg);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,0 if op==1 and parameter==0 else BASE)
  u.emu_start((0xb232d0,0xb232e0,0xb23610,0xb23770)[op],stop,count=1000);assert u.reg_read(UC_X86_REG_ESP)==sp+(4 if op==1 else 8)
  value=u.reg_read(UC_X86_REG_EAX)&255 if op==0 else u.reg_read(UC_X86_REG_EAX) if op==3 else 0;event(6,op,value);cases+=1
 return inputs,expected,dict(cases=cases,snapshots=snapshots)
