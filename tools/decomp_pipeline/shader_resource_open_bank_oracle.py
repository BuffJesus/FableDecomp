"""Compare bank binding, cached sizing and clearing across observable call boundaries."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ENTRY=0xa5d7c0;MAXIMUM=0x9cc350;RESIZE=0xa5e6a0;OBJECT=0x2000000;BANK=OBJECT+0x1000;OLD=OBJECT+0x2000;ALT=OBJECT+0x3000;STOP=0x2200000;STACK=0x3000000

def execute(code,used,maximum,mode,fill):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in [(0xa50000,0x10000),(0x9c0000,0x10000),(OBJECT,0x10000),(STOP,0x1000),(STACK,0x10000)]:u.mem_map(a,n)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return int.from_bytes(u.mem_read(a,4),'little')
 u.mem_write(ENTRY,bytes(code));u.mem_write(OBJECT,bytes([fill])*60);put(OBJECT+8,OLD);put(OBJECT+12,OLD+used*3*4);put(OBJECT+16,OLD+48)
 for i in range(12):put(OLD+i*4,0x22222222)
 for i in range(16):put(ALT+i*4,0x55555555)
 before=bytes(u.mem_read(OBJECT,60));events=[]
 def hook(u,a,n,data):
  if a not in (MAXIMUM,RESIZE):return
  sp=u.reg_read(UC_X86_REG_ESP);this=u.reg_read(UC_X86_REG_ECX);popped=4
  if a==MAXIMUM:
   assert not events and this==BANK and get(OBJECT+4)==BANK;events.append(('maximum',this))
   if mode==1:put(OBJECT+4,0x12345678)
   u.reg_write(UC_X86_REG_EAX,maximum&0xffffffff)
  else:
   count=get(sp+4);value=get(get(sp+8));assert len(events)==1 and this==OBJECT+8 and count==maximum+1 and value==0;events.append(('resize',this,count,value))
   put(OBJECT+8,ALT);put(OBJECT+12,ALT+(0 if mode==2 else count*4));put(OBJECT+16,ALT+64)
   if mode==3 and not count:put(OBJECT+8,0);put(OBJECT+12,0);put(OBJECT+16,0)
   popped=12
  u.reg_write(UC_X86_REG_ESP,sp+popped);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP);put(sp+4,BANK);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,OBJECT)
 regs=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(regs):u.reg_write(r,0x554400+i)
 u.emu_start(ENTRY,STOP,count=1000)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+8
 assert all(u.reg_read(r)==0x554400+i for i,r in enumerate(regs))
 assert get(OBJECT+4)==(0x12345678 if mode==1 else BANK)
 assert [get(ALT+i*4) for i in range(16)]==[0 if i<maximum+1 else 0x55555555 for i in range(16)]
 after=bytes(u.mem_read(OBJECT,60));assert after[:4]==before[:4] and after[20:]==before[20:]
 assert [get(OLD+i*4) for i in range(12)]==[0x22222222]*12
 return events,after,bytes(u.mem_read(ALT,64))

def compare(candidate,native):
 cases=0
 for used in range(3):
  for maximum in [-1,0,1,4,8]:
   for mode in range(4):
    for fill in [0,0x55,0xa5]:
     assert execute(candidate,used,maximum,mode,fill)==execute(native,used,maximum,mode,fill)
     cases+=1
 return cases
