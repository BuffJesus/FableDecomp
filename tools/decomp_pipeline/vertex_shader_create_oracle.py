"""Compare shader COM creation, ignored HRESULTs and data-owner allocation."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ENTRY=0xa63020;GET_SYSTEM=0x9a4ec0;BASE_CTOR=0x99a2f0;NEW=0xbfea1a
OBJECT=0x2000000;SYSTEM=OBJECT+0x1000;DISPLAY=OBJECT+0x2000;DEVICE=OBJECT+0x3000;DEVICE2=DEVICE+16;TABLE=OBJECT+0x4000;TABLE2=OBJECT+0x5000;ARRAY=OBJECT+0x6000;CODE=OBJECT+0x7000;DATA=OBJECT+0x8000;SHADER=OBJECT+0x9000;DECL=SHADER+16;NAME=OBJECT+0xa000
CREATE_SHADER=0x2100000;CREATE_DECL=CREATE_SHADER+16;ALT_DECL=CREATE_SHADER+32;STOP=0x2200000;STACK=0x3000000

def execute(code,count,spare,format,mask,failure_result,fail_allocation,mutation,fill):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in [(0xa60000,0x10000),(0x990000,0x20000),(0xbfe000,0x1000),(OBJECT,0x10000),(CREATE_SHADER,0x1000),(STOP,0x1000),(STACK,0x10000)]:u.mem_map(a,n)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return int.from_bytes(u.mem_read(a,4),'little')
 u.mem_write(ENTRY,bytes(code));u.mem_write(OBJECT,bytes([fill])*8);u.mem_write(DATA,b'\x66'*20)
 put(SYSTEM+0x60,DISPLAY);put(DISPLAY+0x58,DEVICE);put(DEVICE,TABLE);put(TABLE+0x16c,CREATE_SHADER);put(TABLE+0x158,CREATE_DECL);put(TABLE2+0x158,ALT_DECL)
 begin=CODE if count+spare else 0;put(ARRAY,begin);put(ARRAY+4,begin+count*4);put(ARRAY+8,begin+(count+spare)*4);elements=0x12345678 if format else 0;events=[]
 def hook(u,a,n,data):
  if a not in (GET_SYSTEM,BASE_CTOR,NEW,CREATE_SHADER,CREATE_DECL,ALT_DECL):return
  sp=u.reg_read(UC_X86_REG_ESP);this=u.reg_read(UC_X86_REG_ECX);popped=4;result=0
  if a==GET_SYSTEM:assert not events and get(OBJECT)==0x129b3d0;events.append(1);result=SYSTEM
  elif a==CREATE_SHADER:
   assert events==[1] and get(sp+4)==DEVICE and get(sp+8)==(CODE if count else ARRAY);events.append(2);put(get(sp+12),SHADER if mask&1 else 0);popped=16;result=0x80004005 if failure_result else 0
   if mutation==1:put(DISPLAY+0x58,DEVICE2)
   if mutation==2:put(DEVICE,TABLE2)
  elif a in (CREATE_DECL,ALT_DECL):
   assert events==[1,2] and get(sp+4)==DEVICE and get(sp+8)==elements;assert (a==ALT_DECL)==(mutation==2);events.append(3);put(get(sp+12),DECL if mask&2 else 0);popped=16;result=0x80004005 if failure_result else 0
  elif a==NEW:assert events==[1,2,3] and get(sp+4)==20;events.append(4);result=0 if fail_allocation else DATA
  else:assert events==[1,2,3,4] and this==DATA;events.append(5);put(DATA,0x129a7c4);result=DATA
  u.reg_write(UC_X86_REG_EAX,result);u.reg_write(UC_X86_REG_ESP,sp+popped);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000
 for i,v in enumerate([STOP,ARRAY,elements,NAME,NAME]):put(sp+4*i,v)
 u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,OBJECT)
 regs=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(regs):u.reg_write(r,0x554400+i)
 u.emu_start(ENTRY,STOP,count=1000)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+20 and u.reg_read(UC_X86_REG_EAX)==OBJECT
 assert all(u.reg_read(r)==0x554400+i for i,r in enumerate(regs))
 assert events==list(range(1,5 if fail_allocation else 6)) and get(OBJECT+4)==(0 if fail_allocation else DATA)
 if not fail_allocation:assert [get(DATA+i*4) for i in range(5)]==[0x129da50,DECL if mask&2 else 0,SHADER if mask&1 else 0,elements,1]
 return events,bytes(u.mem_read(OBJECT,8)),bytes(u.mem_read(DATA,20)),bytes(u.mem_read(DISPLAY,0x60)),get(DEVICE),bytes(u.mem_read(ARRAY,12))

def compare(candidate,native):
 cases=0
 for count in [0,1,5]:
  for spare in range(2):
   for format in range(2):
    for mask in range(4):
     for failure_result in range(2):
      for fail_allocation in range(2):
       for mutation in range(3):
        for fill in [0,0x55,0xa5]:
         assert execute(candidate,count,spare,format,mask,failure_result,fail_allocation,mutation,fill)==execute(native,count,spare,format,mask,failure_result,fail_allocation,mutation,fill)
         cases+=1
 return cases
