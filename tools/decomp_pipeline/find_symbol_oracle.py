"""Compare real lower-bound, value lookup and existence checks with no call stubs."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32
from unicorn.x86_const import *
MAP=0x2000000;ENTRIES=MAP+0x100;STRINGS=MAP+0x1000;WORDS=MAP+0x2000;QUERY=MAP+0x3000;STOP=0x2200000;STACK=0x3000000

def get_output(u,addr):return struct.unpack('<I',u.mem_read(addr,4))[0]

def execute(codes,n,q,alias,variant):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,z in [(0x9e5000,0x3000),(MAP,0x10000),(STOP,0x1000),(STACK,0x10000)]:u.mem_map(a,z)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 for a,b in codes.items():u.mem_write(a,bytes(b))
 put(MAP+4,ENTRIES);put(MAP+8,ENTRIES+n*16);put(MAP+12,ENTRIES+256)
 values=[];keys=[]
 for i in range(n):
  key=(i-1)//2+1;value=0 if variant==0 else i*13+7 if variant==1 else -i*17-1;values.append(value);keys.append(key);put(ENTRIES+16*i,STRINGS+32*i if i else 0);put(ENTRIES+16*i+12,value);put(STRINGS+32*i,WORDS+i*4);put(STRINGS+32*i+4,4);put(WORDS+i*4,key)
 wanted=0
 while wanted<n and q and (not wanted or keys[wanted]<q-1):wanted+=1
 found=wanted<n and ((not wanted) if not q else wanted!=0 and keys[wanted]==q-1)
 put(QUERY,QUERY+16 if q else 0);put(QUERY+16,QUERY+32);put(QUERY+20,4);put(QUERY+32,q-1)
 if alias and found:put(QUERY,STRINGS+32*wanted if wanted else 0)
 OUTPUT=MAP+0x4000;put(OUTPUT,0x12345678)
 results=[];before=bytes(u.mem_read(MAP,0x3040));sp=STACK+0x8000;regs=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for address,this,expected,mask in [(0x9e6910,MAP+4,ENTRIES+wanted*16,0xffffffff),(0x9e51c0,MAP,int(found),255),(0x9e5120,MAP,values[wanted]&0xffffffff if found else 0,0xffffffff),(0x9e5170,MAP,int(found),255)]:
  put(sp,STOP);put(sp+4,QUERY);put(sp+8,OUTPUT);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,this)
  for i,r in enumerate(regs):u.reg_write(r,0x889900+i)
  u.emu_start(address,STOP,count=1000);assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+(12 if address==0x9e5170 else 8);assert all(u.reg_read(r)==0x889900+i for i,r in enumerate(regs));result=u.reg_read(UC_X86_REG_EAX)&mask;assert result==expected;results.append(result);assert get_output(u,OUTPUT)==(values[wanted]&0xffffffff if found else 0x12345678) if address==0x9e5170 else True;assert before==bytes(u.mem_read(MAP,0x3040))
 return results

def compare(candidate,native):
 cases=0
 for n in range(17):
  for q in range(20):
   for alias in range(2):
    for variant in range(3):assert execute(candidate,n,q,alias,variant)==execute(native,n,q,alias,variant);cases+=1
 return cases
