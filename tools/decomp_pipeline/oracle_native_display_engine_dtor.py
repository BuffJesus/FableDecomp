"""Retail display teardown with real nested view-owner and base destruction."""
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
 def event(kind,a=0,b=0):
  nonlocal snapshots
  expected.extend(struct.pack('<3I',kind,a,b));expected.extend(u.mem_read(BASE,WIDTH));snapshots+=1
 def hook(u,a,n,_):
  if a not in [BASE+0x9100,BASE+0x9200,0x436fb0,0x6404b0,0xbfe9bc]:return
  sp=u.reg_read(UC_X86_REG_ESP);this=u.reg_read(UC_X86_REG_ECX);cleanup=0;result=this
  if a==BASE+0x9100:
   event(1,this)
   if mode==1:put(BASE+0x2c,BASE+0x440)
  elif a==0x436fb0:
   event(2);result=BASE+0xa00
   if mode==2:put(BASE+0x2c,BASE+0x440)
  elif a==0x6404b0:
   event(3,this,get(sp+4));cleanup=4
   if mode==3:put(BASE+0xe4,BASE+0x620)
  elif a==BASE+0x9200:
   event(4,this)
   if mode==4:put(BASE+0xe4,BASE+0x620);put(BASE+0x504,BASE+0x5c0)
  else:event(5,get(sp+4))
  u.reg_write(UC_X86_REG_EAX,result);u.reg_write(UC_X86_REG_ESP,sp+4+cleanup);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook);put(BASE+0x800c,BASE+0x9100)
 for op,refs,nested,present,mode,fill in itertools.product(range(6),[0,1,2,0xffffffff],[0,1,2,0xffffffff],range(2),range(5),[0,0x55,0xff]):
  arena=bytearray([fill])*WIDTH
  def word(o,v):struct.pack_into('<I',arena,o,v)
  word(0x2c,BASE+0x400);word(0x400,BASE+0x8000);word(0x440,BASE+0x8000);word(0xe0,BASE+0x500);word(0xe4,BASE+0x600 if present else 0);word(0x500,BASE+0x700);word(0x504,BASE+0x580)
  for off,count,fn,data in [(0x580,nested,BASE+0x9200,BASE+0x700),(0x5c0,11,BASE+0x9200,BASE+0x700),(0x600,refs,0x436ba0,BASE+0x500),(0x620,9,0x436ba0,BASE+0x500)]:
   for i,v in enumerate([count,fn,data]):word(off+i*4,v)
  inputs.extend(struct.pack('<2I',op,mode));inputs.extend(arena);u.mem_write(BASE,bytes(arena));args=[]if op<2 else [[0,1,2,0xffffffff][op-2]];sp=BASE+0xe000;stop=BASE+0xf000;u.mem_write(sp,struct.pack('<'+'I'*(len(args)+1),stop,*args));u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,BASE)
  u.emu_start(0x4354a0 if op==0 else 0x4354c0 if op==1 else 0x437070,stop,count=100000);assert u.reg_read(UC_X86_REG_ESP)==sp+4+4*len(args);event(6,op,u.reg_read(UC_X86_REG_EAX)if op>=2 else 0);cases+=1
 return inputs,expected,dict(cases=cases,snapshots=snapshots)
