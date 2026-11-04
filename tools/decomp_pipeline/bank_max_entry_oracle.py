"""Compare bank maximum-entry scans, packed callbacks and signed-size edge cases."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ENTRY=0x9cc350;PACKED=0xa629f0;OBJECT=0x2000000;DATA=OBJECT+0x1000;REPLACEMENT=OBJECT+0x2000;STOP=0x2200000;STACK=0x3000000

def execute(code,size,valid,packed,byte,mode):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in [(0x9c0000,0x10000),(0xa60000,0x10000),(OBJECT,0x10000),(STOP,0x1000),(STACK,0x10000)]:u.mem_map(a,n)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return int.from_bytes(u.mem_read(a,4),'little')
 u.mem_write(ENTRY,bytes(code));u.mem_write(OBJECT,b'\xa5'*0x110);put(OBJECT+4,size);put(OBJECT+0x20,DATA);put(OBJECT+0x6c,packed)
 if valid<17:u.mem_write(DATA+valid*12+9,bytes([byte]))
 if mode:u.mem_write(DATA+2*12+9,b'\x01');u.mem_write(REPLACEMENT+3*12+9,b'\x01')
 before=bytes(u.mem_read(OBJECT,0x110));events=[]
 def hook(u,a,n,data):
  if a!=PACKED:return
  sp=u.reg_read(UC_X86_REG_ESP);index=get(sp+4);assert u.reg_read(UC_X86_REG_ECX)==OBJECT+0x68;events.append(index)
  if mode==1:put(OBJECT+4,0)
  if mode==2:put(OBJECT+0x6c,0)
  if mode==3:put(OBJECT+0x6c,0);put(OBJECT+0x20,REPLACEMENT)
  u.reg_write(UC_X86_REG_EAX,0xffffffff if index+1==valid else 0);u.reg_write(UC_X86_REG_ESP,sp+8);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,OBJECT)
 regs=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(regs):u.reg_write(r,0x554400+i)
 u.emu_start(ENTRY,STOP,count=10000)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+4
 assert all(u.reg_read(r)==0x554400+i for i,r in enumerate(regs))
 expected=0xffffffff
 if size>=0x80000001:expected=size-1
 elif mode:expected=0xffffffff if mode==1 else 2 if mode==2 else 3
 elif valid>0 and valid<size:expected=valid
 assert u.reg_read(UC_X86_REG_EAX)==expected
 if mode:assert events==[4]
 elif size>=0x80000001:assert not events
 else:
  count=(size-valid if expected!=0xffffffff else size-1) if packed and size>1 else 0
  assert events==list(range(size-2,size-2-count,-1))
 if not mode:assert bytes(u.mem_read(OBJECT,0x110))==before
 return u.reg_read(UC_X86_REG_EAX),events,bytes(u.mem_read(OBJECT,0x110))

def compare(candidate,native):
 cases=[]
 for size in range(17):
  for valid in range(17):
   for packed in range(2):
    for byte in [1,0x7f,0xff]:cases.append((size,valid,packed,byte,0))
 cases.extend((6,0,6,1,mode) for mode in range(1,4))
 cases.extend((size,0,0,1,0) for size in [0x80000001,0x80000002,0xffffffff]);cases.append((0x80000000,0x7fffffff,1,1,0))
 for args in cases:assert execute(candidate,*args)==execute(native,*args)
 return len(cases)
