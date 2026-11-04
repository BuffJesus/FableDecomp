"""Native counted font-manager reset with allocation and release callback mutation."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
OBJECT=0x2000000;BLOCKS=OBJECT+0x100;DATA=OBJECT+0x1000;CALLBACK=0x2100000;STOP=0x2200000;STACK=0x3000000
COUNTS=[0,0,1,2,0xffffffff]
def execute(codes,old,incoming,fail,mode):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in [(0x9a6000,0x2000),(0xbfe000,0x1000),(OBJECT,0x10000),(CALLBACK,0x1000),(STOP,0x1000),(STACK,0x10000)]:u.mem_map(a,n)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return int.from_bytes(u.mem_read(a,4),'little')
 for a,b in codes.items():u.mem_write(a,bytes(b))
 for i,count in enumerate([COUNTS[old],17,55]):put(BLOCKS+16*i,count)
 put(BLOCKS+4,CALLBACK);put(BLOCKS+8,DATA);put(OBJECT,DATA);put(OBJECT+4,BLOCKS if old else 0)
 nextdata=0 if incoming==0 else DATA if incoming==1 else DATA+64;events=[]
 def hook(u,a,size,data):
  if a not in (CALLBACK,0xbfe9bc,0xbfea1a):return
  sp=u.reg_read(UC_X86_REG_ESP)
  if a==CALLBACK:
   assert u.reg_read(UC_X86_REG_ECX)==DATA;events.append(('release',))
   if mode==1:put(OBJECT+4,BLOCKS+16)
   if mode==2:put(OBJECT+4,0)
   put(OBJECT,DATA+96)
  elif a==0xbfe9bc:events.append(('delete',get(sp+4)))
  else:
   assert get(sp+4)==12;events.append(('new',12));u.reg_write(UC_X86_REG_EAX,0 if fail else BLOCKS+32)
  u.reg_write(UC_X86_REG_ESP,sp+4);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP);put(sp+4,nextdata);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,OBJECT)
 regs=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(regs):u.reg_write(r,0x887700+i)
 u.emu_start(0x9a7340,STOP,count=200)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+8
 assert all(u.reg_read(r)==0x887700+i for i,r in enumerate(regs))
 assert get(OBJECT)==nextdata and get(OBJECT+4)==(BLOCKS+32 if incoming and not fail else 0)
 if old:assert get(BLOCKS)==(COUNTS[old]-1)&0xffffffff
 if incoming and not fail:assert [get(BLOCKS+32+i*4) for i in range(3)]==[1,0x9a6c10,nextdata]
 expected=[('release',),('delete',BLOCKS+16 if mode==1 else 0 if mode==2 else BLOCKS)] if old==2 else []
 if incoming:expected.append(('new',12))
 assert events==expected
 return events,bytes(u.mem_read(OBJECT,8)),bytes(u.mem_read(BLOCKS,48))
def compare(candidate,native):
 cases=0
 for old in range(5):
  for incoming in range(3):
   for fail in range(2):
    for mode in range(3):
     assert execute(candidate,old,incoming,fail,mode)==execute(native,old,incoming,fail,mode);cases+=1
 return cases
