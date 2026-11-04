"""Real engine singleton CRT registration, allocation, engine list and shutdown."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from pe_oracle import EXE,pe_sections
BASE=0x24000000;GLOBAL=0x13ba854
def collect():
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000);u.mem_map(BASE,0x10000);raw=EXE.read_bytes()
 for _,va,_,off,n in pe_sections(raw):
  if n:u.mem_write(0x400000+va,raw[off:off+n])
 expected=bytearray();registered=[];snapshots=mallocs=0
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def put(a,v):u.mem_write(a,struct.pack('<I',v))
 def event(kind,arg=0):
  nonlocal snapshots
  expected.extend(struct.pack('<2I',kind,arg));expected.extend(u.mem_read(GLOBAL,8));expected.extend(u.mem_read(BASE,4096));snapshots+=1
 def hook(u,a,n,_):
  nonlocal mallocs
  if a not in [0x4012bc,0xbfea1a,0xbfea0e,0xbfea14,0xbfe9bc]:return
  sp=u.reg_read(UC_X86_REG_ESP);arg=get(sp+4);result=0
  if a==0x4012bc:registered.append(arg)
  elif a==0xbfea1a:assert arg in [20,12];event(2,arg);result=BASE+(0x100 if arg==20 else 0x700)
  elif a==0xbfea0e:assert arg==12;event(3,arg);result=BASE+0x300+mallocs*0x20;mallocs+=1
  else:event(4 if a==0xbfea14 else 5,arg)
  u.reg_write(UC_X86_REG_EAX,result);u.reg_write(UC_X86_REG_ESP,sp+4);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook)
 def call(a,this=0,args=()):
  sp=BASE+0xe000;stop=BASE+0xf000;u.mem_write(sp,struct.pack('<'+'I'*(len(args)+1),stop,*args));u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,this);u.emu_start(a,stop,count=100000);assert u.reg_read(UC_X86_REG_ESP)==sp+4+4*len(args);return u.reg_read(UC_X86_REG_EAX)
 call(0x121df10);assert registered==[0x1229190];event(0)
 event(1,call(0x436fb0));event(1,call(0x436fb0))
 for p in [BASE+0x900,BASE+0x910]:call(0x6404d0,BASE+0x100,(p,));event(6,p)
 call(0x6404b0,BASE+0x100,(BASE+0x900,));event(7)
 for i in range(3):put(BASE+0x108+i*4,BASE+0x800+i*16)
 event(8);call(registered[0]);event(9)
 return expected,dict(snapshots=snapshots,registered_destructor='01229190')
