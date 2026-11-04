"""Original/rebuilt packed-array initialization, including retail width-32 carry."""
import struct,hashlib,itertools
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
BASE=0x2000000;HEAP=0x2100000;STOP=0x2200000;STACK=0x3000000
def execute(codes,width,count,stride,mode,old):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in ((0xa62000,0x1000),(0x1397000,0x1000),(0xbfe000,0x1000),(BASE,0x10000),(HEAP,0x10000),(STOP,0x1000),(STACK,0x10000)):u.mem_map(a,n)
 def get(a):return int.from_bytes(u.mem_read(a,4),'little')
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 for a,body in codes.items():u.mem_write(a,body)
 for a in (0xbfeb22,0xbfeb1c):u.mem_write(a,b'\xc3')
 u.mem_write(HEAP,b'\x7d'*0x10000);u.mem_write(BASE,b'\x6b'*16);allocated={};freed=set();events=[]
 def allocate(n):
  assert 0<n<=1024;result=HEAP+2048*len(allocated)+16;allocated[result]=n;events.append(('new',n,result));return result
 def hook(u,a,size,user):
  if a not in (0xbfeb22,0xbfeb1c):return
  sp=u.reg_read(UC_X86_REG_ESP);arg=get(sp+4);result=0
  if a==0xbfeb22:result=allocate(arg)
  else:
   events.append(('delete',arg))
   if arg:
    assert arg in allocated and arg not in freed;n=allocated[arg];assert bytes(u.mem_read(arg-16,16))==bytes(u.mem_read(arg+n,16))==b'\x7d'*16;freed.add(arg)
  u.reg_write(UC_X86_REG_EAX,result);u.reg_write(UC_X86_REG_ESP,sp+4);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook);put(BASE,allocate(8) if old else 0);put(BASE+4,0x76543210);u.mem_write(BASE+8,b'\xff');put(BASE+12,0xa5a5a5a5)
 data=BASE+0x1000;u.mem_write(data,b'\x59'*1024);start=data+16+(5*(count-1) if stride<0 and count else 0);bits_in=min(width,32)
 for i in range(count):
  value=7 if mode==0 else i if mode==1 else (0xffffffff if i&1 else 0) if mode==2 else (i*0x9e3779b9)^0x80ff21 if mode==3 else 0xffffff00+i;put(start+i*stride,value)
 before=bytes(u.mem_read(data,1024));mask=(1<<bits_in)-1;values=[int.from_bytes(u.mem_read(start+i*stride,(bits_in+7)//8),'little')&mask if bits_in else 0 for i in range(count)];bias=min(values) if values else 0;combined=0
 for value in values:combined|=value-bias
 bits=combined.bit_length();words=(bits*count+31)//32
 if bits==32:expected=[(value-bias)|((values[i-1]-bias) if i else 0) for i,value in enumerate(values)]
 else:
  packed=0
  for i,value in enumerate(values):packed|=(value-bias)<<(i*bits)
  expected=[(packed>>(32*i))&0xffffffff for i in range(words)]
 registers=(UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP);sp=STACK+0x8000
 def invoke(entry,args):
  put(sp,STOP)
  for i,v in enumerate(args):put(sp+4+i*4,v)
  for j,r in enumerate(registers):u.reg_write(r,0x889900+j)
  u.reg_write(UC_X86_REG_ECX,BASE);u.reg_write(UC_X86_REG_ESP,sp);u.emu_start(entry,STOP,count=100000)
  assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+4+4*len(args) and all(u.reg_read(r)==0x889900+j for j,r in enumerate(registers))
  return u.reg_read(UC_X86_REG_EAX)
 invoke(0xa62d80,(start,width,stride,count));assert get(BASE+4)==count and get(BASE+12)==bias and bytes(u.mem_read(BASE+8,1))==bytes([bits])
 assert bytes(u.mem_read(BASE+9,3))==b'\x6b'*3 and bytes(u.mem_read(data,1024))==before
 pointer=get(BASE)
 if words:assert pointer in allocated and allocated[pointer]==words*4 and [get(pointer+i*4) for i in range(words)]==expected
 else:assert pointer==0
 assert invoke(0xa62a60,())==16+words*4
 stream=sum(value<<(32*i) for i,value in enumerate(expected))
 for i in range(count):assert invoke(0xa629f0,(i,))==(((stream>>(i*bits))&((1<<bits)-1))+bias)&0xffffffff
 invoke(0xa629d0,());assert get(BASE)==get(BASE+4)==get(BASE+12)==0 and bytes(u.mem_read(BASE+8,1))==b'\0' and set(allocated)==freed
 return dict(bits=bits,bias=bias,expected=expected,events=events,arena=hashlib.sha256(u.mem_read(BASE,0x10000)).hexdigest(),heap=hashlib.sha256(u.mem_read(HEAP,0x10000)).hexdigest())
def cases():return itertools.product((0,1,7,8,9,15,16,17,24,31,32,40),(0,1,2,3,7,16,33),(0,5,12,-5),range(5),range(2))
def compare(candidate,original):
 total=0
 for args in cases():assert execute(candidate,*args)==execute(original,*args),args;total+=1
 return total
