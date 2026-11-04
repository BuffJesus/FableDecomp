"""Compare shader-bank lifetime with mutating native boundary callbacks."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
OBJECT=0x2000000;OLD=OBJECT+0x1000;NEW=OBJECT+0x2000;STACK=0x3000000;STOP=0x2200000
CTOR=0xa62020;DTOR=0xa620d0

def execute(code,entry,fill,mode,allocated):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in [(0x990000,0x300000),(OBJECT,0x10000),(STACK,0x10000),(STOP,0x1000)]:u.mem_map(a,n)
 def put(a,v):u.mem_write(a,struct.pack('<I',v))
 def get(a):return int.from_bytes(u.mem_read(a,4),'little')
 u.mem_write(entry,bytes(code));u.mem_write(OBJECT,bytes([fill])*0x128)
 start=OLD if allocated else 0
 if entry==DTOR:put(OBJECT+0x110,start);put(OBJECT+0x114,start);put(OBJECT+0x118,0xabc456)
 events=[]
 def hook(u,a,size,data):
  if a not in (0x9cd480,0x9ce180,0xa62050,0xbfea14):return
  sp=u.reg_read(UC_X86_REG_ESP);this=u.reg_read(UC_X86_REG_ECX)
  if a==0x9cd480:
   assert this==OBJECT;events.append(('base_ctor',));u.mem_write(OBJECT,bytes([fill^0x5a])*0x110);u.reg_write(UC_X86_REG_EAX,OBJECT)
  elif a==0xa62050:
   assert this==OBJECT and get(OBJECT)==0x129d644 and get(OBJECT+0x110)==start;events.append(('close',))
   if mode:put(OBJECT+0x110,NEW if mode==1 else 0)
   put(OBJECT+0x114,0xabc123)
  elif a==0xbfea14:
   assert get(OBJECT+0x11c)==0x129c82c;events.append(('free',get(sp+4)))
  else:
   assert this==OBJECT and get(OBJECT+0x11c)==0x129c82c;events.append(('base_dtor',));u.mem_write(OBJECT,b'\x37'*0x110)
  u.reg_write(UC_X86_REG_ESP,sp+4);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,OBJECT)
 regs=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(regs):u.reg_write(r,0x445500+i)
 u.emu_start(entry,STOP,count=1000)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+4
 assert all(u.reg_read(r)==0x445500+i for i,r in enumerate(regs))
 if entry==CTOR:
  assert u.reg_read(UC_X86_REG_EAX)==OBJECT and get(OBJECT)==0x129d644 and get(OBJECT+0x11c)==0x129d630
  assert bytes(u.mem_read(OBJECT+0x110,12))==b'\0'*12
  assert events==[('base_ctor',)]
 else:
  freed=start if mode==0 else NEW if mode==1 else 0
  assert events==[('close',)]+([('free',freed)] if freed else [])+[('base_dtor',)]
 assert bytes(u.mem_read(OBJECT+0x120,8))==bytes([fill])*8
 return events,bytes(u.mem_read(OBJECT,0x128))

def compare(candidate,native):
 count=0
 for entry in [CTOR,DTOR]:
  for fill in [0,1,0x55,0x80,0xff]:
   for mode in range(3):
    for allocated in range(2):
     assert execute(candidate[entry],entry,fill,mode,allocated)==execute(native[entry],entry,fill,mode,allocated)
     count+=1
 return count
