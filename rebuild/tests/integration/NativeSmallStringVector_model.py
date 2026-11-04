"""Native small string-vector allocation/copy, array cookie and owner lifecycle."""
import struct,hashlib,itertools
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
BASE=0x2000000;HEAP=0x2100000;STOP=0x2200000;STACK=0x3000000
def execute(codes,kind,old,n,mode,self):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,size in ((0x99e000,0x1000),(0x9d2000,0x1000),(0x13bd000,0x1000),(0xbfe000,0x1000),(BASE,0x10000),(HEAP,0x10000),(STOP,0x1000),(STACK,0x10000)):u.mem_map(a,size)
 def get(a):return int.from_bytes(u.mem_read(a,4),'little')
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 for a,body in codes.items():u.mem_write(a,body)
 for a in (0xbfeb22,0xbfeb1c,0xbfe9bc):u.mem_write(a,b'\xc3')
 a=BASE;b=BASE+8;refs=BASE+0x1000;query=BASE+0x100;sp=STACK+0x8000;put(0x13bd800,16);u.mem_write(HEAP,b'\x7d'*0x10000)
 for i in range(16):put(refs+i*32+13,1)
 allocations={};freed=set();events=[]
 def hook(u,pc,size,user):
  if pc not in (0xbfeb22,0xbfeb1c,0xbfe9bc):return
  stack=u.reg_read(UC_X86_REG_ESP);arg=get(stack+4);result=0
  if pc==0xbfeb22:
   assert 4<=arg<=1024;result=HEAP+len(allocations)*2048+16;assert result+arg+16<HEAP+0x10000;allocations[result]=arg;events.append(('new-array',arg,result))
  elif pc==0xbfeb1c:
   assert arg in allocations and arg not in freed;size=allocations[arg];assert get(arg)==(size-4)//4
   assert bytes(u.mem_read(arg-16,16))==bytes(u.mem_read(arg+size,16))==b'\x7d'*16
   assert all(get(arg+i)==0 for i in range(4,size,4));freed.add(arg);events.append(('delete-array',arg))
  else:raise AssertionError('unexpected final anchor release')
  u.reg_write(UC_X86_REG_EAX,result);u.reg_write(UC_X86_REG_ESP,stack+4);u.reg_write(UC_X86_REG_EIP,get(stack))
 u.hook_add(UC_HOOK_CODE,hook);registers=(UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP)
 def invoke(entry,this,args):
  put(sp,STOP)
  for i,value in enumerate(args):put(sp+4+i*4,value)
  for j,r in enumerate(registers):u.reg_write(r,0x778800+j)
  u.reg_write(UC_X86_REG_ECX,this);u.reg_write(UC_X86_REG_ESP,sp);u.emu_start(entry,STOP,count=1000000)
  assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+4+4*len(args) and all(u.reg_read(r)==0x778800+j for j,r in enumerate(registers))
 def populate(vector,count):
  for i in range(count):
   value=0 if mode==0 else refs+(0 if mode==1 else i%16)*32;put(query,value);invoke(0x99efb0,get(vector)+4*i,(query,));assert u.reg_read(UC_X86_REG_EAX)==get(vector)+4*i
 def verify_owners(vectors):
  owners=[1]*16;total=16
  for vector,count in vectors:
   assert bytes(u.mem_read(vector+4,1))==bytes([count&255]);total+=count
   for i in range(count):
    value=get(get(vector)+i*4)
    if value:assert refs<=value<refs+16*32 and (value-refs)%32==0;owners[(value-refs)//32]+=1
  assert get(0x13bd800)==total and [get(refs+i*32+13) for i in range(16)]==owners
 invoke(0x9d2160,a,(old,));populate(a,old)
 if kind==0:
  invoke(0x9d2160,a,(n,));assert all(get(get(a)+4*i)==0 for i in range(n));verify_owners(((a,n),))
 else:
  invoke(0x9d2160,b,(n,));populate(b,n);target=b if self else a;invoke(0x9d21d0,target,(b,))
  expected=[0 if self or mode==0 else refs+(0 if mode==1 else i%16)*32 for i in range(n)]
  assert [get(get(target)+4*i) for i in range(n)]==expected;verify_owners(((a,old if self else n),(b,n)))
 invoke(0x9d2110,a,())
 if kind:invoke(0x9d2110,b,())
 assert get(a)==get(b)==0 and bytes(u.mem_read(a+4,1))==bytes(u.mem_read(b+4,1))==b'\0'
 assert get(0x13bd800)==16 and all(get(refs+i*32+13)==1 for i in range(16)) and set(allocations)==freed
 return dict(events=events,arena=hashlib.sha256(u.mem_read(BASE,0x10000)).hexdigest(),heap=hashlib.sha256(u.mem_read(HEAP,0x10000)).hexdigest())
def cases():
 for old,n,mode in itertools.product((0,1,3,7),(0,1,2,7,31,127,128,255),range(3)):yield (0,old,n,mode,0)
 for old,n,mode,self in itertools.product((0,1,3,7),(0,1,2,7,31,127),range(3),range(2)):yield (1,old,n,mode,self)
def compare(candidate,original):
 total=0
 for args in cases():assert execute(candidate,*args)==execute(original,*args),args;total+=1
 return total
