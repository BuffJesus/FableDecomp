"""Native implicit destruction, allocation order, live owner pointers and scalar flags."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
DTOR=0xa5eb70;RANGE=0xa5e900;SCALAR=0xab4690
OBJECT=0x2000000;FIRST=OBJECT+0x1000;SECOND=OBJECT+0x2000;DECL=OBJECT+0x3000;ELEMENTS=OBJECT+0x4000;STACK=0x3000000;STOP=0x2200000

def execute(codes,entry,count,mask,declaration,mode,flags):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in [(0x990000,0x300000),(OBJECT,0x10000),(STACK,0x10000),(STOP,0x1000)]:u.mem_map(a,n)
 def put(a,v):u.mem_write(a,struct.pack('<I',v))
 def get(a):return int.from_bytes(u.mem_read(a,4),'little')
 for a,code in codes.items():u.mem_write(a,bytes(code))
 u.mem_write(OBJECT,b'\x55'*28);u.mem_write(FIRST,b'\x6a'*80);u.mem_write(SECOND,b'\x6a'*80)
 put(OBJECT+4,FIRST);put(OBJECT+8,FIRST+count*20);put(OBJECT+12,FIRST+80);put(OBJECT+16,DECL if declaration else 0)
 for i in range(8):
  stream=(FIRST if i<4 else SECOND)+(i%4)*20
  put(stream+8,ELEMENTS+i*16 if mask&(1<<(i%4)) else 0)
 events=[];stream_calls=0
 def hook(u,a,size,data):
  nonlocal stream_calls
  if a not in (0xbfea14,0x99a2e0,0xbfe9bc):return
  sp=u.reg_read(UC_X86_REG_ESP);this=u.reg_read(UC_X86_REG_ECX)
  if a==0xbfea14:
   ptr=get(sp+4);events.append(('free',ptr))
   if ptr==DECL and mode==3:put(OBJECT+4,SECOND);put(OBJECT+8,SECOND+count*20)
  elif a==0xbfe9bc:assert get(sp+4)==OBJECT;events.append(('delete',OBJECT))
  else:
   events.append(('base',this,get(this)))
   if this!=OBJECT:
    stream_calls+=1
    if stream_calls==1 and mode in (1,2) and entry!=RANGE:put(OBJECT+4,SECOND if mode==1 else 0);put(OBJECT+8,SECOND)
    put(this+4,0x77777777)
   put(this,0x1231710)
  u.reg_write(UC_X86_REG_ESP,sp+4);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,OBJECT)
 if entry==RANGE:u.reg_write(UC_X86_REG_ECX,FIRST);u.reg_write(UC_X86_REG_EDX,FIRST+count*20);put(sp+4,OBJECT+0x9000)
 elif entry==SCALAR:put(sp+4,flags)
 regs=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(regs):u.reg_write(r,0x776600+i)
 u.emu_start(entry,STOP,count=3000)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+(8 if entry in (RANGE,SCALAR) else 4)
 assert all(u.reg_read(r)==0x776600+i for i,r in enumerate(regs))
 expected=[];array=SECOND if mode==3 and declaration and entry!=RANGE else FIRST
 if entry!=RANGE and declaration:expected.append(('free',DECL))
 for i in range(count):
  if mask&(1<<i):expected.append(('free',ELEMENTS+(i+(4 if array==SECOND else 0))*16))
  expected.append(('base',array+i*20+4,0x6a6a6a6a))
 if entry!=RANGE:
  freed=(SECOND if mode==1 else 0) if count and mode in (1,2) else array
  if freed:expected.append(('free',freed))
  expected.append(('base',OBJECT,0x55555555))
  if entry==SCALAR:
   assert u.reg_read(UC_X86_REG_EAX)==OBJECT
   if flags&1:expected.append(('delete',OBJECT))
 assert events==expected,(entry,count,mode,events,expected)
 return events,bytes(u.mem_read(OBJECT,28)),bytes(u.mem_read(FIRST,80)),bytes(u.mem_read(SECOND,80))

def compare(candidate,native):
 cases=0
 for entry in [DTOR,RANGE,SCALAR]:
  for count in [0,1,3]:
   for mask in [0,1,5,15]:
    for declaration in [0,1]:
     for mode in range(4):
      for flags in ([0,1,2,3,0xffffffff] if entry==SCALAR else [0]):
       assert execute(candidate,entry,count,mask,declaration,mode,flags)==execute(native,entry,count,mask,declaration,mode,flags)
       cases+=1
 return cases
