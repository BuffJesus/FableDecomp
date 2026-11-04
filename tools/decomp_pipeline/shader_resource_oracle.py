"""Native shader-resource callbacks, including live array replacement and resizing."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
CTOR=0xa5d870;CLOSE=0xa5d750;UNINIT=0xa5d7a0
OBJECT=0x2000000;FIRST=OBJECT+0x1000;SECOND=OBJECT+0x2000;SHADERS=OBJECT+0x3000;VTABLE=OBJECT+0x4000;DELETE=0x2100000;STOP=0x2200000;STACK=0x3000000

def execute(codes,entry,mask,count,mode,fill):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in [(0x990000,0x140000),(OBJECT,0x10000),(DELETE,0x1000),(STOP,0x1000),(STACK,0x10000)]:u.mem_map(a,n)
 def put(a,v):u.mem_write(a,struct.pack('<I',v))
 def get(a):return int.from_bytes(u.mem_read(a,4),'little')
 for a,code in codes.items():u.mem_write(a,bytes(code))
 u.mem_write(OBJECT,bytes([fill])*0x3c)
 for i in range(8):put(SHADERS+i*16,VTABLE)
 put(VTABLE,DELETE)
 for i in range(4):put(FIRST+4*i,SHADERS+i*16 if mask&(1<<i) else 0);put(SECOND+4*i,SHADERS+(i+4)*16)
 if entry!=CTOR:put(OBJECT+4,0x1111);put(OBJECT+8,FIRST);put(OBJECT+12,FIRST+count*4);put(OBJECT+20,0x2222)
 events=[]
 def hook(u,a,size,data):
  if a not in (0x99a2f0,0xab3b10,DELETE):return
  sp=u.reg_read(UC_X86_REG_ESP);this=u.reg_read(UC_X86_REG_ECX);popped=4
  if a==0x99a2f0:assert this==OBJECT;events.append(('base',));put(OBJECT,0x129a7c4)
  elif a==0xab3b10:
   assert this==OBJECT+24 and get(OBJECT)==0x129d580;events.append(('input',));u.mem_write(this,b'\x73'*28)
   if mode:put(OBJECT+4,0x9876);put(OBJECT+20,0x1357);put(OBJECT+52,0x4444)
  else:
   assert get(sp+4)==1;popped=8;index=(this-SHADERS)//16;assert 0<=index<8 and all(e!=('delete',index) for e in events)
   events.append(('delete',index))
   if len(events)==1:
    if mode==1:put(OBJECT+8,SECOND);put(OBJECT+12,SECOND+16)
    elif mode==2:put(OBJECT+12,get(OBJECT+8))
    elif mode==3:put(OBJECT+12,get(OBJECT+8)+16)
   put(OBJECT+4,0x9876);put(OBJECT+20,0x1357)
  u.reg_write(UC_X86_REG_ESP,sp+popped);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,OBJECT)
 regs=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(regs):u.reg_write(r,0x556600+i)
 u.emu_start(entry,STOP,count=1000)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+4
 assert all(u.reg_read(r)==0x556600+i for i,r in enumerate(regs))
 if entry==CTOR:
  assert events==[('base',),('input',)] and u.reg_read(UC_X86_REG_EAX)==OBJECT
  assert get(OBJECT+4)==(0x9876 if mode else 0) and get(OBJECT+20)==0 and get(OBJECT+52)==0 and get(OBJECT+56)==0
 else:
  expected=[];i=0;limit=count;swapped=False
  while i<limit:
   if swapped or mask&(1<<i):
    expected.append(('delete',i+4 if swapped else i))
    if len(expected)==1:
     if mode==1:swapped=True;limit=4
     elif mode==2:limit=0
     elif mode==3:limit=4
   i+=1
  assert events==expected and get(OBJECT+4)==0
  assert get(OBJECT+20)==(0 if entry==UNINIT else 0x1357 if events else 0x2222)
 return events,bytes(u.mem_read(OBJECT,60)),bytes(u.mem_read(FIRST,16)),bytes(u.mem_read(SECOND,16))

def compare(candidate,native):
 cases=0
 for fill in [0,1,0x55,0x80,0xff]:
  for mode in [0,1]:
   assert execute(candidate,CTOR,0,0,mode,fill)==execute(native,CTOR,0,0,mode,fill);cases+=1
 for entry in [CLOSE,UNINIT]:
  for mask in range(16):
   for count in range(5):
    for mode in range(4):
     assert execute(candidate,entry,mask,count,mode,0xa5)==execute(native,entry,mask,count,mode,0xa5);cases+=1
 return cases
