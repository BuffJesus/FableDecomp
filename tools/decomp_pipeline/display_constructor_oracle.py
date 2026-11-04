"""Native display constructor, with typed initialization/growth call boundaries."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ENTRY=0x9c0880;BASE=0x99a2f0;WIDE=0x99aed0;BUSY=0xa61880;MALLOC=0xbfea0e;PIXEL=0x9e3830;PIXEL_GROW=0x9c2e30;SAMPLE_GROW=0x9c2ca0
OBJECT=0x2000000;HEAP=OBJECT+0x2000;SYSTEM=OBJECT+0x1000;STACK=0x3000000;STOP=0x2200000

def execute(code,busy_code,fill,growth,mutation):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in [(0x9c0000,0x4000),(0x99a000,0x1000),(0x9e3000,0x1000),(0xa61000,0x1000),(0xbfe000,0x1000),(OBJECT,0x10000),(STACK,0x10000),(STOP,0x1000)]:u.mem_map(a,n)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return int.from_bytes(u.mem_read(a,4),'little')
 u.mem_write(ENTRY,bytes(code));u.mem_write(BUSY,bytes(busy_code));u.mem_write(OBJECT,bytes([fill])*0x2c8);u.mem_write(STACK,b'\xcc'*0x10000)
 events=[];allocations=[];next_heap=HEAP
 def allocate(n):
  nonlocal next_heap
  result=next_heap;next_heap+=(n+15)&~15;assert next_heap<OBJECT+0x10000;u.mem_write(result,b'\xa5'*n);allocations.append((result,n));return result
 def hook(u,a,size,data):
  if a==BUSY:events.append(('busy',u.reg_read(UC_X86_REG_ECX)-OBJECT));return
  if a not in [BASE,WIDE,MALLOC,PIXEL,PIXEL_GROW,SAMPLE_GROW]:return
  sp=u.reg_read(UC_X86_REG_ESP);this=u.reg_read(UC_X86_REG_ECX);argc=0;value=this
  if a==BASE:assert this==OBJECT;put(this,0x129a7c4);events.append(('base',))
  elif a==WIDE:assert this==OBJECT+0x258;put(this,0);events.append(('wide',))
  elif a==MALLOC:
   n=get(sp+4);assert n in [12,16];value=allocate(n);events.append(('malloc',n))
  elif a==PIXEL:
   argc=1;fmt=get(sp+4);put(this,fmt^0x55);events.append(('pixel',fmt))
   if mutation:put(OBJECT+0x2c4,0x3f000000+mutation)
  else:
   argc=5;offset=0x298 if a==PIXEL_GROW else 0x2a4;assert this==OBJECT+offset
   start=get(this);finish=get(this+4);end=get(this+8);assert get(sp+4)==finish and finish==end
   item=get(get(sp+8));assert get(sp+16)==1 and get(sp+20)&255==1
   count=(finish-start)//4;capacity=count+1 if growth==0 else max(2*count,1) if growth==1 else count+5
   new=allocate(capacity*4)
   if count:u.mem_write(new,bytes(u.mem_read(start,count*4)))
   put(new+count*4,item);put(this,new);put(this+4,new+(count+1)*4);put(this+8,new+capacity*4)
   events.append(('grow',offset,item,count,capacity))
  u.reg_write(UC_X86_REG_EAX,value);u.reg_write(UC_X86_REG_ESP,sp+4+argc*4);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP);put(sp+4,SYSTEM);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,OBJECT)
 u.emu_start(ENTRY,STOP,count=50000);assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+8 and u.reg_read(UC_X86_REG_EAX)==OBJECT
 return bytes(u.mem_read(OBJECT,0x2c8)),events,[(a,bytes(u.mem_read(a,n))) for a,n in allocations]
