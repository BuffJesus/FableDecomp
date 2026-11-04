"""Retail CRT initialization and reverse registered destruction of progress tips."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from pe_oracle import EXE,pe_sections
BASE=0x24000000;GLOBALS=[0x13b89d8,0x13b89dc,0x13bca20]
def collect(mode):
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000);u.mem_map(BASE,0x10000);raw=EXE.read_bytes()
 for _,va,_,off,n in pe_sections(raw):
  if n:u.mem_write(0x400000+va,raw[off:off+n])
 expected=bytearray();registered=[];snapshots=0
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def put(a,v):u.mem_write(a,struct.pack('<I',v))
 def event(kind,arg=0):
  nonlocal snapshots
  expected.extend(struct.pack('<2I',kind,arg))
  for a in GLOBALS:expected.extend(u.mem_read(a,4))
  expected.extend(u.mem_read(BASE,4096));snapshots+=1
 def hook(u,a,n,_):
  if a not in (0x4012bc,0xbfea14,0xbfe9bc,0x99b3c0):return
  assert a!=0x99b3c0,'Empty literals must not allocate'
  sp=u.reg_read(UC_X86_REG_ESP);arg=get(sp+4)
  if a==0x4012bc:registered.append(arg)
  else:event(2 if a==0xbfea14 else 3,arg)
  u.reg_write(UC_X86_REG_EAX,0);u.reg_write(UC_X86_REG_ESP,sp+4);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook)
 def call(a):
  sp=BASE+0xe000;stop=BASE+0xf000;put(sp,stop);u.reg_write(UC_X86_REG_ESP,sp);u.emu_start(a,stop,count=10000);assert u.reg_read(UC_X86_REG_ESP)==sp+4
 for a in GLOBALS:put(a,0)
 call(0x121a3e0);call(0x121a400);assert registered==[0x1228c40,0x1228c50];event(0)
 if mode:
  put(GLOBALS[0],BASE+0x100);put(GLOBALS[1],BASE+(0x100 if mode==2 else 0x120))
  for i in range(2):
   o=BASE+0x100+i*0x20;text=BASE+0x400+i*0x40
   for off,v in [(0,text),(4,text+4),(8,text+16),(12,2 if mode==2 else 0 if mode==3 else 1)]:put(o+off,v)
 event(1)
 for a in reversed(registered):call(a)
 event(9);return expected,dict(snapshots=snapshots,registered=registered)
