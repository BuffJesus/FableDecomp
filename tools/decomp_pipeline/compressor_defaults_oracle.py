"""Execute CACompressor's default virtuals against retail: each pops its arguments and touches nothing."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32
from unicorn.x86_const import *
OBJ=0x2000000;STACK=0x3000000;STOP=0x4000000
# address -> stack argument count (this in ecx)
ENTRIES={0x52add0:8,0x52ade0:5,0x52adf0:5,0x52ae00:1}
def run(codes,entry,args):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,z in [(0x52a000,0x1000),(OBJ,0x1000),(STACK,0x10000),(STOP,0x1000)]:u.mem_map(a,z)
 for a,b in codes.items():u.mem_write(a,bytes(b))
 u.mem_write(OBJ,bytes(i*7&0xff for i in range(0x1000)))
 sp=STACK+0x8000;u.mem_write(sp,struct.pack('<I',STOP)+b''.join(struct.pack('<I',v) for v in args)+b'\x5a'*16)
 regs=[UC_X86_REG_EAX,UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP,UC_X86_REG_EDX]
 for i,r in enumerate(regs):u.reg_write(r,0x778800+i)
 u.reg_write(UC_X86_REG_ECX,OBJ);u.reg_write(UC_X86_REG_ESP,sp)
 u.emu_start(entry,STOP,count=100)
 return u.reg_read(UC_X86_REG_ESP)-sp,[u.reg_read(r) for r in regs],bytes(u.mem_read(OBJ,0x1000)),bytes(u.mem_read(sp+4,4*len(args)+16))
def compare(candidate,native):
 cases=0
 for entry,argc in ENTRIES.items():
  for seed in range(4):
   args=[(0x1000*seed+i)*0x10001&0xffffffff for i in range(argc)]
   c=run(candidate,entry,args);assert c==run(native,entry,args),hex(entry)
   pop,regs,obj,stack=c
   assert pop==4+4*argc and regs[1:5]==[0x778801,0x778802,0x778803,0x778804]
   assert obj==bytes(i*7&0xff for i in range(0x1000)) and stack==b''.join(struct.pack('<I',v) for v in args)+b'\x5a'*16
   cases+=1
 return cases
