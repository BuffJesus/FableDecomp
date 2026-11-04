"""Native InitEntry CRC calculation and unsorted pair-vector growth."""
import struct,hashlib,itertools
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
BASE=0x2000000;HEAP=0x2100000;STOP=0x2200000;STACK=0x3000000
def execute(codes,count,capacity,pattern,nulls):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in ((0x400000,0x10000),(0x420000,0x10000),(0x990000,0x50000),(0x129a000,0x1000),(0xbfe000,0x1000),(BASE,0x10000),(HEAP,0x10000),(STOP,0x1000),(STACK,0x10000)):u.mem_map(a,n)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return int.from_bytes(u.mem_read(a,4),'little')
 for a,body in codes.items():u.mem_write(a,body)
 boundaries=(0xbfea0e,0xbfea14,0x42d131,0x9cbe10,0x99ed10,0x99ec30,0x99eae0,0x9d34e0)
 for a in boundaries:u.mem_write(a,b'\xc3')
 bank=BASE+0x100;query=BASE+0x500;data=BASE+0x600;text=BASE+0x800;vector=bank+0xd0;put(bank+0x78,256);u.mem_write(HEAP,b'\x7d'*0x10000)
 allocated={};freed=set();events=[]
 def allocate(n):
  assert 0<n<992;result=HEAP+1024*len(allocated)+16;assert result+n+16<HEAP+0x10000;allocated[result]=n;events.append(('allocate',n,result));return result
 def free(p):
  if p:
   assert p in allocated and p not in freed;n=allocated[p];assert bytes(u.mem_read(p-16,16))==bytes(u.mem_read(p+n,16))==b'\x7d'*16;freed.add(p);events.append(('free',p))
 def hook(u,a,size,user):
  if a not in boundaries:return
  sp=u.reg_read(UC_X86_REG_ESP);arg=get(sp+4)
  if a==0xbfea0e:result=allocate(arg)
  elif a==0xbfea14:free(arg);result=0
  else:raise AssertionError(('unexpected branch',hex(a)))
  u.reg_write(UC_X86_REG_EAX,result);u.reg_write(UC_X86_REG_ESP,sp+4);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook)
 if capacity:
  start=allocate(capacity*8);put(vector,start);put(vector+4,start);put(vector+8,start+capacity*8)
 expected=[];lengths=(0,1,2,3,7,15,31,64);sp=STACK+0x8000;registers=(UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP)
 for i in range(count):
  length=lengths[(i+pattern)%8];raw=bytes((0 if pattern==0 else j if pattern==1 else (j*71+i*19)^0xa5 if pattern==2 else 255-j)&255 for j in range(65));u.mem_write(text,raw);put(data,text);put(data+4,length);put(data+8,65);u.mem_write(data+12,b'\x01');put(data+13,1);put(query,0 if nulls and i&1 else data)
  crc=0
  for byte in raw[:length if get(query) else 0]:
   crc^=byte
   for bit in range(8):crc=(crc>>1)^(0xedb88320 if crc&1 else 0)
  payload=0xffffffff-i;expected.append((crc,payload));put(sp,STOP);put(sp+4,payload);put(sp+8,query);put(sp+12,query)
  for j,r in enumerate(registers):u.reg_write(r,0x556600+j)
  u.reg_write(UC_X86_REG_ECX,bank);u.reg_write(UC_X86_REG_ESP,sp);u.emu_start(0x9ce050,STOP,count=100000)
  assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+16 and all(u.reg_read(r)==0x556600+j for j,r in enumerate(registers))
  if i==capacity:capacity=max(1,i*2)
  start=get(vector);finish=get(vector+4);end=get(vector+8)
  assert finish-start==(i+1)*8 and end-start==capacity*8 and bytes(u.mem_read(bank+0xdd,1))==b'\x01'
  assert [(get(start+j*8),get(start+j*8+4)) for j in range(i+1)]==expected
  assert bytes(u.mem_read(text,65))==raw and get(data)==text and get(data+4)==length and get(data+13)==1
 assert bytes(u.mem_read(bank+0xdd,1))==bytes([int(count!=0)])
 free(get(vector));assert set(allocated)==freed
 return dict(events=events,expected=expected,arena=hashlib.sha256(u.mem_read(BASE,0x10000)).hexdigest(),heap=hashlib.sha256(u.mem_read(HEAP,0x10000)).hexdigest())
def compare(candidate,original):
 cases=0
 for args in itertools.product(range(33),(0,1,7,33),range(4),range(2)):
  assert execute(candidate,*args)==execute(original,*args),args;cases+=1
 return cases
