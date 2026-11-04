import struct,binascii
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32
from unicorn.x86_const import *
M=0x2000000;D=M+0x100;CRC=M+0x400;SYM=M+0x1000;ENT=M+0x1100;STR=M+0x2000;WORDS=M+0x3000;Q=M+0x4000;STOP=0x2200000;STACK=0x3000000
def crc(word):return (binascii.crc32(struct.pack('<I',word),0xffffffff)^0xffffffff)&0xffffffff
def value(i,v):return 0 if v==0 else i*13+7 if v==1 else -i*17-1
def execute(codes,table,n,q,v,route):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,z in [(0x400000,0x2000),(0x9ad000,0x1000),(0x9b2000,0x1000),(0x9e5000,0x3000),(0x129a000,0x1000),(M,0x10000),(STOP,0x1000),(STACK,0x10000)]:u.mem_map(a,z)
 def put(a,x):u.mem_write(a,struct.pack('<I',x&0xffffffff))
 for a,b in codes.items():u.mem_write(a,bytes(b))
 u.mem_write(0x129a168,table)
 put(D+0x60,SYM);put(D+0x68,CRC);put(D+0x6c,CRC+(n*8 if route!=1 else 0));put(D+0x70,CRC+256)
 rows=sorted((crc(i+(20 if route==2 else 0)),value(i,v)) for i in range(n))
 for i,(k,val) in enumerate(rows):put(CRC+8*i,k);put(CRC+8*i+4,val)
 put(SYM+4,ENT);put(SYM+8,ENT+n*16);put(SYM+12,ENT+256)
 for i in range(n):
  put(ENT+i*16,STR+i*16);put(ENT+i*16+12,value(i,v));put(STR+i*16,WORDS+i*4);put(STR+i*16+4,4);put(WORDS+i*4,i)
 put(Q,Q+16 if q else 0);put(Q+16,Q+32);put(Q+20,0 if q==1 else 4);put(Q+32,q-2)
 expected=value(q-2,v)&0xffffffff if q>=2 and q-2<n and route!=2 else 0
 before=bytes(u.mem_read(M,0x4040));sp=STACK+0x8000;put(sp,STOP);put(sp+4,Q);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,D)
 regs=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(regs):u.reg_write(r,0x889900+i)
 u.emu_start(0x9ad410,STOP,count=2000)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+8
 assert all(u.reg_read(r)==0x889900+i for i,r in enumerate(regs))
 result=u.reg_read(UC_X86_REG_EAX);assert result==expected,(n,q,v,route,result,expected)
 assert before==bytes(u.mem_read(M,0x4040))
 return result
def compare(candidate,native,table):
 cases=0
 for n in range(17):
  for q in range(20):
   for v in range(3):
    for route in range(3):assert execute(candidate,table,n,q,v,route)==execute(native,table,n,q,v,route);cases+=1
 return cases
