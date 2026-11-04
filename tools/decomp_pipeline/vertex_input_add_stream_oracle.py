"""Compare AddStream across explicit vector-copy, growth, and declaration boundaries."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ENTRY=0xab3b40;COPY=0xa5e0a0;OVERFLOW=0xab4540;FREE=0xbfea14;BASE=0x99a2e0;GENERATE=0xab3870
OBJ=0x2000000;STREAMS=OBJ+0x1000;ALT=OBJ+0x2000;FORMAT=OBJ+0x3000;ELEMENTS=OBJ+0x4000;TEMP=OBJ+0x5000;SAVED=OBJ+0x6000;STOP=0x2200000;STACK=0x3000000

def execute(code,used,capacity,length,index,generate,alias):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,size in [(0xab0000,0x10000),(0xa50000,0x10000),(0x990000,0x10000),(0xbfe000,0x1000),(OBJ,0x10000),(STOP,0x1000),(STACK,0x10000)]:u.mem_map(a,size)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return int.from_bytes(u.mem_read(a,4),'little')
 def vector(a,b,n,cap):put(a,b);put(a+4,b+n*8);put(a+8,b+cap*8)
 begin=STREAMS if capacity else 0;put(OBJ+4,begin);put(OBJ+8,begin+used*20);put(OBJ+12,begin+capacity*20)
 for i in range(used):put(STREAMS+i*20,57+i);put(STREAMS+i*20+4,0x129d578)
 source=STREAMS+4 if alias else FORMAT;put(source,0x129d578);vector(source+4,ELEMENTS if length else 0,length,length)
 values=b''.join(struct.pack('<II',0xff+i,i%8) for i in range(length));u.mem_write(ELEMENTS,values)
 u.mem_write(ENTRY,bytes(code));events=[];copies=0
 def hook(u,a,size,data):
  nonlocal copies
  if a not in [COPY,OVERFLOW,FREE,BASE,GENERATE]:return
  sp=u.reg_read(UC_X86_REG_ESP);this=u.reg_read(UC_X86_REG_ECX);pop=0;result=0
  if a==COPY:
   other=get(sp+4);count=(get(other+4)-get(other))//8;assert count==length
   assert bytes(u.mem_read(get(other),count*8))==values if count else True
   dest=(TEMP if copies==0 else SAVED) if count else 0;vector(this,dest,count,count)
   if count:u.mem_write(dest,values)
   events.append(('copy',count));copies+=1;result=this;pop=4
  elif a==OVERFLOW:
   position,value,tag,count,atend=[get(sp+i) for i in [4,8,12,16,20]]
   assert this==OBJ+4 and position==begin+used*20 and count==1 and atend==1 and used==capacity
   assert get(value)==index and get(value+4)==0x129d578 and get(value+8)==(TEMP if length else 0)
   events.append(('grow',used));
   if used:u.mem_write(ALT,bytes(u.mem_read(begin,used*20)))
   put(ALT+used*20,index);put(ALT+used*20+4,0x129d578);vector(ALT+used*20+8,SAVED if length else 0,length,length)
   if length:u.mem_write(SAVED,values)
   put(OBJ+4,ALT);put(OBJ+8,ALT+(used+1)*20);put(OBJ+12,ALT+(used+max(used,1))*20);pop=20
  elif a==FREE:assert get(sp+4)==TEMP;events.append(('free',))
  elif a==BASE:assert get(this)==0x129d578;put(this,0x1231710);events.append(('base',))
  else:assert this==OBJ and events[-1]==('base',);events.append(('generate',));put(OBJ+20,0x1234)
  u.reg_write(UC_X86_REG_EAX,result);u.reg_write(UC_X86_REG_ESP,sp+4+pop);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP);put(sp+4,index);put(sp+8,source);put(sp+12,generate);u.reg_write(UC_X86_REG_ECX,OBJ);u.reg_write(UC_X86_REG_ESP,sp)
 saved=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(saved):u.reg_write(r,0x778800+i)
 u.emu_start(ENTRY,STOP,count=1000)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+16
 assert all(u.reg_read(r)==0x778800+i for i,r in enumerate(saved))
 expected=[('copy',length),('grow',used) if used==capacity else ('copy',length)]+([('free',)] if length else [])+[('base',)]+([('generate',)] if generate else [])
 assert events==expected
 first=get(OBJ+4);assert get(OBJ+8)==first+(used+1)*20 and get(first+used*20)==index
 assert bytes(u.mem_read(get(first+used*20+8),length*8))==values if length else True
 assert bytes(u.mem_read(ELEMENTS,length*8))==values
 return events,bytes(u.mem_read(OBJ,28)),bytes(u.mem_read(STREAMS,100)),bytes(u.mem_read(ALT,100)),bytes(u.mem_read(SAVED,64))

def cases():
 for used in range(4):
  for capacity in range(used,5):
   for length in [0,1,4,7]:
    for index in [0,255,0xffffffff]:
     for generate in range(2):
      for alias in range(2):
       if alias and not used:continue
       yield used,capacity,length,index,generate,alias

def compare(candidate,native):
 count=0
 for case in cases():assert execute(candidate,*case)==execute(native,*case),case;count+=1
 return count
