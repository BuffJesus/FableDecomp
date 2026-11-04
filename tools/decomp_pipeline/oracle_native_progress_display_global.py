"""Real global progress owner CRT lifetime and native stop dispatch."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from pe_oracle import EXE,pe_sections
BASE=0x24000000;GLOBAL=0x13caa38
def collect(mode):
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000);u.mem_map(BASE,0x10000);raw=EXE.read_bytes()
 for _,va,_,off,n in pe_sections(raw):
  if n:u.mem_write(0x400000+va,raw[off:off+n])
 expected=bytearray();registered=[];snapshots=0
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def event(kind,arg=0,flag=0):
  nonlocal snapshots
  expected.extend(struct.pack('<3I',kind,arg,flag));expected.extend(u.mem_read(GLOBAL,8));expected.extend(u.mem_read(BASE,4096));snapshots+=1
 def hook(u,a,n,_):
  if a not in (0x4012bc,0xbfe9bc,BASE+0x9100,BASE+0x9200):return
  sp=u.reg_read(UC_X86_REG_ESP);this=u.reg_read(UC_X86_REG_ECX);cleanup=0
  if a==0x4012bc:registered.append(get(sp+4))
  elif a==0xbfe9bc:event(4,get(sp+4))
  elif a==BASE+0x9100:
   assert this==BASE+0x40 and get(sp+4)&255==1 and get(sp+8)&255==0;event(2,this,1);put(BASE+0x44,get(BASE+0x44)+1);cleanup=8
  else:
   event(3,this)
   if mode==5:put(GLOBAL,0);put(GLOBAL+4,BASE+0x120)
  u.reg_write(UC_X86_REG_EAX,0);u.reg_write(UC_X86_REG_ESP,sp+4+cleanup);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook)
 def call(a,this=0):
  sp=BASE+0xe000;stop=BASE+0xf000;put(sp,stop);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,this);u.reg_write(UC_X86_REG_EDX,0);u.emu_start(a,stop,count=10000);assert u.reg_read(UC_X86_REG_ESP)==sp+4
 put(GLOBAL,0);put(GLOBAL+4,0);put(BASE+0x8008,BASE+0x9100);call(0x12241b0);assert registered==[0x122a890];event(0)
 if mode:
  put(GLOBAL,BASE+0x40);put(GLOBAL+4,BASE+0x100)
  for off,v in [(0x40,BASE+0x8000),(0x100,2 if mode==2 else 0 if mode==3 else 0xffffffff if mode==4 else 1),(0x104,BASE+0x9200),(0x108,BASE+0x40)]:put(BASE+off,v)
 event(1);call(0x9e9f20,1);event(5);call(registered[0]);event(9)
 return expected,dict(snapshots=snapshots)
