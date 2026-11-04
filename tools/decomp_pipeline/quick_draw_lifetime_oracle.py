"""Execute native and rebuilt draw-info destruction with mutating callbacks."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ENTRY=0x9c2f50;OBJECT=0x2000000;ITEMS=OBJECT+0x1000;REPLACEMENT=OBJECT+0x2000;STACK=0x3000000;STOP=0x2200000

def execute(code,count,mode,fill):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in [(0x990000,0x300000),(OBJECT,0x10000),(STACK,0x10000),(STOP,0x1000)]:u.mem_map(a,n)
 def put(a,v):u.mem_write(a,struct.pack('<I',v))
 def get(a):return int.from_bytes(u.mem_read(a,4),'little')
 u.mem_write(ENTRY,bytes(code));u.mem_write(ITEMS,bytes([fill])*300)
 start=0 if count==-1 else ITEMS
 put(OBJECT,start);put(OBJECT+4,start+max(count,0)*60);put(OBJECT+8,0 if count==-1 else ITEMS+300)
 events=[]
 def hook(u,a,size,data):
  if a not in (0x9f9f70,0x99b510,0xbfea14):return
  sp=u.reg_read(UC_X86_REG_ESP);this=u.reg_read(UC_X86_REG_ECX)
  if a==0xbfea14:events.append(('free',get(sp+4)))
  else:
   index=len(events)//2
   offset=24 if a==0x9f9f70 else 20
   assert this==ITEMS+index*60+offset
   events.append(('texture' if offset==24 else 'string',this))
   u.mem_write(this,b'\0'*(8 if offset==24 else 4))
   if len(events)==1 and mode:put(OBJECT,REPLACEMENT if mode==1 else 0);put(OBJECT+4,REPLACEMENT)
  u.reg_write(UC_X86_REG_ESP,sp+4);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,OBJECT)
 saved=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(saved):u.reg_write(r,0x11223000+i)
 u.emu_start(ENTRY,STOP,count=1000)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+4
 assert all(u.reg_read(r)==0x11223000+i for i,r in enumerate(saved))
 expected=[]
 for i in range(max(count,0)):expected.extend([('texture',ITEMS+i*60+24),('string',ITEMS+i*60+20)])
 free_pointer=(REPLACEMENT if mode==1 else 0) if count>0 and mode else start
 if free_pointer:expected.append(('free',free_pointer))
 assert events==expected
 return events,bytes(u.mem_read(OBJECT,12)),bytes(u.mem_read(ITEMS,300))

def compare(candidate,native):
 scenarios=0
 for count in [-1,0,1,2,4]:
  for mode in range(3):
   for fill in [0,1,0x55,0x80,0xff]:
    assert execute(candidate,count,mode,fill)==execute(native,count,mode,fill)
    scenarios+=1
 return scenarios
