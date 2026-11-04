"""Compare complete declaration assignment chains with each compiler's private ABI."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
OBJECT=0x2000000;SOURCE=OBJECT+0x100;OLD=OBJECT+0x1000;SRC=OBJECT+0x2000;GROW=OBJECT+0x3000;STOP=0x2200000;STACK=0x3000000

def execute(codes,self,used,cap,n,fill,entry=0xa5e2e0):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,size in [(0xa50000,0x10000),(0xbfe000,0x1000),(OBJECT,0x10000),(STOP,0x1000),(STACK,0x10000)]:u.mem_map(a,size)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return int.from_bytes(u.mem_read(a,4),'little')
 for a,c in codes.items():u.mem_write(a,bytes(c))
 begin=OLD if cap else 0;src=SRC if n else 0
 for at,first,count,capacity in [(OBJECT,begin,used,cap),(SOURCE,src,n,n)]:put(at,first);put(at+4,first+count*8);put(at+8,first+capacity*8)
 oldbytes=bytes((0x11+fill+i*13)&255 for i in range(cap*8));srcbytes=bytes((0x55+fill+i*19)&255 for i in range(n*8))
 u.mem_write(OLD,oldbytes);u.mem_write(SRC,srcbytes);u.mem_write(GROW,b'\xa5'*80);events=[]
 def hook(u,a,size,data):
  if a not in (0xbfea0e,0xbfea14):return
  sp=u.reg_read(UC_X86_REG_ESP);events.append((a,get(sp+4)));u.reg_write(UC_X86_REG_EAX,GROW if a==0xbfea0e else 0);u.reg_write(UC_X86_REG_ESP,sp+4);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP);put(sp+4,OBJECT if self else SOURCE);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,OBJECT)
 regs=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(regs):u.reg_write(r,0x554400+i)
 u.emu_start(entry,STOP,count=10000)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+8 and u.reg_read(UC_X86_REG_EAX)==OBJECT
 assert all(u.reg_read(r)==0x554400+i for i,r in enumerate(regs))
 grow=not self and n>cap;size=used if self else n;capacity=n if grow else cap;first=GROW if grow else begin
 assert (get(OBJECT),get(OBJECT+4),get(OBJECT+8))==(first,first+size*8,first+capacity*8)
 if size:assert bytes(u.mem_read(first,size*8))==(oldbytes[:used*8] if self else srcbytes)
 if not grow and capacity>size:assert bytes(u.mem_read(first+size*8,(capacity-size)*8))==oldbytes[size*8:]
 assert events==([(0xbfea0e,n*8)]+([(0xbfea14,OLD)] if cap else []) if grow else [])
 assert bytes(u.mem_read(SRC,n*8))==srcbytes
 return events,bytes(u.mem_read(OBJECT,12)),bytes(u.mem_read(OLD,80)),bytes(u.mem_read(SRC,80)),bytes(u.mem_read(GROW,80))

def compare(candidate,native):
 cases=0
 for self in range(2):
  for used in range(5):
   for cap in range(used,7):
    for n in range(8):
     for fill in range(3):
      assert execute(candidate,self,used,cap,n,fill)==execute(native,self,used,cap,n,fill)
      cases+=1
 return cases
