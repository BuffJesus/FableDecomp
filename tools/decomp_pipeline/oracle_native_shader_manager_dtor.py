"""Shader-manager teardown with actual member, pool, and base lifetimes."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from pe_oracle import EXE,pe_sections
BASE=0x24000000;WIDTH=0x4000
def scenarios():
 for op,present,count,refs,owned,mode in itertools.product(range(9),range(2),[0,1,3],[0,1,2],range(2),range(4)):
  yield op,present,count,refs,owned,mode
def collect():
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000);u.mem_map(BASE,0x10000);raw=EXE.read_bytes()
 for _,va,_,off,n in pe_sections(raw):
  if n:u.mem_write(0x400000+va,raw[off:off+n])
 inputs=bytearray();expected=bytearray();cases=snapshots=0
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def event(kind,a=0,b=0,c=0):
  nonlocal snapshots
  expected.extend(struct.pack('<4I',kind,a,b,c));expected.extend(u.mem_read(BASE,WIDTH));snapshots+=1
 def hook(u,a,n,_):
  if a not in [0x98a7a0,0xbfea14,0xbfe9bc,0x43020a,0x99eae0,BASE+0x9100,BASE+0x9200]:return
  sp=u.reg_read(UC_X86_REG_ESP);this=u.reg_read(UC_X86_REG_ECX);cleanup=0
  if a==0x98a7a0:
   event(1,this)
   if mode==1:put(BASE+0x3cc,BASE+0x2100)
  elif a==0xbfea14:
   p=get(sp+4);event(2,p)
   if mode==2:put(BASE+0x390,get(BASE+0x390)^0x12345678)
   if mode==3 and p==BASE+0x2000:put(BASE+0x3cc,BASE+0x2140)
  elif a==0xbfe9bc:event(3,get(sp+4))
  elif a==0x43020a:
   event(4,this,get(sp+4));cleanup=4
   if mode==3:put(this,BASE+0x1140)
  elif a==0x99eae0:
   event(5,this);put(this,0)
   if mode==1:put(BASE+0x3b8,BASE+0x1200)
  elif a==BASE+0x9100:
   event(6,this,get(sp+4));cleanup=4
   if mode==2 and BASE+0x1800<=this<BASE+0x1900:put(this+20,0)
   put(this+4,0x76543210)
  else:
   event(7,this)
   if mode==3:put(BASE+0x3a8,BASE+0x1340)
  u.reg_write(UC_X86_REG_EAX,this);u.reg_write(UC_X86_REG_ESP,sp+4+cleanup);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook);put(BASE+0x8000,BASE+0x9100)
 for op,present,count,refs,owned,mode in scenarios():
  arena=bytearray((i*17+cases*13)&255 for i in range(WIDTH))
  def word(o,v):struct.pack_into('<I',arena,o,v&0xffffffff)
  def vector(o,p,n,stride):
   for j,v in enumerate([p,p+n*stride if p else 0,p+n*stride if p else 0]):word(o+j*4,v)
  for o,p,stride in [(0x3dc,0x2000,48),(0x3cc,0x2100,16),(0x1a0,0x2200,16),(0x194,0x2300,16),(0xc,0x2400,4),(0x158,0x2500,16)]:vector(o,BASE+p if present else 0,count,stride)
  vector(0x3b4,BASE+0x1200 if count else 0,count,4);word(0x3c0,BASE+0x1100);word(0x3c4,count)
  for off in [0x1100,0x1140]:word(off+4,BASE+0x1180);word(off+8,BASE+off);word(off+12,BASE+off)
  vector(0x3a8,BASE+0x1300 if count else 0,count,8)
  for i in range(count):
   word(0x1200+i*4,BASE+0x2600+i*16);word(0x1300+i*8,BASE+0x1600+i*32);word(0x1304+i*8,BASE+0x1400+i*16)
   word(0x1400+i*16,refs);word(0x1404+i*16,BASE+0x9200);word(0x1408+i*16,BASE+0x1600+i*32)
  word(0x1e4,BASE+0x1700 if present else 0);word(0x1700,BASE+0x8000);word(0x1708,refs)
  word(0x17c,BASE+0x1800 if count else 0)
  for i in range(count):
   off=0x1800+i*0x30;word(off,BASE+0x8000);arena[off+12]=owned if i==0 else 1;word(off+20,BASE+off+0x30 if i+1<count else 0)
  inputs.extend(struct.pack('<6I',op,present,count,refs,owned,mode));inputs.extend(arena);u.mem_write(BASE,bytes(arena))
  sp=BASE+0xe000;stop=BASE+0xf000;args=[]if op==0 else [[0,1,2,0xffffffff][(op-1)%4]];u.mem_write(sp,struct.pack('<'+'I'*(len(args)+1),stop,*args));u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,BASE+(0x1e0 if op>=5 else 0))
  u.emu_start(0x98abf0 if op==0 else 0x98c3d0 if op>=5 else 0x98de90,stop,count=100000);assert u.reg_read(UC_X86_REG_ESP)==sp+4+len(args)*4
  event(8,op,u.reg_read(UC_X86_REG_EAX)if op else 0);cases+=1
 return inputs,expected,dict(cases=cases,snapshots=snapshots)
