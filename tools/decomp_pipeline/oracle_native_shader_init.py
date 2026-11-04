"""Full shader-manager initialization against recorded native dependencies."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from pe_oracle import EXE,pe_sections
BASE=0x24000000;WIDTH=0x4000
NAMES=['PSCONST_INPUT_FACTOR_0','PSCONST_INPUT_FACTOR_1','PSCONST_OUTPUT_FACTOR','PSCONST_MAX_FOG_ALPHA','PSCONST_SHADOW_FACTOR_AND_FADE_COLOUR','PSCONST_USER_0','PSCONST_USER_1','PSCONST_USER_2','PSCONST_USER_3']
NAME_ADDRESSES=[0x129a134,0x129a11c,0x129a104,0x129a0ec,0x129a0c4,0x129a0b4,0x129a0a4,0x129a094,0x129a084]
def collect():
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000);u.mem_map(BASE,0x10000);raw=EXE.read_bytes()
 for _,va,_,off,n in pe_sections(raw):
  if n:u.mem_write(0x400000+va,raw[off:off+n])
 inputs=bytearray();expected=bytearray();cases=snapshots=0;resource_calls=0;name_calls=0
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def event(kind,a=0,b=0,c=0):
  nonlocal snapshots
  expected.extend(struct.pack('<4I',kind,a,b,c));expected.extend(u.mem_read(BASE,WIDTH));snapshots+=1
 def hook(u,a,n,_):
  nonlocal resource_calls,name_calls
  if a not in [0x98d870,0xa5d4d0,0x99ebf0,0x98db20,0x99eae0,0x98cf70,0x98d360]:return
  sp=u.reg_read(UC_X86_REG_ESP);this=u.reg_read(UC_X86_REG_ECX);cleanup=0;result=this
  if a==0x98d870:
   assert this==BASE+12 and get(sp+4)==1 and get(get(sp+8))==0;event(1,this,1);cleanup=8;put(this,BASE+0x500);put(this+4,BASE+0x504);put(this+8,BASE+0x510)
   if mode==1:put(BASE+0x3a8,BASE+0x700);put(BASE+0x3ac,BASE+0x700+resources*8)
  elif a==0xa5d4d0:
   assert get(sp+4)==BASE;event(2,this,BASE);cleanup=4;put(this+0x14,BASE);resource_calls+=1
   if mode==2:put(BASE+0x3ac,get(BASE+0x3a8)+8)
   elif mode==3 and resource_calls==1:put(BASE+0x3ac,get(BASE+0x3a8)+(resources+1)*8)
  elif a==0x99ebf0:
   literal=get(sp+4);index=NAME_ADDRESSES.index(literal);assert get(sp+8)==0xffffffff and bytes(u.mem_read(literal,len(NAMES[index])+1))==NAMES[index].encode()+b'\0';event(3,index,0xffffffff);put(this,0xabc00000+index);cleanup=8
  elif a==0x98db20:
   assert this==BASE+0x3b4;value=get(get(sp+4));assert value==0xabc00000+name_calls;event(4,value);cleanup=4;name_calls+=1
   end=get(this+4);put(end,value);put(this+4,end+4)
   if mode==4:put(BASE+0x3cc,BASE+0x2000);put(BASE+0x3d0,BASE+0x2000+tablecount*16);put(BASE+0x3d4,BASE+0x2100)
  elif a==0x99eae0:event(5,get(this));put(this,0)
  elif a==0x98d360:
   position,count,value=[get(sp+4+i*4)for i in range(3)];assert this==BASE+0x3cc and bytes(u.mem_read(value,16))==b'\0'*16;event(6,position,count);cleanup=12
   for i in range(count):u.mem_write(position+i*16,b'\0'*16)
   put(this+4,get(this+4)+count*16)
   if mode==5:put(this,BASE+0x2000);put(this+4,BASE+0x2090);put(this+8,BASE+0x2100)
  else:
   first,last=get(sp+4),get(sp+8);assert this==BASE+0x3cc;event(7,first,last);cleanup=8;put(this+4,first);result=first
   if mode==5:put(this,BASE+0x2000);put(this+4,BASE+0x2090);put(this+8,BASE+0x2100)
  u.reg_write(UC_X86_REG_EAX,result);u.reg_write(UC_X86_REG_ESP,sp+4+cleanup);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook)
 for resources,layouts,tablecount,namecount,mode in itertools.product([0,1,3],[0,1,3],[0,3,9,10,12],[0,2],range(6)):
  arena=bytearray((i*7+cases*19)&255 for i in range(WIDTH))
  def pack(o,*values):struct.pack_into('<'+'I'*len(values),arena,o,*values)
  pack(12,BASE+0x500,BASE+0x500+layouts*4,BASE+0x510);pack(0x3a8,BASE+0x600,BASE+0x600+resources*8,BASE+0x620)
  pack(0x3b4,BASE+0x1400,BASE+0x1400+namecount*4,BASE+0x1480);pack(0x3cc,BASE+0x1800,BASE+0x1800+tablecount*16,BASE+0x1900)
  for i in range(4):pack(0x600+i*8,BASE+0x1000+i*0x80,0x12345000+i);pack(0x700+i*8,BASE+0x1000+(3-i)*0x80,0x56789000+i)
  inputs.extend(struct.pack('<5I',resources,layouts,tablecount,namecount,mode));inputs.extend(arena);u.mem_write(BASE,bytes(arena));resource_calls=name_calls=0
  sp=BASE+0xe000;stop=BASE+0xf000;u.mem_write(sp,struct.pack('<3I',stop,BASE+0x3000,BASE+0x3100));u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,BASE);u.emu_start(0x98acf0,stop,count=100000);assert u.reg_read(UC_X86_REG_ESP)==sp+12 and name_calls==9
  event(8);cases+=1
 return inputs,expected,dict(cases=cases,snapshots=snapshots)
