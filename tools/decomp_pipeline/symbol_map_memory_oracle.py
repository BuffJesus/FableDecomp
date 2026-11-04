"""Compare symbol-map storage release including actual copy/destruction helpers."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
OBJECT=0x2000000;DATA=OBJECT+0x1000;STOP=0x2200000;STACK=0x3000000

def execute(codes,count,spare,pattern,null):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in [(0x6fb000,0x1000),(0x990000,0x10000),(0x9e0000,0x10000),(0xbfe000,0x1000),(OBJECT,0x10000),(STOP,0x1000),(STACK,0x10000)]:u.mem_map(a,n)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return int.from_bytes(u.mem_read(a,4),'little')
 for a,b in codes.items():u.mem_write(a,bytes(b))
 u.mem_write(OBJECT,b'\x9d'*16);u.mem_write(OBJECT+13,bytes([pattern&1]));u.mem_write(DATA,bytes([0x20+pattern])*(24*16))
 start=0 if null else DATA;put(OBJECT,start);put(OBJECT+4,start+count*16);put(OBJECT+8,start+(count+spare)*16);tail=bytes(u.mem_read(OBJECT+12,4));events=[]
 def hook(u,a,size,data):
  if a not in (0x99eae0,0x99efb0,0xbfea14):return
  assert a!=0x99efb0,'empty copy must not assign strings'
  sp=u.reg_read(UC_X86_REG_ESP)
  if a==0x99eae0:address=u.reg_read(UC_X86_REG_ECX);events.append(('destroy',address));put(address,0)
  else:events.append(('free',get(sp+4)))
  u.reg_write(UC_X86_REG_ESP,sp+4);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,OBJECT)
 regs=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(regs):u.reg_write(r,0x887700+i)
 u.emu_start(0x9e7680,STOP,count=3000)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+4 and bytes(u.mem_read(OBJECT+12,4))==tail
 assert all(u.reg_read(r)==0x887700+i for i,r in enumerate(regs))
 expected=[('destroy',DATA+i*16+off) for i in range(count) for off in [8,4,0]]+[('free',start)] if count+spare else []
 assert events==expected and [get(OBJECT+i*4) for i in range(3)]==([0]*3 if count+spare else [start]*3)
 return events,bytes(u.mem_read(OBJECT,16)),bytes(u.mem_read(DATA,24*16))

def compare(candidate,native):
 cases=0
 for count in range(17):
  for spare in range(4):
   for pattern in range(4):
    for null in range(2 if count==0 and spare==0 else 1):
     assert execute(candidate,count,spare,pattern,null)==execute(native,count,spare,pattern,null);cases+=1
 return cases
