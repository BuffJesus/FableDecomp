"""Compare streaming font handle initialization with explicit bank/string boundaries."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
BASE=0xa5ef50;OBJECT=0x2000000;BLOCKS=OBJECT+0x1000;DATA=OBJECT+0x2000;CALLBACK=0x2100000;STOP=0x2200000;STACK=0x3000000
COUNTS=[0,0,1,2,0xffffffff]
def execute(code,old,new,alias,mode,index):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in [(0xa5e000,0x2000),(0x990000,0x50000),(0xbfe000,0x1000),(0x129d000,0x1000),(OBJECT,0x10000),(CALLBACK,0x1000),(STOP,0x1000),(STACK,0x10000)]:u.mem_map(a,n)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return int.from_bytes(u.mem_read(a,4),'little')
 u.mem_write(BASE,bytes(code));u.mem_write(0x129d5a4,b'GlyphData\0')
 for i,count in enumerate([COUNTS[old],COUNTS[new],7]):put(BLOCKS+16*i,count);put(BLOCKS+16*i+4,CALLBACK);put(BLOCKS+16*i+8,DATA+8*i)
 previous=BLOCKS if old else 0;incoming=previous if alias else BLOCKS+16 if new else 0
 put(OBJECT+0x188,DATA);put(OBJECT+0x18c,previous);events=[]
 def hook(u,a,size,data):
  if a not in (0x99ebf0,0x99eae0,0x9ccdc0,0x9d58d0,CALLBACK,0xbfe9bc):return
  sp=u.reg_read(UC_X86_REG_ESP);ecx=u.reg_read(UC_X86_REG_ECX);pop=0
  if a==0x99ebf0:
   assert get(sp+4)==0x129d5a4 and get(sp+8)==0xffffffff;events.append(('string',));put(ecx,0x129d5a4);pop=8;u.reg_write(UC_X86_REG_EAX,ecx)
  elif a==0x9ccdc0:
   assert ecx==OBJECT and get(get(sp+4))==0x129d5a4;events.append(('lookup',));pop=4;u.reg_write(UC_X86_REG_EAX,index)
  elif a==0x99eae0:assert get(ecx)==0x129d5a4;events.append(('string_free',));put(ecx,0)
  elif a==0x9d58d0:
   assert ecx==OBJECT and get(sp+8)==index;result=get(sp+4);put(result,DATA+24);put(result+4,incoming);u.reg_write(UC_X86_REG_EAX,result);events.append(('entry',index));pop=8
  elif a==CALLBACK:
   events.append(('release',ecx))
   if ecx==DATA and mode:put(OBJECT+0x18c,BLOCKS+32 if mode==1 else 0)
  else:events.append(('delete',get(sp+4)))
  u.reg_write(UC_X86_REG_ESP,sp+4+pop);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP);put(sp+4,OBJECT+0x3000);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,OBJECT)
 regs=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(regs):u.reg_write(r,0x887700+i)
 u.emu_start(BASE,STOP,count=300)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+8
 assert all(u.reg_read(r)==0x887700+i for i,r in enumerate(regs))
 assert events[:4]==[('string',),('lookup',),('string_free',),('entry',index)]
 return events,bytes(u.mem_read(OBJECT,0x190)),bytes(u.mem_read(BLOCKS,48))
def compare(candidate,native):
 cases=0
 for old in range(5):
  for new in range(5):
   for alias in range(2):
    for mode in range(3):
     for index in [0,19,0xffffffff]:
      assert execute(candidate,old,new,alias,mode,index)==execute(native,old,new,alias,mode,index);cases+=1
 return cases
