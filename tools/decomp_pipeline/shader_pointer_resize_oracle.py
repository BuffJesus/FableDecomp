"""Compare pointer resize and its separately recovered fill-insert boundary."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ENTRY=0xa5e6a0;INSERT=0xa5e3b0;MEMMOVE=0xbfeae6;OBJECT=0x2000000;DATA=OBJECT+0x1000;ALT=OBJECT+0x2000;VALUE=OBJECT+0x3000;STOP=0x2200000;STACK=0x3000000

def execute(code,used,cap,target,alias,value,mode):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in [(0xa50000,0x10000),(0xbfe000,0x1000),(OBJECT,0x10000),(STOP,0x1000),(STACK,0x10000)]:u.mem_map(a,n)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return int.from_bytes(u.mem_read(a,4),'little')
 u.mem_write(ENTRY,bytes(code));begin=DATA if cap else 0;put(OBJECT,begin);put(OBJECT+4,begin+used*4);put(OBJECT+8,begin+cap*4)
 for i in range(16):put(DATA+i*4,0x80000000+i*16)
 pvalue=DATA+(used//2)*4 if alias else VALUE;put(pvalue,value);before=bytes(u.mem_read(DATA,64));events=[]
 def hook(u,a,n,data):
  assert a!=MEMMOVE,'valid resize shrink must not move elements'
  if a!=INSERT:return
  sp=u.reg_read(UC_X86_REG_ESP);assert u.reg_read(UC_X86_REG_ECX)==OBJECT;position=get(sp+4);count=get(sp+8);pointer=get(sp+12);assert (position,count,pointer)==(begin+used*4,target-used,pvalue);events.append((position,count,pointer,get(pointer)))
  if mode:put(OBJECT,ALT);put(OBJECT+4,ALT+4);put(OBJECT+8,ALT+8);put(pointer,0x12345678)
  elif count:
   if count>cap-used:put(OBJECT,ALT);put(OBJECT+8,ALT+target*4)
   put(OBJECT+4,get(OBJECT)+target*4)
  u.reg_write(UC_X86_REG_ESP,sp+16);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP);put(sp+4,target);put(sp+8,pvalue);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,OBJECT)
 regs=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(regs):u.reg_write(r,0x554400+i)
 u.emu_start(ENTRY,STOP,count=1000)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+12
 assert all(u.reg_read(r)==0x554400+i for i,r in enumerate(regs))
 assert len(events)==int(target>=used)
 if target<used:assert (get(OBJECT),get(OBJECT+4),get(OBJECT+8))==(begin,begin+target*4,begin+cap*4)
 elif mode:assert (get(OBJECT),get(OBJECT+4),get(OBJECT+8))==(ALT,ALT+4,ALT+8) and get(pvalue)==0x12345678
 else:
  first=ALT if target>cap else begin;capacity=target if target>cap else cap
  assert (get(OBJECT),get(OBJECT+4),get(OBJECT+8))==(first,first+target*4,first+capacity*4)
 if not mode or target<used:assert bytes(u.mem_read(DATA,64))==before
 return events,bytes(u.mem_read(OBJECT,12)),bytes(u.mem_read(DATA,64)),get(VALUE)

def compare(candidate,native):
 cases=0
 for used in range(5):
  for cap in range(used,7):
   for target in range(8):
    for alias in range(2):
     if alias and not used:continue
     for value in [0,0x55555555,0xffffffff]:
      for mode in range(2):
       assert execute(candidate,used,cap,target,alias,value,mode)==execute(native,used,cap,target,alias,value,mode)
       cases+=1
 return cases
