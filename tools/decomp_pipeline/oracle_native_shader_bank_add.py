"""Shader-bank insertion and its actual pointer-vector overflow implementation."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from pe_oracle import EXE,pe_sections
BASE=0x24000000;WIDTH=0x4000

def scenarios():
 for n in range(5):
  for cap in sorted(set([n,8])):
   for index,kind,mode,fail in itertools.product(sorted(set([0,n,n+2])),[0,1,2,0xffffffff],range(4),[0,1]):
    yield(0,n,cap,index,kind,mode,fail,1,1,0)
 for n in range(7):
  for pos,fill,atend,alias,mode in itertools.product(range(n+1),[0,1,3],[0,1],[0,1],range(4)):
   if alias and n==0:continue
   yield(1,n,n,pos,0,mode,0,fill,atend,alias)

def collect():
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000);u.mem_map(BASE,0x10000);raw=EXE.read_bytes()
 for _,va,_,off,n in pe_sections(raw):
  if n:u.mem_write(0x400000+va,raw[off:off+n])
 inputs=bytearray();expected=bytearray();cases=snapshots=0;heap=0
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def event(kind,a=0,b=0,c=0,d=0):
  nonlocal snapshots
  expected.extend(struct.pack('<5I',kind,a,b,c,d));expected.extend(u.mem_read(BASE,WIDTH));snapshots+=1
 def hook(u,a,n,_):
  nonlocal heap
  if a not in [0xbfea0e,0xbfeae6,0xbfea14,0xbfea1a,0xa62120,0x9cbc20]:return
  sp=u.reg_read(UC_X86_REG_ESP);this=u.reg_read(UC_X86_REG_ECX);cleanup=0;result=0
  if a==0xbfea0e:
   size=get(sp+4);assert size<=128 and size>0;event(1,size);result=BASE+0x1000+heap*0x100;heap+=1
   if mode==1:put(BASE+0x118,BASE+0x3040)
   elif mode==2:
    put(BASE+0x200,get(BASE+0x200)^0x100);put(BASE+0x900,get(BASE+0x900)^0x101)
  elif a==0xbfeae6:
   dest,source,size=[get(sp+4+i*4)for i in range(3)];assert size<=128;event(2,dest,source,size)
   if size:u.mem_write(dest,bytes(u.mem_read(source,size)))
   result=dest
  elif a==0xbfea14:
   pointer=get(sp+4);event(3,pointer)
   if mode==3:put(BASE+0x110,BASE+0x3000);put(BASE+0x114,BASE+0x3004);put(BASE+0x118,BASE+0x3040)
  elif a==0xbfea1a:
   assert get(sp+4)==0x24;event(4,0x24);result=0 if fail else BASE+0x600
  elif a==0xa62120:
   assert this==BASE+0x600 and get(sp+4)==shader_type and get(sp+8)==index and get(sp+12)==BASE
   event(5,this,shader_type,index,BASE);u.mem_write(this,b'\x6a'*0x24);put(this+4,shader_type);cleanup=12;result=this
   if mode==1:put(BASE+0x110,BASE+0x3000);put(BASE+0x114,BASE+0x3000+(index+1)*4);put(BASE+0x118,BASE+0x3040)
  else:
   assert this==BASE and get(sp+4)==shader_type and get(sp+8)==index and get(sp+12)==BASE+0x700
   event(6,this,shader_type,index,BASE+0x700);put(BASE+4,0x98760000|index);cleanup=12
  u.reg_write(UC_X86_REG_EAX,result);u.reg_write(UC_X86_REG_ESP,sp+4+cleanup);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook)
 for op,count,capacity,index,shader_type,mode,fail,fill,atend,alias in scenarios():
  arena=bytearray((i*17+cases*13)&255 for i in range(WIDTH))
  def word(o,v):struct.pack_into('<I',arena,o,v&0xffffffff)
  start=BASE+0x200 if capacity else 0
  word(0x110,start);word(0x114,start+count*4);word(0x118,start+capacity*4);word(0x900,0xabcdef98)
  params=(op,count,capacity,index,shader_type,mode,fail,fill,atend,alias);inputs.extend(struct.pack('<10I',*params));inputs.extend(arena);u.mem_write(BASE,bytes(arena));heap=0
  args=[shader_type,index,BASE+0x700] if op==0 else [start+index*4,BASE+0x200 if alias else BASE+0x900,BASE+0x940,fill,atend]
  sp=BASE+0xe000;stop=BASE+0xf000;u.mem_write(sp,struct.pack('<'+'I'*(len(args)+1),stop,*args));u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,BASE if op==0 else BASE+0x110)
  u.emu_start(0xa623d0 if op==0 else 0xa62740,stop,count=100000);assert u.reg_read(UC_X86_REG_ESP)==sp+4+len(args)*4
  event(7,op);cases+=1
 return inputs,expected,dict(cases=cases,snapshots=snapshots)
