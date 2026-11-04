"""Original runtime-data resize/fill/growth replay; full arena at CRT boundaries."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from pe_oracle import EXE,pe_sections
BASE=0x24000000;WIDTH=0x4000

def scenarios():
 for n in range(7):
  for capacity,newsize,alias,mode in itertools.product(sorted(set([n,12])),range(10),[0,1],range(4)):
   if alias and n==0:continue
   yield(0,n,capacity,newsize,0,alias,0,mode)
  for capacity,pos,fill,alias,mode in itertools.product(sorted(set([n,12])),range(n+1),[0,1,3],[0,1],range(4)):
   if alias and n==0:continue
   yield(1,n,capacity,pos,fill,alias,0,mode)
  for pos,fill,alias,atend,mode in itertools.product(range(n+1),[0,1,3],[0,1],[0,1],range(4)):
   if alias and n==0:continue
   yield(3,n,n,pos,fill,alias,atend,mode)
 for n in range(9):
  for first in range(n+1):
   for last,mode in itertools.product(range(first,n+1),range(4)):yield(2,n,12,first,last,0,0,mode)
 for count,first,last,mode in itertools.product(range(9),[0,1,3],[0,1,3],range(4)):yield(4,count,12,first,last,0,0,mode)

def collect(overrides=None, compact=False):
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000);u.mem_map(BASE,0x10000);raw=EXE.read_bytes()
 for _,va,_,off,n in pe_sections(raw):
  if n:u.mem_write(0x400000+va,raw[off:off+n])
 for address,body in (overrides or {}).items():u.mem_write(address,body)
 inputs=bytearray();expected=bytearray();cases=snapshots=0;heap=0;mutated=False
 def get(a):return int.from_bytes(u.mem_read(a,4),'little')
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def event(kind,a=0,b=0,c=0):
  nonlocal snapshots
  expected.extend(struct.pack('<4I',kind,a,b,c));expected.extend(u.mem_read(BASE,WIDTH));snapshots+=1
 def hook(u,a,n,_):
  nonlocal heap,mutated
  if a not in [0xbfea0e,0xbfeae6,0xbfea14]:return
  sp=u.reg_read(UC_X86_REG_ESP);result=0
  if a==0xbfea0e:
   size=get(sp+4);assert 0<size<=384;event(1,size);result=BASE+0x1000+heap*0x100;heap+=1
   if mode==1:put(BASE+0x900,get(BASE+0x900)^0x101)
   elif mode==2:put(BASE+8,BASE+0x3040)
  elif a==0xbfeae6:
   dest,source,size=[get(sp+4+i*4)for i in range(3)];assert size<=384;event(2,dest,source,size)
   if mode==1 and size:put(source,get(source)^0x100)
   if mode==2 and not mutated:put(BASE+4,get(BASE+4)+4);mutated=True
   if size:u.mem_write(dest,bytes(u.mem_read(source,size)))
   result=dest
  else:
   pointer=get(sp+4);event(3,pointer)
   if mode==3:put(BASE,BASE+0x3000);put(BASE+4,BASE+0x3004);put(BASE+8,BASE+0x3040)
  u.reg_write(UC_X86_REG_EAX,result);u.reg_write(UC_X86_REG_ESP,sp+4);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook)
 for op,count,capacity,first,last,alias,atend,mode in (s for s in scenarios() if s[0] in (0,1,3)):
  if compact and (count>3 or mode!=0):continue
  arena=bytearray((i*17+cases*13)&255 for i in range(WIDTH));start=BASE+0x200 if capacity else 0
  struct.pack_into('<3I',arena,0,start,start+count*12,start+capacity*12);struct.pack_into('<I',arena,0x900,0xabcdef98)
  inputs.extend(struct.pack('<8I',op,count,capacity,first,last,alias,atend,mode));inputs.extend(arena);u.mem_write(BASE,bytes(arena));heap=0;mutated=False
  value=BASE+0x200 if alias else BASE+0x900
  if op==0:args=[first,value]
  elif op==1:args=[start+first*12,last,value]
  elif op==2:args=[start+first*12,start+last*12]
  elif op==3:args=[start+first*12,value,BASE+0x940,last,atend]
  else:args=[BASE+0x200+last*12,BASE+0x940]
  sp=BASE+0xe000;stop=BASE+0xf000;u.mem_write(sp,struct.pack('<'+'I'*(len(args)+1),stop,*args));u.reg_write(UC_X86_REG_ESP,sp)
  u.reg_write(UC_X86_REG_ECX,BASE if op!=4 else BASE+0x200+first*12);u.reg_write(UC_X86_REG_EDX,BASE+0x200+(first+count)*4)
  u.emu_start([0x9d4010,0x9d3a20,0,0x9d3580,0][op],stop,count=100000);assert u.reg_read(UC_X86_REG_ESP)==sp+4+len(args)*4
  result=u.reg_read(UC_X86_REG_EAX) if op in [2,4] else 0;event(4,op,result);cases+=1
 return inputs,expected,dict(cases=cases,snapshots=snapshots)
