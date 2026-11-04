import struct,itertools
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32
from unicorn.x86_const import *
MEM=0x2000000;DEST=MEM;SOURCE=MEM+0x20;INFO=(0,MEM+0x100,MEM+0x200);DATA=(0,MEM+0x300,MEM+0x340);STACK=0x3000000;STOP=0x4000000
def execute(code,address,data,info,count1,count2,alias):
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(address&~0xfff,0x1000);u.mem_map(MEM,0x1000);u.mem_map(STACK,0x10000);u.mem_map(STOP,0x1000)
 def put(a,x):u.mem_write(a,struct.pack('<I',x&0xffffffff))
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 u.mem_write(address,bytes(code));u.mem_write(MEM,b'\xa5'*0x1000)
 for p,c in zip(INFO[1:],(count1,count2)):put(p,c)
 source=DEST if alias else SOURCE;put(source,DATA[data]);put(source+4,INFO[info]);before=bytearray(u.mem_read(MEM,0x400));expected=bytearray(before)
 struct.pack_into('<II',expected,0,DATA[data],INFO[info])
 if info:struct.pack_into('<I',expected,INFO[info]-MEM,(count1 if info==1 else count2)+1 & 0xffffffff)
 sp=STACK+0x8000;put(sp,STOP);put(sp+4,source);u.reg_write(UC_X86_REG_ECX,DEST);u.reg_write(UC_X86_REG_ESP,sp)
 regs=(UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP)
 for i,r in enumerate(regs):u.reg_write(r,0x123400+i)
 u.emu_start(address,STOP,count=100)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+8 and u.reg_read(UC_X86_REG_EAX)==DEST
 assert all(u.reg_read(r)==0x123400+i for i,r in enumerate(regs))
 assert bytes(u.mem_read(MEM,0x400))==expected
 return get(DEST),get(DEST+4),get(INFO[1]),get(INFO[2])
def compare(candidate,native):
 cases=0
 for addr in candidate:
  for data,info,c1,c2,alias in itertools.product(range(3),range(3),(0,1,2,0xffffffff),(0,1,2,0xffffffff),range(2)):
   assert execute(candidate[addr],addr,data,info,c1,c2,alias)==execute(native[addr],addr,data,info,c1,c2,alias)
   cases+=1
 return cases
