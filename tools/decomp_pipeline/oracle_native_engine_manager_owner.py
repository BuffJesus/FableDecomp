"""Retail singleton getter, owner reset, manager and list teardown composed."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from pe_oracle import EXE,pe_sections
BASE=0x24000000;GLOBAL=0x13ba854;WIDTH=4096
def scenarios():
 for op in [0,1]:
  for fill,n,refs,owned,existing,failure,mode in itertools.product([0,0x55,0xff],[0,1,3],[0,1,2,0xffffffff],range(2),range(2),[0,20,12]if op==0 else [0,12],range(4)):
   yield op,fill,n,refs,owned,existing,failure,mode,0,refs&1
 for fill,n,flags,mode,changed in itertools.product([0,0x55,0xff],[0,1,3,6],[0,1,2,0xffffffff],range(4),range(2)):yield 2,fill,n,1,1,1,0,mode,flags,changed
 for fill,n,mode in itertools.product([0,0x55,0xff],[0,1,3,6],range(4)):yield 3,fill,n,1,1,1,0,mode,0,0
 for fill,n,mode,existing in itertools.product([0,0x55,0xff],[0,1,3],range(4),range(2)):yield 4,fill,n,1,1,existing,0,mode,0,1
def collect():
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000);u.mem_map(BASE,0x10000);raw=EXE.read_bytes()
 for _,va,_,off,n in pe_sections(raw):
  if n:u.mem_write(0x400000+va,raw[off:off+n])
 inputs=bytearray();expected=bytearray();cases=snapshots=0
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def event(kind,a=0,b=0):
  nonlocal snapshots
  expected.extend(struct.pack('<3I',kind,a,b));expected.extend(u.mem_read(GLOBAL,8));expected.extend(u.mem_read(BASE,WIDTH));snapshots+=1
 def hook(u,a,n,_):
  if a not in [0xbfea1a,0xbfea0e,0xbfea14,0xbfe9bc]:return
  sp=u.reg_read(UC_X86_REG_ESP);arg=get(sp+4);result=0
  if a==0xbfea1a:
   assert arg in [20,12];event(1,arg);result=0 if failure==arg else BASE+(0x200 if arg==20 else 0x600)
   if mode==1:put(GLOBAL,BASE+(0x100 if arg==20 else 0x240))
  elif a==0xbfea0e:
   assert arg==12;event(2,arg);result=BASE+0x400
   if mode==2:put(BASE+0x208,BASE+0x820);put(GLOBAL+4,BASE+0x740)
  elif a==0xbfea14:
   event(3,arg)
   if mode==1 and BASE+0x320<=arg<BASE+0x3e0:put(arg,0)
   if mode==3 and arg==BASE+0x320:put(BASE+0x104,get(arg))
  else:
   event(4,arg)
   if mode==3 and arg==BASE+0x100:put(GLOBAL+4,BASE+0x740)
  u.reg_write(UC_X86_REG_EAX,result);u.reg_write(UC_X86_REG_ESP,sp+4);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook)
 for op,fill,count,refs,owned,existing,failure,mode,flags,changed in scenarios():
  arena=bytearray([fill])*WIDTH
  def word(o,v):struct.pack_into('<I',arena,o,v)
  for off,head in [(0x100,0x300),(0x200,0x400),(0x240,0x440)]:
   word(off,0x1231584);word(off+4,BASE+head)
   for j in range(3):word(off+8+j*4,BASE+0x800+j*16 if changed and off==0x100 else 0)
   word(head,BASE+head);word(head+4,BASE+head)
  word(0x300,BASE+0x320 if count else BASE+0x300);word(0x304,BASE+0x320+(count-1)*0x20 if count else BASE+0x300)
  for i in range(count):
   off=0x320+i*0x20;word(off,BASE+off+0x20 if i+1<count else BASE+0x300);word(off+4,BASE+off-0x20 if i else BASE+0x300);word(off+8,BASE+0x900+i*16)
  for off,r in [(0x700,refs),(0x740,2)]:word(off,r);word(off+4,0x4361e0);word(off+8,BASE+0x100)
  global_state=struct.pack('<2I',BASE+0x100 if op!=0 or existing else 0,BASE+0x700 if owned else 0)
  inputs.extend(struct.pack('<5I',op,mode,failure,flags,existing));inputs.extend(global_state);inputs.extend(arena);u.mem_write(BASE,bytes(arena));u.mem_write(GLOBAL,global_state)
  args=[BASE+0x200 if existing else 0]if op==1 else [flags]if op==2 else [];this=GLOBAL if op==1 else BASE+0x104 if op==3 else BASE+0x100 if op!=4 or existing else 0
  sp=BASE+0xe000;stop=BASE+0xf000;u.mem_write(sp,struct.pack('<'+'I'*(len(args)+1),stop,*args));u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,this)
  u.emu_start([0x436fb0,0x4366b0,0x437020,0x436900,0x4361e0][op],stop,count=100000);assert u.reg_read(UC_X86_REG_ESP)==sp+4+4*len(args);event(5,op,u.reg_read(UC_X86_REG_EAX)if op in [0,2]else 0);cases+=1
 return inputs,expected,dict(cases=cases,snapshots=snapshots)
