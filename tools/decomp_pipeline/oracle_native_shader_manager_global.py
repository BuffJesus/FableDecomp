"""Actual retail CRT shader-manager construction, empty shutdown and teardown."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from pe_oracle import EXE,pe_sections
BASE=0x24000000;GLOBAL=0x13bc470;WIDTH=1000
def collect():
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000);u.mem_map(BASE,0x10000);raw=EXE.read_bytes()
 for _,va,_,off,n in pe_sections(raw):
  if n:u.mem_write(0x400000+va,raw[off:off+n])
 expected=bytearray();registered=[];snapshots=0
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def put(a,v):u.mem_write(a,struct.pack('<I',v))
 def event(kind,arg=0):
  nonlocal snapshots
  expected.extend(struct.pack('<2I',kind,arg));expected.extend(u.mem_read(GLOBAL,WIDTH));expected.extend(u.mem_read(BASE+0x1000,24));snapshots+=1
 def hook(u,a,n,_):
  if a not in [0xbfea0e,0xbfea14,0x4012bc]:return
  sp=u.reg_read(UC_X86_REG_ESP);result=0
  if a==0xbfea0e:assert get(sp+4)==24;event(1,24);result=BASE+0x1000
  elif a==0xbfea14:event(4,get(sp+4))
  else:registered.append(get(sp+4))
  u.reg_write(UC_X86_REG_EAX,result);u.reg_write(UC_X86_REG_ESP,sp+4);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook)
 def call(a):
  sp=BASE+0xe000;stop=BASE+0xf000;put(sp,stop);u.reg_write(UC_X86_REG_ESP,sp);u.emu_start(a,stop,count=100000);assert u.reg_read(UC_X86_REG_ESP)==sp+4
 call(0x1223d00);assert registered==[0x122a4c0];event(2)
 for o,p,stride in [(0x3dc,0x2000,48),(0x3cc,0x2100,16),(0x1a0,0x2200,16),(0x194,0x2300,16),(0x158,0x2400,16),(0xc,0x2500,4)]:
  for j,value in enumerate([BASE+p,BASE+p+stride,BASE+p+2*stride]):put(GLOBAL+o+j*4,value)
 put(GLOBAL+0x1d4,0xabcdef00);event(3);call(registered[0]);event(5)
 return expected,dict(snapshots=snapshots,registered_destructor='0122a4c0')
