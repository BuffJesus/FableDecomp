"""Execute CDataInputStream::ReadZlibCompressed against retail with modelled overflow read, vector(n), CCompressorZlib members, free and error callback."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
STREAM=0x2000000;SRC=0x2001000;OUT=0x2002000;HEAP=0x2010000;STACK=0x3000000;STOP=0x4000000;CB=0x5000000
ENTRY=0x996230;OVERFLOW,VECTOR,WORKSPACE,BASE,INIT,INFLATE,DTOR,FREE,CBSLOT=0x993ca0,0x514e20,0xa3a7d0,0xa3aea0,0xa3aa10,0xa3ab00,0xa3acc0,0xbfea14,0x13bc9fc
ZLIB_VT=0x1245974
STUBS=(OVERFLOW,VECTOR,WORKSPACE,BASE,INIT,INFLATE,DTOR,FREE,CB)
def run(codes,source,available,size,ret,callback):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,z in [(0x514000,0x1000),(0x993000,0x1000),(0x996000,0x1000),(0xa3a000,0x1000),(0xbfe000,0x1000),(0x13bc000,0x1000),
             (STREAM,0x4000),(HEAP,0x10000),(STACK,0x10000),(STOP,0x1000),(CB,0x1000)]:u.mem_map(a,z)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 for a,b in codes.items():u.mem_write(a,bytes(b))
 for a in STUBS:u.mem_write(a,b'\xc3')
 put(CBSLOT,CB if callback else 0)
 u.mem_write(STREAM,b'\xa5'*0x40);u.mem_write(SRC,source);u.mem_write(OUT,b'\xee'*64)
 put(STREAM+4,0);put(STREAM+8,len(source));put(STREAM+0xc,SRC);put(STREAM+0x10,0);put(STREAM+0x14,min(available,len(source)))
 trace=[];heap=[HEAP];obj={}
 def hook(uc,a,s,user):
  if a not in STUBS:return
  sp=uc.reg_read(UC_X86_REG_ESP);ecx=uc.reg_read(UC_X86_REG_ECX);pop=0;eax=None
  if a==OVERFLOW:
   out,n=get(sp+4),get(sp+8);pos=get(ecx+4);trace.append(('overflow',n,pos))
   u.mem_write(out,source[pos:pos+n]);put(ecx+4,pos+n);put(ecx+0xc,SRC+pos+n);put(ecx+0x14,0);pop=8
  elif a==VECTOR:
   n=get(sp+4);trace.append(('vector',n));p=heap[0];heap[0]+=(n+15)&~15
   u.mem_write(p,b'\0'*n);put(ecx,p);put(ecx+4,p+n);put(ecx+8,p+n);eax=ecx;pop=4
  elif a==WORKSPACE:trace.append(('workspace',));eax=0x1234
  elif a==BASE:trace.append(('base',get(sp+4)));obj['this']=ecx;eax=ecx;pop=4
  elif a==INIT:trace.append(('init',ecx==obj.get('this'),get(ecx)))
  elif a==INFLATE:
   d,n,o,cap=[get(sp+4+4*i) for i in range(4)];data=bytes(u.mem_read(d,n)) if n else b''
   trace.append(('inflate',ecx==obj.get('this'),data,o,cap));u.mem_write(o,bytes(b^0x5a for b in data)[:cap]);eax=ret;pop=16
  elif a==DTOR:trace.append(('dtor',ecx==obj.get('this')))
  elif a==FREE:trace.append(('free',))
  else:trace.append(('callback',))
  if eax is not None:uc.reg_write(UC_X86_REG_EAX,eax)
  uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP);put(sp+4,OUT);put(sp+8,size)
 regs=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(regs):u.reg_write(r,0x778800+i)
 u.reg_write(UC_X86_REG_ECX,STREAM);u.reg_write(UC_X86_REG_ESP,sp)
 u.emu_start(ENTRY,STOP,count=20000)
 assert u.reg_read(UC_X86_REG_ESP)==sp+12 and all(u.reg_read(r)==0x778800+i for i,r in enumerate(regs))
 return trace,bytes(u.mem_read(OUT,64)),bytes(u.mem_read(STREAM,0x18))
def compare(candidate,native):
 cases=0
 for size,n,good,callback,available in itertools.product((1,7,20),(0,1,6,23),(0,1),(0,1),(0,3,9,40)):
  source=struct.pack('<i',n)+bytes((0x30+3*i)&0xff for i in range(n));ret=size if good else size+1
  c=run(candidate,source,available,size,ret,callback);assert c==run(native,source,available,size,ret,callback),(size,n,good,callback,available)
  trace,out,stream=c;names=[t[0] for t in trace]
  core=[x for x in names if x!='overflow']
  assert core==['vector','workspace','base','init','inflate']+(['callback'] if callback and not good else [])+['dtor','free']
  assert ('vector',n) in trace and ('base',0x1234) in trace and ('init',True,ZLIB_VT) in trace and ('dtor',True) in trace
  inf=[t for t in trace if t[0]=='inflate'][0];assert inf[1] and inf[2]==source[4:] and inf[3]==OUT and inf[4]==size
  assert out[:min(size,n)]==bytes(b^0x5a for b in source[4:])[:size] and struct.unpack_from('<I',stream,4)[0]==len(source);cases+=1
 for size in (0,-3):
  c=run(candidate,b'\x04\x00\x00\x00abcd',8,size,0,1);assert c==run(native,b'\x04\x00\x00\x00abcd',8,size,0,1)
  trace,out,stream=c;assert not trace and out==b'\xee'*64;cases+=1
 return cases
