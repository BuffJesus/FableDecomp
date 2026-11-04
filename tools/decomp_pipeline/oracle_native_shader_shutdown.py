"""Shader-manager shutdown with actual resource uninitialization and bank close."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from pe_oracle import EXE,pe_sections
BASE=0x24000000;WIDTH=0x4000
def collect():
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000);u.mem_map(BASE,0x10000);raw=EXE.read_bytes()
 for _,va,_,off,n in pe_sections(raw):
  if n:u.mem_write(0x400000+va,raw[off:off+n])
 inputs=bytearray();expected=bytearray();cases=snapshots=0;deletes=0
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def event(kind,a=0,b=0):
  nonlocal snapshots
  expected.extend(struct.pack('<3I',kind,a,b));expected.extend(u.mem_read(BASE,WIDTH));snapshots+=1
 def hook(u,a,n,_):
  nonlocal deletes
  if a!=BASE+0x9100:return
  sp=u.reg_read(UC_X86_REG_ESP);this=u.reg_read(UC_X86_REG_ECX);assert get(sp+4)==1;event(1,this,1);deletes+=1
  if mode==1:put(BASE+0x3a8,BASE+0x600);put(BASE+0x3ac,BASE+0x600+count*8)
  elif mode==2:put(BASE+0x3ac,get(BASE+0x3a8)+8)
  elif mode==3 and deletes==1:put(BASE+0x3ac,get(BASE+0x3a8)+(count+1)*8)
  elif mode==4:
   resource=get(this+4);put(resource+8,BASE+0x2200);put(resource+12,BASE+0x2204)
  put(BASE+0x1d4,0xabcdef12);put(this+8,0x98765432)
  u.reg_write(UC_X86_REG_EAX,this);u.reg_write(UC_X86_REG_ESP,sp+8);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook);put(BASE+0x8000,BASE+0x9100)
 for count,shadercount,pattern,mode in itertools.product(range(7),range(4),range(3),range(5)):
  arena=bytearray((i*11+cases*17)&255 for i in range(WIDTH))
  def pack(o,*values):struct.pack_into('<'+'I'*len(values),arena,o,*values)
  pack(0x3a8,BASE+0x400,BASE+0x400+count*8,BASE+0x438);pack(0x1d4,0x12345678);pack(0x2200,0)
  for i in range(7):
   resource=BASE+0x800+i*0x100;pack(0x400+i*8,resource,0x77889900+i);pack(0x600+i*8,BASE+0x800+(6-i)*0x100,0x99887700+i)
   array=BASE+0x1800+i*0x80;pack(resource-BASE+4,BASE+0x3000,array,array+shadercount*4,array+12,BASE)
   for j in range(3):
    shader=BASE+0x2400+i*0xc0+j*0x40;pack(array-BASE+j*4,shader if pattern==1 or(pattern==2 and j%2==0)else 0);pack(shader-BASE,BASE+0x8000,resource,0)
  inputs.extend(struct.pack('<4I',count,shadercount,pattern,mode));inputs.extend(arena);u.mem_write(BASE,bytes(arena));deletes=0
  sp=BASE+0xe000;stop=BASE+0xf000;put(sp,stop);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,BASE);u.emu_start(0x98a7a0,stop,count=100000);assert u.reg_read(UC_X86_REG_ESP)==sp+4
  event(2);cases+=1
 return inputs,expected,dict(cases=cases,snapshots=snapshots)
