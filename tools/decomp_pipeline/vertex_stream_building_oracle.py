"""Execute stream-format construction, append, and size queries against retail."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
OBJECT=0x2000000;DATA=OBJECT+0x1000;GROW=OBJECT+0x2000;STOP=0x2200000;STACK=0x3000000
SIZES=[4,8,12,16,4,4,8,4,2,4,6,8,4,4]

def execute(codes,entry,types=(),cap=0,index=0,data_type=0):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,size in [(0xab0000,0x10000),(0x990000,0x10000),(0xbfe000,0x1000),(OBJECT,0x10000),(STOP,0x1000),(STACK,0x10000)]:u.mem_map(a,size)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return int.from_bytes(u.mem_read(a,4),'little')
 for a,b in codes.items():u.mem_write(a,bytes(b))
 begin=DATA if cap else 0;used=len(types);put(OBJECT,0x129d578);put(OBJECT+4,begin);put(OBJECT+8,begin+used*8);put(OBJECT+12,begin+cap*8)
 for i,t in enumerate(types):put(DATA+8*i,100+i);put(DATA+8*i+4,t)
 u.mem_write(DATA+used*8,b'\xcd'*(128-used*8));events=[]
 if entry==0xab37f0:u.mem_write(OBJECT,b'\xab'*16)
 def hook(u,a,size,data):
  if a not in (0x99a2d0,0xbfea0e,0xbfea14):return
  sp=u.reg_read(UC_X86_REG_ESP);result=0
  if a==0x99a2d0:
   assert u.reg_read(UC_X86_REG_ECX)==OBJECT;events.append(('base',));put(OBJECT,0x1231710);result=OBJECT
  elif a==0xbfea0e:events.append(('malloc',get(sp+4)));result=GROW
  else:events.append(('free',get(sp+4)))
  u.reg_write(UC_X86_REG_EAX,result);u.reg_write(UC_X86_REG_ESP,sp+4);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP);put(sp+4,index);put(sp+8,data_type)
 u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,data_type if entry==0xab3740 else OBJECT)
 saved=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(saved):u.reg_write(r,0x557700+i)
 u.emu_start(entry,STOP,count=10000)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+(12 if entry==0xab3810 else 4)
 assert all(u.reg_read(r)==0x557700+i for i,r in enumerate(saved))
 result=u.reg_read(UC_X86_REG_EAX)
 if entry==0xab3740:assert result==(SIZES[data_type] if 0<=data_type<14 else 0)
 elif entry==0xab37b0:assert result==sum(SIZES[t] if 0<=t<14 else 0 for t in types)
 elif entry==0xab37f0:
  assert result==OBJECT and bytes(u.mem_read(OBJECT,16))==struct.pack('<4I',0x129d578,0,0,0) and events==[('base',)]
 else:
  grow=used==cap;first=GROW if grow else begin;capacity=used+max(used,1) if grow else cap
  assert (get(OBJECT+4),get(OBJECT+8),get(OBJECT+12))==(first,first+(used+1)*8,first+capacity*8)
  expected=b''.join(struct.pack('<II',100+i,t&0xffffffff) for i,t in enumerate(types))+struct.pack('<II',index,data_type&0xffffffff)
  assert bytes(u.mem_read(first,len(expected)))==expected
  assert events==([('malloc',capacity*8)]+([('free',begin)] if begin else []) if grow else [])
  result=None # void
 return result,events,bytes(u.mem_read(OBJECT,16)),bytes(u.mem_read(DATA,128)),bytes(u.mem_read(GROW,128))

def cases():
 yield 0xab37f0,(),0,0,0
 for t in list(range(256))+[-1,-2,0x7fffffff,0x80000000,0xffffffff]:yield 0xab3740,(),0,0,t
 for count in range(10):
  for seed in range(18):
   types=tuple((seed+i)%18 for i in range(count))
   yield 0xab37b0,types,count,0,0
   for cap in sorted({count,count+1,count+3}):
    for index in [0,255,0xffffffff]:yield 0xab3810,types,cap,index,seed

def compare(candidate,native):
 count=0
 for case in cases():
  assert execute(candidate,*case)==execute(native,*case),case
  count+=1
 return count
