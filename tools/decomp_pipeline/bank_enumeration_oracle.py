"""Exercise real enumeration methods with only string-construction boundaries."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
BASE=0x2000000;STACK=0x3000000;STOP=0x4000000
ADDRESSES=[0x9cbd10,0xa63380,0xa5ef10,0x9c79e0]
def execute(codes,kind,typ,text,owners):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,z in [(0x9c0000,0x10000),(0xa50000,0x20000),(0x99e000,0x1000),(0x129b000,0x1000),(0x122d000,0x1000),(BASE,0x10000),(STACK,0x10000),(STOP,0x1000)]:u.mem_map(a,z)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 for a,b in codes.items():u.mem_write(a,bytes(b))
 for a,b in [(0x129b464,b'EGameText\0'),(0x129b458,b'ETextGroup\0'),(0x122d70e,b'\0')]:u.mem_write(a,b)
 for a in [0x99ec30,0x99ebf0]:u.mem_write(a,b'\xc3')
 ref=BASE+0x1000 if text else 0;out=BASE+0x800;put(BASE+0xf0,ref);put(BASE+0x1004,owners);before=bytes(u.mem_read(BASE,0x214));trace=[]
 def hook(uc,a,size,data):
  sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX)
  if a==0x9cbd10:trace.append(('base',this,get(sp+8)))
  if a not in (0x99ec30,0x99ebf0):return
  assert this==out
  if a==0x99ec30:
   assert get(sp+4)==BASE+0xf0;put(out,ref)
   if ref:put(ref+4,get(ref+4)+1)
   trace.append(('copy',ref));pop=4
  else:
   target=0x129b464 if typ==0 else 0x129b458 if typ==1 else 0x122d70e
   assert get(sp+4)==target and get(sp+8)==0xffffffff;put(out,BASE+0x1100);trace.append(('literal',target));pop=8
  uc.reg_write(UC_X86_REG_EAX,this);uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP);put(sp+4,out);put(sp+8,typ);u.reg_write(UC_X86_REG_ECX,BASE);u.reg_write(UC_X86_REG_ESP,sp)
 regs=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(regs):u.reg_write(r,0x887700+i)
 u.emu_start(ADDRESSES[kind],STOP,count=200)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+12 and u.reg_read(UC_X86_REG_EAX)==out
 assert all(u.reg_read(r)==0x887700+i for i,r in enumerate(regs));assert before==bytes(u.mem_read(BASE,0x214))
 assert get(BASE+0x1004)==((owners+1)&0xffffffff if ref and kind!=3 else owners)
 assert get(out)==(BASE+0x1100 if kind==3 else ref)
 if kind!=3:assert trace==[('base',BASE,typ),('copy',ref)]
 return trace,get(out),get(BASE+0x1004)
def compare(candidate,native):
 count=0
 for kind in range(4):
  for typ in [0,1,2,3,127,255,0x80000000,0xffffffff]:
   for text in range(5):
    for owners in [0,1,7,0xffffffff]:
     assert execute(candidate,kind,typ,text,owners)==execute(native,kind,typ,text,owners);count+=1
 return count
