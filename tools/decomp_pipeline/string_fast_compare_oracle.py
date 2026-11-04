"""Compare full native fast string ordering, with no emulated call boundaries."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32
from unicorn.x86_const import *
OBJECT=0x2000000;STOP=0x2200000;STACK=0x3000000
def execute(code,n,m,mode):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,z in [(0x9e6000,0x1000),(OBJECT,0x1000),(STOP,0x1000),(STACK,0x10000)]:u.mem_map(a,z)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 a=bytes((i*71+n*13)&255 for i in range(32));b=bytearray(a)
 if mode==4:b[mode%4]^=0x80
 if mode==5:b[16]^=0xff
 if mode==6:b[m]^=0x80
 if mode==7:b[0]^=0xff
 left=0 if mode==0 else OBJECT+32;right=0 if mode==1 else left if mode==2 else OBJECT+64
 put(OBJECT,left);put(OBJECT+4,right);put(OBJECT+32,OBJECT+128);put(OBJECT+36,n);put(OBJECT+64,OBJECT+192);put(OBJECT+68,m);u.mem_write(OBJECT+128,a);u.mem_write(OBJECT+192,bytes(b));u.mem_write(0x9e6530,bytes(code))
 sp=STACK+0x8000;put(sp,STOP);put(sp+4,OBJECT+4);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,OBJECT)
 regs=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(regs):u.reg_write(r,0x765400+i)
 before=bytes(u.mem_read(OBJECT,256));u.emu_start(0x9e6530,STOP,count=200)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+8 and before==bytes(u.mem_read(OBJECT,256))
 assert all(u.reg_read(r)==0x765400+i for i,r in enumerate(regs));result=u.reg_read(UC_X86_REG_EAX)&255
 expected=False
 if left!=right:
  if not left:expected=True
  elif right:
   if n!=m:expected=n<m
   else:expected=[int.from_bytes(a[i:i+4],'little') for i in range(0,n,4)]<[int.from_bytes(b[i:i+4],'little') for i in range(0,m,4)]
 assert result==expected
 return result
def compare(candidate,native):
 cases=0
 for n in range(18):
  for m in range(18):
   for mode in range(8):assert execute(candidate,n,m,mode)==execute(native,n,m,mode);cases+=1
 return cases
