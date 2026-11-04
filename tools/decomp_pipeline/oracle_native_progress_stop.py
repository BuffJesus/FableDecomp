"""Nullable progress display forwarding and live owner mutation."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from pe_oracle import EXE,pe_sections
BASE=0x24000000;GLOBAL=0x13caa38
def collect():
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000);u.mem_map(BASE,0x10000);raw=EXE.read_bytes()
 for _,va,_,off,n in pe_sections(raw):
  if n:u.mem_write(0x400000+va,raw[off:off+n])
 inputs=bytearray();expected=bytearray();cases=snapshots=0
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def event(kind,this=0,a=0,b=0):
  nonlocal snapshots
  expected.extend(struct.pack('<4I',kind,this,a,b));expected.extend(u.mem_read(GLOBAL,8));expected.extend(u.mem_read(BASE,256));snapshots+=1
 def hook(u,a,n,_):
  if a!=BASE+0x9100:return
  sp=u.reg_read(UC_X86_REG_ESP);this=u.reg_read(UC_X86_REG_ECX);assert this==BASE+0x40;event(1,this,get(sp+4)&255,get(sp+8)&255);put(BASE+0x44,get(BASE+0x44)+1)
  if mode==1:put(GLOBAL,0)
  elif mode==2:put(GLOBAL,BASE+0x80);put(GLOBAL+4,0)
  u.reg_write(UC_X86_REG_ESP,sp+12);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook);put(BASE+0x8008,BASE+0x9100)
 for fill,present,a,b,mode in itertools.product(range(256),range(2),range(2),range(2),range(3)):
  arena=bytearray([fill])*256;struct.pack_into('<I',arena,0x40,BASE+0x8000);owner=struct.pack('<2I',BASE+0x40 if present else 0,BASE+0xa0);inputs.extend(struct.pack('<3I',mode,a,b));inputs.extend(owner);inputs.extend(arena);u.mem_write(BASE,bytes(arena));u.mem_write(GLOBAL,owner)
  sp=BASE+0xe000;stop=BASE+0xf000;put(sp,stop);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,a);u.reg_write(UC_X86_REG_EDX,b);u.emu_start(0x9e9f20,stop,count=1000);assert u.reg_read(UC_X86_REG_ESP)==sp+4;event(2);cases+=1
 return inputs,expected,dict(cases=cases,snapshots=snapshots)
