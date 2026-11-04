"""Engine list append/removal with live value aliases and mutating heap calls."""
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
 def event(kind,p=0):
  nonlocal snapshots
  expected.extend(struct.pack('<2I',kind,p));expected.extend(u.mem_read(BASE,WIDTH));snapshots+=1
 def hook(u,a,n,_):
  if a not in [0xbfea0e,0xbfea14]:return
  sp=u.reg_read(UC_X86_REG_ESP);arg=get(sp+4);event(1 if a==0xbfea0e else 2,arg)
  if a==0xbfea0e:
   assert arg==12;result=BASE+0x500
   if mode==1:put(BASE+4,BASE+0x140)
   elif mode==2:put(BASE+0x104,BASE+0x100)
   elif mode==3:put(BASE+0x108,0x10293847)
  else:
   result=0
   if mode==1:put(arg,0)
   elif mode==2:put(value_pointer,BASE+0x710)
   elif mode==3:put(BASE+4,BASE+0x140);put(arg+8,0)
  u.reg_write(UC_X86_REG_EAX,result);u.reg_write(UC_X86_REG_ESP,sp+4);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook)
 for op,count,index,mode,fill in itertools.product(range(4),range(7),range(4),range(4),[0,0x55,0xff]):
  arena=bytearray([fill])*WIDTH
  def word(o,v):struct.pack_into('<I',arena,o,v)
  word(4,BASE+0x100);word(0x100,BASE+0x200 if count else BASE+0x100);word(0x104,BASE+0x200+(count-1)*0x20 if count else BASE+0x100);word(0x140,BASE+0x140);word(0x144,BASE+0x140)
  for i in range(count):
   off=0x200+i*0x20;word(off,BASE+off+0x20 if i+1<count else BASE+0x100);word(off+4,BASE+off-0x20 if i else BASE+0x100);word(off+8,[BASE+0x700,BASE+0x710,0][(i+fill)%3])
  target=[BASE+0x700,BASE+0x710,BASE+0x720,0][index];word(0x800,target)
  # Parent methods receive an engine reference; null pointer values are tested
  # through list::remove, whose input is a reference to the pointer value.
  if op<2 and not target:continue
  value_pointer=BASE+0x208 if op==3 and count else BASE+0x800
  inputs.extend(struct.pack('<4I',op,count,mode,target));inputs.extend(arena);u.mem_write(BASE,bytes(arena));sp=BASE+0xe000;stop=BASE+0xf000;u.mem_write(sp,struct.pack('<2I',stop,target if op<2 else value_pointer));u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,BASE if op<2 else BASE+4)
  u.emu_start(0x6404d0 if op==0 else 0x6404b0 if op==1 else 0x6405b0,stop,count=10000);assert u.reg_read(UC_X86_REG_ESP)==sp+8;event(3);cases+=1
 return inputs,expected,dict(cases=cases,snapshots=snapshots)
