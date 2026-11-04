"""Reference-count assignment with source and destination mutation during release."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ENTRY=0x9de7c0;OBJECT=0x2000000;SOURCE=OBJECT+0x100;BLOCKS=OBJECT+0x1000;CALLBACK=0x2100000;STOP=0x2200000;STACK=0x3000000
COUNTS=[0,0,1,2,0xffffffff]
def execute(code,oldcase,newcase,alias,mode):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in [(0x9d0000,0x10000),(0xbfe000,0x1000),(OBJECT,0x10000),(CALLBACK,0x1000),(STOP,0x1000),(STACK,0x10000)]:u.mem_map(a,n)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return int.from_bytes(u.mem_read(a,4),'little')
 u.mem_write(ENTRY,bytes(code));blocks=[BLOCKS+i*16 for i in range(3)]
 for i,b in enumerate(blocks):put(b,COUNTS[oldcase] if i==0 else COUNTS[newcase] if i==1 else 1);put(b+4,CALLBACK);put(b+8,0x3333)
 old=blocks[0] if oldcase else 0;incoming=old if alias==1 else blocks[1] if newcase else 0
 put(OBJECT,0xaaaa);put(OBJECT+4,old);put(SOURCE,0xbbbb);put(SOURCE+4,incoming)
 source=OBJECT if alias==2 else SOURCE
 if alias==2:incoming=old
 incoming_data=get(source);incoming_count=get(incoming) if incoming else 0
 changed=old!=incoming;released=changed and old and COUNTS[oldcase]==1;events=[]
 def hook(u,a,size,data):
  if a not in (CALLBACK,0xbfe9bc):return
  sp=u.reg_read(UC_X86_REG_ESP)
  if a==CALLBACK:
   assert u.reg_read(UC_X86_REG_ECX)==0x3333;events.append(('release',))
   if mode==1:put(OBJECT+4,blocks[2])
   elif mode==2:put(source,0x7777);put(source+4,blocks[2])
   elif mode==3:put(OBJECT+4,0)
  else:events.append(('delete',get(sp+4)))
  u.reg_write(UC_X86_REG_ESP,sp+4);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP);put(sp+4,source);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,OBJECT)
 regs=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(regs):u.reg_write(r,0x887700+i)
 u.emu_start(ENTRY,STOP,count=500)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+8 and u.reg_read(UC_X86_REG_EAX)==OBJECT
 assert all(u.reg_read(r)==0x887700+i for i,r in enumerate(regs))
 assert get(OBJECT)==(incoming_data if changed else 0xaaaa) and get(OBJECT+4)==incoming
 if incoming:assert get(incoming)==(incoming_count+(1 if changed else 0))&0xffffffff
 expected=[('release',),('delete',blocks[2] if mode==1 else 0 if mode==3 else old)] if released else []
 assert events==expected
 return events,bytes(u.mem_read(OBJECT,8)),bytes(u.mem_read(SOURCE,8)),bytes(u.mem_read(BLOCKS,48))
def compare(candidate,native):
 cases=0
 for old in range(5):
  for new in range(5):
   for alias in range(3):
    for mode in range(4):
     assert execute(candidate,old,new,alias,mode)==execute(native,old,new,alias,mode)
     cases+=1
 return cases
