"""Retail display-engine construction with mutating base/camera boundaries."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from pe_oracle import EXE,pe_sections
BASE=0x24000000;WIDTH=4096
def collect():
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000);u.mem_map(BASE,0x10000);raw=EXE.read_bytes()
 for _,va,_,off,n in pe_sections(raw):
  if n:u.mem_write(0x400000+va,raw[off:off+n])
 inputs=bytearray();expected=bytearray();cases=snapshots=0
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def event(kind,this=0,args=(0,)*8):
  nonlocal snapshots
  expected.extend(struct.pack('<10I',kind,this,*args));expected.extend(u.mem_read(BASE,WIDTH));snapshots+=1
 def hook(u,a,n,_):
  if a not in [0x99a2f0,0xa0bf20]:return
  sp=u.reg_read(UC_X86_REG_ESP);this=u.reg_read(UC_X86_REG_ECX);cleanup=0
  if a==0x99a2f0:
   event(1,this);put(this,0x129a7c4)
   if mode==1:put(BASE+0x500,0x87654321)
   if mode==2:put(BASE+0x68,0x23456789)
  else:
   args=struct.unpack('<3I',u.mem_read(get(sp+4),12))+struct.unpack('<3I',u.mem_read(get(sp+8),12))+(get(sp+12),get(sp+16));event(2,this,args);cleanup=16
   for i in range(14):put(this+i*4,args[i%8]^(0x12340000+i))
   if mode==1:put(BASE+0x50c,0xabcdef01)
   elif mode==2:put(BASE+0x74,0x76543210);put(BASE+0xc0,0x10293847)
   elif mode==3:put(BASE+0x8,0x55443322);put(BASE+0xf8,0xdeadbeef)
  u.reg_write(UC_X86_REG_EAX,this);u.reg_write(UC_X86_REG_ESP,sp+4+cleanup);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook)
 for fill,pattern,mode in itertools.product(range(256),range(4),range(4)):
  arena=bytearray([fill])*WIDTH
  for i in range(9):struct.pack_into('<I',arena,0x500+i*4,[0,0xffffffff,0x12345678+i*0x10101,(fill*0x1010101+i)&0xffffffff][pattern])
  inputs.extend(struct.pack('<I',mode));inputs.extend(arena);u.mem_write(BASE,bytes(arena));sp=BASE+0xe000;stop=BASE+0xf000;u.mem_write(sp,struct.pack('<2I',stop,BASE+0x500));u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,BASE)
  u.emu_start(0x434e10,stop,count=10000);assert u.reg_read(UC_X86_REG_ESP)==sp+8;event(3,u.reg_read(UC_X86_REG_EAX));cases+=1
 return inputs,expected,dict(cases=cases,snapshots=snapshots)
