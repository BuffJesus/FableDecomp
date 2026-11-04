"""Bank entry read ordering with packed metadata and mutating file callbacks."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from pe_oracle import EXE,pe_sections,va_to_off
BASE=0x24000000;WIDTH=0x1000

def collect():
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x900000,0x200000);u.mem_map(BASE,0x10000);raw=EXE.read_bytes();off=va_to_off(pe_sections(raw),0x9cc2a0);u.mem_write(0x9cc2a0,raw[off:off+95])
 inputs=bytearray();expected=bytearray();cases=snapshots=0
 def get(a):return int.from_bytes(u.mem_read(a,4),'little')
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def event(kind,a=0,b=0,c=0):
  nonlocal snapshots
  expected.extend(struct.pack('<4I',kind,a,b,c));expected.extend(u.mem_read(BASE,WIDTH));snapshots+=1
 def hook(u,a,n,_):
  if a not in [0xa629f0,BASE+0x9100,BASE+0x9140]:return
  sp=u.reg_read(UC_X86_REG_ESP);this=u.reg_read(UC_X86_REG_ECX);cleanup=0;result=0
  if a==0xa629f0:
   assert this in [BASE+0x38,BASE+0x48] and get(sp+4)==((index-1)&0xffffffff);event(1,this,get(sp+4));cleanup=4
   if this==BASE+0x48:
    result=0x76543210
    if mode==1:put(BASE+0x3c,1-get(BASE+0x3c));put(BASE+0x20,BASE+0x300);put(BASE+0x7c,BASE+0x540)
   else:
    result=get(BASE+0x900)
    if mode==2:put(BASE+0x7c,BASE+0x580)
  elif a==BASE+0x9100:
   assert this in [BASE+0x500,BASE+0x540,BASE+0x580];position=get(sp+4);event(2,this,position);put(this+4,position);cleanup=4
   if mode:put(BASE+0x7c,BASE+0x540);put(BASE+0x900,get(BASE+0x900)^0x80000000)
  else:
   assert this in [BASE+0x500,BASE+0x540,BASE+0x580] and get(sp+4)==BASE+0x700 and get(sp+8)==length and get(sp+12)==0
   event(3,this,get(sp+4),length);put(BASE+0x700,0x12340000|mode);cleanup=12
  u.reg_write(UC_X86_REG_EAX,result);u.reg_write(UC_X86_REG_ESP,sp+4+cleanup);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook);put(BASE+0x8000+20,BASE+0x9100);put(BASE+0x8000+12,BASE+0x9140)
 for index,packed_size,packed_offset,offset,length,mode in itertools.product([0,1,3],[0,1],[0,1],[0,1,0x7fffffff,0xffffffff],[0,1,8,0xffffffff],range(4)):
  arena=bytearray((i*17+cases*13)&255 for i in range(WIDTH))
  def word(o,v):struct.pack_into('<I',arena,o,v&0xffffffff)
  word(0x4c,packed_size);word(0x3c,packed_offset);word(0x20,BASE+0x200);word(0x7c,BASE+0x500)
  for o in [0x500,0x540,0x580]:word(o,BASE+0x8000)
  inputs.extend(struct.pack('<4I',index,offset,length,mode));inputs.extend(arena);u.mem_write(BASE,bytes(arena))
  sp=BASE+0xe000;stop=BASE+0xf000;u.mem_write(sp,struct.pack('<5I',stop,index,offset,BASE+0x700,length));u.reg_write(UC_X86_REG_ECX,BASE);u.reg_write(UC_X86_REG_ESP,sp)
  u.emu_start(0x9cc2a0,stop,count=1000);assert u.reg_read(UC_X86_REG_ESP)==sp+20;event(4);cases+=1
 return inputs,expected,dict(cases=cases,snapshots=snapshots)
