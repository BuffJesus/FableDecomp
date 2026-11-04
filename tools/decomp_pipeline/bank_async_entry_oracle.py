"""Retail async-entry allocation, alignment and actual virtual cleanup chain."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UcError
from unicorn.x86_const import *
BANK=0x2000000;ENTRY=BANK+0x1000;INFO=BANK+0x1100;RESULT=BANK+0x1200;STOP=0x2200000;STACK=0x3000000
ALIGNS=[1,2,3,4,16,4096,0xffffffff];SIZES=[0,1,2,3,15,16,17,0xffffffff,0xfffffffe]
def execute(codes,align,size,index,fail,mutation):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in [(0x990000,0x50000),(0xbfe000,0x1000),(0x129b000,0x1000),(BANK,0x10000),(STOP,0x1000),(STACK,0x10000)]:u.mem_map(a,n)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return int.from_bytes(u.mem_read(a,4),'little')
 for a,b in codes.items():u.mem_write(a,bytes(b))
 put(0x129b880,0x9d6fa0);put(BANK+0x98,align);put(BANK+0x200+0x98,5);events=[];allocations=0
 def hook(u,a,length,data):
  nonlocal allocations
  if a not in (0xbfea1a,0xbfe9bc,0x99a2f0,0x99a300,0x9cc240):return
  sp=u.reg_read(UC_X86_REG_ESP);ecx=u.reg_read(UC_X86_REG_ECX);pop=0
  if a==0xbfea1a:
   allocations+=1;assert get(sp+4)==(16 if allocations==1 else 12);events.append(('new',get(sp+4)))
   u.reg_write(UC_X86_REG_EAX,0 if allocations==fail else ENTRY if allocations==1 else INFO)
  elif a==0xbfe9bc:events.append(('delete',get(sp+4)))
  elif a in (0x99a2f0,0x99a300):assert ecx==ENTRY;events.append(('base_ctor' if a==0x99a2f0 else 'base_dtor',));u.reg_write(UC_X86_REG_EAX,ecx)
  else:
   assert ecx==BANK and get(sp+4)==index and get(ENTRY+4)==BANK and get(ENTRY+8)==index
   events.append(('size',index));pop=4;u.reg_write(UC_X86_REG_EAX,size)
   if mutation:put(ENTRY+4,BANK+0x200)
  u.reg_write(UC_X86_REG_ESP,sp+4+pop);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;regs=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(regs):u.reg_write(r,0x887700+i)
 put(sp,STOP);put(sp+4,RESULT);put(sp+8,index);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,BANK)
 u.emu_start(0x9d58d0,STOP,count=500)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+12 and u.reg_read(UC_X86_REG_EAX)==RESULT
 assert all(u.reg_read(r)==0x887700+i for i,r in enumerate(regs))
 assert get(RESULT)==(0 if fail==1 else ENTRY) and get(RESULT+4)==(INFO if fail==0 else 0)
 if fail!=1:
  actual_align=5 if mutation else align;expected=(((size+actual_align-1)&0xffffffff)//actual_align)*actual_align
  assert [get(ENTRY+4*i) for i in range(4)]==[0x129b880,BANK+0x200 if mutation else BANK,index,expected]
  if fail==0:assert [get(INFO+4*i) for i in range(3)]==[1,0x9d6a60,ENTRY]
  put(sp,STOP);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,ENTRY);u.emu_start(0x9d6a60,STOP,count=100)
  assert u.reg_read(UC_X86_REG_ESP)==sp+4 and events[-2:]==[('base_dtor',),('delete',ENTRY)]
 return events,bytes(u.mem_read(ENTRY,16)),bytes(u.mem_read(INFO,12)),bytes(u.mem_read(RESULT,8))
def compare(candidate,native):
 cases=0
 for align in ALIGNS:
  for size in SIZES:
   for index in [0,7,0xffffffff]:
    for fail in range(3):
     for mutation in range(2):
      assert execute(candidate,align,size,index,fail,mutation)==execute(native,align,size,index,fail,mutation);cases+=1
 return cases
