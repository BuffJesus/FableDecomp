"""Concrete engine shutdown, GPU detach ordering and mutable resource lists."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from pe_oracle import EXE,pe_sections
BASE=0x24000000;WIDTH=0x9000
GLOBALS=(0x1436e18,0x1436e1c,0x1436e78,0x1436eb0)
def collect():
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000);u.mem_map(BASE,0x40000);raw=EXE.read_bytes()
 for _,va,_,off,n in pe_sections(raw):
  if n:u.mem_write(0x400000+va,raw[off:off+n])
 inputs=bytearray();expected=bytearray();cases=snapshots=0
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def event(kind,self,args=()):
  nonlocal snapshots
  expected.extend(struct.pack('<6I',kind,self,*(tuple(args)+(0,)*(4-len(args)))));expected.extend(b''.join(u.mem_read(a,4)for a in GLOBALS));expected.extend(u.mem_read(BASE,WIDTH));snapshots+=1
 def swap():
  if mode&1:put(GLOBALS[0],BASE+0x5000 if get(GLOBALS[0])==BASE+0x1000 else BASE+0x1000)
 def hook(u,a,n,_):
  if a not in (0x9bf5a0,0x99a360,BASE+0x31000,BASE+0x31100,BASE+0x31200,BASE+0x31300):return
  sp=u.reg_read(UC_X86_REG_ESP);self=u.reg_read(UC_X86_REG_ECX);cleanup=0
  if a==0x9bf5a0:
   assert self==BASE+0xb00 and get(sp+4)==BASE+12;event(1,self,(get(sp+4),));cleanup=4;swap()
  elif a==BASE+0x31000:
   self=get(sp+4);args=tuple(get(sp+8+i*4)for i in range(4));assert self in (BASE+0x900,BASE+0x940)and args==(0,0,0,0);event(2,self,args);cleanup=20;swap()
  elif a==BASE+0x31100:
   self=get(sp+4);assert get(sp+8)==0;event(3,self,(0,));cleanup=8;swap()
  elif a==BASE+0x31200:
   self=get(sp+4);stage=get(sp+8);assert stage<4 and get(sp+12)==0;event(4,self,(stage,0));cleanup=12;swap()
  elif a==BASE+0x31300:
   assert get(sp+4)==1;cleanup=4
   kind=5 if BASE+0x300<=self<BASE+0x400 else 6 if BASE+0x500<=self<BASE+0x600 else 7
   assert kind!=7 or BASE+0x700<=self<BASE+0x800;event(kind,self,(1,));put(self+4,get(self+4)+1)
   if mode&2:
    if kind==5:put(BASE+0x16c,get(BASE+0x168))
    elif kind==6:put(BASE+0x150,get(BASE+0x14c))
    elif self==BASE+0x700:put(BASE+0xa84,0)
  else:
   assert self==BASE;event(8,self);u.mem_write(BASE+4,b'\0');u.mem_write(BASE+0x84,b'\x7f')
  u.reg_write(UC_X86_REG_EAX,0x80004005);u.reg_write(UC_X86_REG_ESP,sp+4+cleanup);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook)
 for off,target in [(100*4,0x31000),(104*4,0x31100),(65*4,0x31200)]:put(BASE+0x30000+off,BASE+target)
 put(BASE+0x32000,BASE+0x31300)
 for active,mask,pattern,mode in itertools.product(range(2),range(16),range(4),range(4)):
  fill=(0,1,0x55,0xff)[mask%4];arena=bytearray([fill])*WIDTH
  def field(off,value):struct.pack_into('<I',arena,off,value&0xffffffff)
  arena[0x84]=active;count=(0,1,3,5)[pattern]
  for off,array,obj in [(0x168,0xa00,0x300),(0x14c,0xa40,0x500),(0x15c,0xa80,0x700)]:
   field(off,BASE+array);field(off+4,BASE+array+count*4);field(off+8,BASE+array+32)
   for i in range(5):field(array+i*4,0 if pattern>=2 and i%3==1 else BASE+obj+i*32);field(obj+i*32,BASE+0x32000)
  for renderer,device in [(0x1000,0x900),(0x5000,0x940)]:
   field(renderer+0x3cf0,BASE+device);field(device,BASE+0x30000);field(renderer+0x3cf4,BASE+0xc00);field(renderer+0x3cb8,(-1,0,2,5)[pattern])
   for i in range(4):field(renderer+0x3d00+i*4,BASE+0xd00+i*32 if mask&(1<<i)else 0)
  globals=(BASE+0x1000,BASE+0xb00,BASE+0xf00,BASE+0xf40)if active else (0,0,0x12345678,0x87654321)
  inputs.extend(struct.pack('<5I',mode,*globals));inputs.extend(arena);u.mem_write(BASE,bytes(arena))
  for a,value in zip(GLOBALS,globals):put(a,value)
  sp=BASE+0x3e000;stop=BASE+0x3f000;put(sp,stop);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,BASE);u.emu_start(0xb25d80,stop,count=10000);assert u.reg_read(UC_X86_REG_ESP)==sp+4;event(9,BASE);cases+=1
 return inputs,expected,dict(cases=cases,snapshots=snapshots)
