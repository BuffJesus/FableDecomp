"""Display startup composed with native view construction and counted ownership."""
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
 def event(kind,a=0,b=0,c=0,d=0):
  nonlocal snapshots
  expected.extend(struct.pack('<5I',kind,a,b,c,d));expected.extend(u.mem_read(BASE,WIDTH));snapshots+=1
 def hook(u,a,n,_):
  if a not in [0x99ebf0,0x99eae0,0x9e9f40,0xbfea1a,0xbfe9bc,BASE+0x9100,BASE+0x9200,BASE+0x9300]:return
  sp=u.reg_read(UC_X86_REG_ESP);this=u.reg_read(UC_X86_REG_ECX);cleanup=0;result=this
  if a==0x99ebf0:
   label={0x12315ac:1,0x1231588:2}[get(sp+4)];assert get(sp+8)==0xffffffff;event(1,label,get(sp+8));put(this,BASE+0xa00+label*16);cleanup=8
   if mode==1:put(BASE+0x68,0x11223344)
  elif a==0x99eae0:event(3,get(this));put(this,0)
  elif a==0x9e9f40:
   label=(get(this)-BASE-0xa00)//16;event(2,label,get(sp+4),u.reg_read(UC_X86_REG_EDX)&255,get(sp+8));cleanup=8
   if mode==2:put(BASE,BASE+0x8020)
   if mode==3 and label==2:put(BASE+0x68,0x87654321)
  elif a in [BASE+0x9200,BASE+0x9300]:
   event(4,this,1 if a==BASE+0x9200 else 2);result=refs&1
   if mode==1:u.mem_write(BASE+0x90,b'\0');put(BASE+0x98,0xdeadbeef);put(BASE+0x9c,0xdeadbeef)
  elif a==0xbfea1a:
   size=get(sp+4);assert size in [8,12];event(5,size);result=0 if failure==size else BASE+(0x900 if size==8 else 0x940)
   if mode==1:u.mem_write(BASE+0x90,b'\0');put(BASE+0x98,0x76543210)
   if mode==2 and size==8:put(BASE+0xe4,BASE+0x620)
   if mode==2 and size==12:put(BASE+0xe0,BASE+0x980)
  elif a==BASE+0x9100:
   event(6,this)
   if mode==3:put(BASE+0x504,BASE+0x5c0)
  else:event(7,get(sp+4))
  u.reg_write(UC_X86_REG_EAX,result);u.reg_write(UC_X86_REG_ESP,sp+4+cleanup);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook);put(BASE+0x8004,BASE+0x9200);put(BASE+0x8024,BASE+0x9300)
 for fill,refs,nested,present,failure,mode in itertools.product([0,1,0x55,0xff],[0,1,2,0xffffffff],[0,1,2,0xffffffff],range(2),[0,8,12],range(4)):
  arena=bytearray([fill])*WIDTH
  def word(o,v):struct.pack_into('<I',arena,o,v)
  word(0,BASE+0x8000);word(0xe0,BASE+0x500);word(0xe4,BASE+0x600 if present else 0)
  word(0x500,BASE+0x700);word(0x504,BASE+0x580)
  for off,count,fn,data in [(0x580,nested,BASE+0x9100,BASE+0x700),(0x5c0,11,BASE+0x9100,BASE+0x700),(0x600,refs,0x436ba0,BASE+0x500),(0x620,9,0x436ba0,BASE+0x500)]:
   for i,v in enumerate([count,fn,data]):word(off+i*4,v)
  inputs.extend(struct.pack('<3I',refs,failure,mode));inputs.extend(arena);u.mem_write(BASE,bytes(arena))
  def call(a,this):
   sp=BASE+0xe000;stop=BASE+0xf000;put(sp,stop);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,this);u.emu_start(a,stop,count=100000);assert u.reg_read(UC_X86_REG_ESP)==sp+4
  call(0x4350d0,BASE);event(8);call(0x436ba0,0);event(9);cases+=1
 return inputs,expected,dict(cases=cases,snapshots=snapshots)
