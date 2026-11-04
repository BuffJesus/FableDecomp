"""Compare complete declaration fill-insert chains, including aliasing and storage growth."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
OBJECT=0x2000000;DATA=OBJECT+0x1000;GROW=OBJECT+0x2000;VALUE=OBJECT+0x3000;STOP=0x2200000;STACK=0x3000000

def execute(codes,used,cap,pos,n,alias,value):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,size in [(0xab0000,0x10000),(0xbfe000,0x1000),(OBJECT,0x10000),(STOP,0x1000),(STACK,0x10000)]:u.mem_map(a,size)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return int.from_bytes(u.mem_read(a,4),'little')
 def put_element(a,v):u.mem_write(a,struct.pack('<Q',v))
 def element(a):return int.from_bytes(u.mem_read(a,8),'little')
 for a,c in codes.items():u.mem_write(a,bytes(c))
 begin=DATA if cap else 0;put(OBJECT,begin);put(OBJECT+4,begin+used*8);put(OBJECT+8,begin+cap*8)
 for i in range(cap):put_element(DATA+i*8,0x80000000+i*16 if i<used else 0xaaaaaaaa)
 pvalue=DATA+(used//2)*8 if alias else VALUE;put_element(pvalue,value);original=[element(DATA+i*8) for i in range(used)];expected=original[:pos]+[value]*n+original[pos:];events=[]
 def hook(u,a,size,data):
  if a not in (0xbfea0e,0xbfea14,0xbfeae6):return
  sp=u.reg_read(UC_X86_REG_ESP);result=0
  if a==0xbfea0e:events.append(('malloc',get(sp+4)));result=GROW
  elif a==0xbfea14:events.append(('free',get(sp+4)))
  else:
   dest,source,count=get(sp+4),get(sp+8),get(sp+12);events.append(('move',dest,source,count));result=dest
   if count:u.mem_write(dest,bytes(u.mem_read(source,count)))
  u.reg_write(UC_X86_REG_EAX,result);u.reg_write(UC_X86_REG_ESP,sp+4);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP);put(sp+4,begin+pos*8);put(sp+8,n);put(sp+12,pvalue);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,OBJECT)
 regs=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(regs):u.reg_write(r,0x554400+i)
 u.emu_start(0xab4260,STOP,count=10000)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+16
 assert all(u.reg_read(r)==0x554400+i for i,r in enumerate(regs))
 grow=n>cap-used;capacity=used+max(used,n) if grow else cap;first=GROW if grow else begin
 assert (get(OBJECT),get(OBJECT+4),get(OBJECT+8))==(first,first+(used+n)*8,first+capacity*8)
 assert [element(first+i*8) for i in range(used+n)]==expected
 if not grow:assert [element(first+i*8) for i in range(used+n,cap)]==[0xaaaaaaaa]*(cap-used-n)
 assert [e for e in events if e[0]=='malloc']==([('malloc',capacity*8)] if grow else [])
 assert [e for e in events if e[0]=='free']==([('free',begin)] if grow and begin else [])
 if n==0:assert not events
 return events,bytes(u.mem_read(OBJECT,12)),bytes(u.mem_read(DATA,128)),bytes(u.mem_read(GROW,128)),element(VALUE)

def compare(candidate,native):
 cases=0
 for used in range(5):
  for cap in range(used,8):
   for pos in range(used+1):
    for n in range(6):
     for alias in range(2):
      if alias and not used:continue
      for value in [0,0x1122334455667788,0xffffffffffffffff]:
       assert execute(candidate,used,cap,pos,n,alias,value)==execute(native,used,cap,pos,n,alias,value)
       cases+=1
 return cases
