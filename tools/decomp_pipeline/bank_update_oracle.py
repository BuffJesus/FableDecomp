"""Compare bank update policies with real packed-reader and no emulated calls."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32
from unicorn.x86_const import *
BASE=0x2000000;RUNTIME=BASE+0x1000;UPDATES=BASE+0x2000;POINTERS=BASE+0x3000;WORDS=BASE+0x4000;STACK=0x3000000;STOP=0x4000000
def valid(i,p):return p==0 or p==1 and i%2!=0 or p==2 and i%3==0
def execute(codes,n,short,packed,pattern,prior):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,z in [(0x9c0000,0x10000),(0xa50000,0x20000),(0x1397000,0x1000),(BASE,0x10000),(STACK,0x10000),(STOP,0x1000)]:u.mem_map(a,z)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def byte(a,v):u.mem_write(a,bytes([v]))
 for a,b in codes.items():u.mem_write(a,bytes(b))
 for i in range(33):put(0x13979e8+i*4,(1<<i)-1)
 u.mem_write(BASE,b'\xa5'*0x190);u.mem_write(UPDATES,b'\x6b'*0x200);count=n//2 if short else n
 put(BASE+4,n);put(BASE+0x20,RUNTIME);put(BASE+0x24,RUNTIME+count*12);put(BASE+0x2c,POINTERS);put(BASE+0x68,WORDS);put(BASE+0x6c,int(bool(packed)));byte(BASE+0x70,1 if packed==1 else 32);put(BASE+0x74,0)
 flags=[bool(prior)]*16;bits=0
 for i in range(16):
  byte(RUNTIME+i*12+9,int(valid(i,pattern)));put(POINTERS+i*4,UPDATES+i*32);byte(UPDATES+i*32+20,prior)
  if i:
   if packed==1:bits|=int(valid(i,2-pattern))<<(i-1)
   else:put(WORDS+(i-1)*4,0x80000000 if valid(i,2-pattern) else 0)
 if packed==1:put(WORDS,bits)
 before=bytes(u.mem_read(BASE,0x190));other=bytes(u.mem_read(UPDATES,0x200));trace=[];regs=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 def call(address,args,expected,boolean=False):
  sp=STACK+0x8000;put(sp,STOP)
  for j,arg in enumerate(args):put(sp+4+j*4,arg)
  u.reg_write(UC_X86_REG_ECX,BASE);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_EAX,0x87654321)
  for j,r in enumerate(regs):u.reg_write(r,0x778800+j)
  u.emu_start(address,STOP,count=10000);assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+4+4*len(args);assert all(u.reg_read(r)==0x778800+j for j,r in enumerate(regs))
  value=u.reg_read(UC_X86_REG_EAX)&(255 if boolean else 0xffffffff);assert value==expected,(hex(address),value,expected,n,short,packed,pattern,prior);trace.append(value)
  assert bytes(u.mem_read(BASE,0x190))==before
 for index in [0,1,n,n+1,0xffffffff]:
  for requested in range(2):
   call(0x9cc190,[index],int(bool(index and index<n and valid(index,2-pattern if packed else pattern))),True)
   allowed=index<count and valid(index,pattern);call(0x9cc300,[index,requested],int(flags[index] if allowed else False),True)
   if allowed:flags[index]=bool(requested)
   assert all(u.mem_read(UPDATES+i*32+20,1)[0]==flags[i] for i in range(16))
   for address,args,expected,boolean in [(0x9cba70,[],0,False),(0x9cbc40,[index,n],1,True),(0xa63340,[],105,False),(0xa63ac0,[index,n],0,True),(0xa5ee90,[],109,False),(0xa5f9c0,[index,n],0,True)]:call(address,args,expected,boolean)
 flags=[bool(prior)]*16
 for i in range(16):byte(UPDATES+i*32+20,prior)
 for repeat in range(2):
  expected=0
  for i in range(1,n):
   if valid(i,2-pattern if packed else pattern):
    allowed=i<count and valid(i,pattern)
    if not allowed or not flags[i]:expected+=1
    if allowed:flags[i]=True
  call(0xa5eea0,[],expected);assert all(u.mem_read(UPDATES+i*32+20,1)[0]==flags[i] for i in range(16))
 after=bytes(u.mem_read(UPDATES,0x200));assert all(a==b for i,(a,b) in enumerate(zip(other,after)) if i%32!=20)
 return trace,after
def compare(candidate,native):
 cases=0
 for n in range(13):
  for short in range(2):
   for packed in range(3):
    for pattern in range(3):
     for prior in range(2):
      assert execute(candidate,n,short,packed,pattern,prior)==execute(native,n,short,packed,pattern,prior);cases+=1
 return cases
