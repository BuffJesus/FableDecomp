"""Shader manager and pixel-shader construction, full object plus header allocation."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from pe_oracle import EXE,pe_sections
BASE=0x24000000;WIDTH=0x1000
def collect():
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000);u.mem_map(BASE,0x10000);raw=EXE.read_bytes()
 for _,va,_,off,n in pe_sections(raw):
  if n:u.mem_write(0x400000+va,raw[off:off+n])
 inputs=bytearray();expected=bytearray();cases=snapshots=0
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def event(kind,a=0,b=0,c=0):
  nonlocal snapshots
  expected.extend(struct.pack('<4I',kind,a,b,c));expected.extend(u.mem_read(BASE,WIDTH));snapshots+=1
 def hook(u,a,n,_):
  if a not in [0x99a2f0,0x98d490,0xa5c570,0xbfea0e]:return
  sp=u.reg_read(UC_X86_REG_ESP);this=u.reg_read(UC_X86_REG_ECX);cleanup=0;result=this
  if a==0x99a2f0:
   assert this==BASE;event(1,this);put(this,0x129a7c4)
   if mode==1:put(BASE+4,0x11223344);put(BASE+0x38c,0x55667788)
  elif a==0x98d490:
   assert this==BASE+0x18;event(2,this);u.mem_write(this,b'\xa5'*0x14c)
   if mode==2:put(BASE+0x3ac,0x12345678);put(BASE+0x1e4,0x87654321)
  elif a==0xa5c570:
   assert this==BASE+0x178 and get(sp+4)==0 and get(sp+8)==0x1000;event(3,this,0,0x1000);cleanup=8;u.mem_write(this,b'\x55'*28)
   if mode==3:put(BASE+0x3c0,0x44556677);put(BASE+0x194,0x33445566);put(BASE+0x3e0,0x22334455)
  else:
   assert get(sp+4)==24;event(4,24);result=BASE+0x800
   if mode==4:put(BASE+0x3c0,0x77889900);put(BASE+0x3c4,0x11223344);put(BASE+0x3e4,0x33445566)
   elif mode==5:put(BASE+0x198,0x44556677);put(BASE+0x1d8,0x12344321)
  u.reg_write(UC_X86_REG_EAX,result);u.reg_write(UC_X86_REG_ESP,sp+4+cleanup);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook)
 for op,fill,mode in itertools.product([0,1],range(256),range(6)):
  if op==1 and mode:continue
  arena=bytes([fill])*WIDTH;inputs.extend(struct.pack('<3I',op,fill,mode));inputs.extend(arena);u.mem_write(BASE,arena)
  sp=BASE+0xe000;stop=BASE+0xf000;put(sp,stop);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,BASE if op==0 else BASE+0x400);u.emu_start(0x98ab00 if op==0 else 0xa5ec00,stop,count=100000);assert u.reg_read(UC_X86_REG_ESP)==sp+4
  event(5,op,u.reg_read(UC_X86_REG_EAX));cases+=1
 return inputs,expected,dict(cases=cases,snapshots=snapshots)
