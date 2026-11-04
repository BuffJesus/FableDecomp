"""Compare resource input binding with observable vector-assignment boundaries."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ENTRY=0xa5d8b0;OBJECT=0x2000000;INPUT=OBJECT+0x1000;STREAMS=OBJECT+0x2000;DECL=OBJECT+0x3000;ALT=OBJECT+0x4000;STOP=0x2200000;STACK=0x3000000

def execute(code,fill,self,stream_mode,declaration_mode,count):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,size in [(0xa50000,0x10000),(OBJECT,0x10000),(STOP,0x1000),(STACK,0x10000)]:u.mem_map(a,size)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return int.from_bytes(u.mem_read(a,4),'little')
 u.mem_write(ENTRY,bytes(code));u.mem_write(OBJECT,bytes([fill])*60);u.mem_write(INPUT,b'\x66'*28)
 source=OBJECT+24 if self else INPUT;put(source+16,DECL);put(source+20,DECL+count*8);put(source+24,DECL+count*8)
 before=bytes(u.mem_read(OBJECT,60));events=[]
 def hook(u,a,size,data):
  if a not in (0xa5e9c0,0xa5e2e0):return
  sp=u.reg_read(UC_X86_REG_ESP);this=u.reg_read(UC_X86_REG_ECX);other=get(sp+4);events.append((a,this,other))
  if a==0xa5e9c0:
   assert len(events)==1 and this==OBJECT+28 and other==source+4
   if stream_mode==1:put(source+16,ALT);put(source+20,ALT+count*8)
   if stream_mode==2:put(OBJECT+40,DECL);put(OBJECT+44,DECL+count*8)
   put(this,STREAMS);put(this+4,STREAMS+20);put(this+8,STREAMS+40)
  else:
   assert len(events)==2 and this==OBJECT+40 and other==source+16
   first=get(other)
   if declaration_mode==1:first=ALT
   if declaration_mode==2:first=0
   finish=first+(0 if declaration_mode>=2 else count*8)
   put(this,first);put(this+4,finish);put(this+8,finish)
  u.reg_write(UC_X86_REG_EAX,this);u.reg_write(UC_X86_REG_ESP,sp+8);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP);put(sp+4,source);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,OBJECT)
 regs=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(regs):u.reg_write(r,0x554400+i)
 u.emu_start(ENTRY,STOP,count=1000)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+8
 assert all(u.reg_read(r)==0x554400+i for i,r in enumerate(regs))
 after=bytes(u.mem_read(OBJECT,60));assert after[:28]==before[:28] and after[56:]==before[56:]
 assert get(OBJECT+52)==(get(OBJECT+40) if count and declaration_mode<2 else OBJECT+40)
 return events,after,bytes(u.mem_read(INPUT,28))

def compare(candidate,native):
 cases=0
 for fill in [0,1,0x55,0xa5]:
  for self in range(2):
   for stream_mode in range(3):
    for declaration_mode in range(4):
     for count in range(3):
      assert execute(candidate,fill,self,stream_mode,declaration_mode,count)==execute(native,fill,self,stream_mode,declaration_mode,count)
      cases+=1
 return cases
