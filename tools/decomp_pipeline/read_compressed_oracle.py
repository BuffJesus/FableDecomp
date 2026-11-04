"""Execute CDataInputStream::ReadCompressed against retail with modelled chunk overflow, vector(n), LZO, free and error callback."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
STREAM=0x2000000;SRC=0x2001000;OUT=0x2002000;HEAP=0x2010000;STACK=0x3000000;STOP=0x4000000;CB=0x5000000
ENTRY=0x995fb0;OVERFLOW,VECTOR,LZO,FREE,CBSLOT=0x993ca0,0x514e20,0xc069d0,0xbfea14,0x13bc9fc
STUBS=(OVERFLOW,VECTOR,LZO,FREE,CB)
def run(codes,source,available,size,reported,callback):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,z in [(0x514000,0x1000),(0x993000,0x1000),(0x995000,0x2000),(0xbfe000,0x1000),(0xc06000,0x1000),(0x13bc000,0x1000),
             (STREAM,0x4000),(HEAP,0x10000),(STACK,0x10000),(STOP,0x1000),(CB,0x1000)]:u.mem_map(a,z)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 for a,b in codes.items():u.mem_write(a,bytes(b))
 for a in STUBS:u.mem_write(a,b'\xc3')
 put(CBSLOT,CB if callback else 0)
 u.mem_write(STREAM,b'\xa5'*0x40);u.mem_write(SRC,source);u.mem_write(OUT,b'\xee'*64)
 put(STREAM+4,0);put(STREAM+8,len(source));put(STREAM+0xc,SRC);put(STREAM+0x10,0);put(STREAM+0x14,min(available,len(source)))
 trace=[];heap=[HEAP]
 def hook(uc,a,s,user):
  if a not in STUBS:return
  sp=uc.reg_read(UC_X86_REG_ESP);ecx=uc.reg_read(UC_X86_REG_ECX);pop=0
  if a==OVERFLOW:
   out,n=get(sp+4),get(sp+8);pos=get(ecx+4);trace.append(('overflow',n,pos))
   u.mem_write(out,source[pos:pos+n]);put(ecx+4,pos+n);put(ecx+0xc,SRC+pos+n);put(ecx+0x14,0);pop=8
  elif a==VECTOR:
   n=get(sp+4);trace.append(('vector',n));p=heap[0];heap[0]+=(n+15)&~15
   u.mem_write(p,b'\0'*n);put(ecx,p);put(ecx+4,p+n);put(ecx+8,p+n);uc.reg_write(UC_X86_REG_EAX,ecx);pop=4
  elif a==LZO:
   src,n,dst,lenp,work=[get(sp+4+4*i) for i in range(5)];cap=get(lenp)
   data=bytes(u.mem_read(src,n));trace.append(('lzo',data,dst,cap,work));u.mem_write(dst,data[::-1][:cap]);put(lenp,reported)
  elif a==FREE:trace.append(('free',get(sp+4)))
  else:trace.append(('callback',))
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
 for size,extra,wide,good,callback,available in itertools.product((5,9,20),(0,1,2),(0,1),(0,1),(0,1),(0,4,9,30)):
  n=size-3+extra;head=(b'\xff\xff'+struct.pack('<i',n)) if wide else struct.pack('<h',n)
  block=bytes((0x40+7*i)&0xff for i in range(n));source=head+block+b'\xd1\xd2\xd3';reported=size-3 if good else size-2
  c=run(candidate,source,available,size,reported,callback);assert c==run(native,source,available,size,reported,callback),(size,extra,wide,good,callback,available)
  trace,out,stream=c
  lz=[t for t in trace if t[0]=='lzo'];assert len(lz)==1 and lz[0][1]==block and lz[0][2]==OUT and lz[0][3]==size and lz[0][4]==0
  assert ('vector',n) in trace and trace[-1][0]=='free' and (('callback',) in trace)==(callback and not good)
  assert out[:size-3]==block[::-1][:size-3] and out[size-3:size]==b'\xd1\xd2\xd3' and out[size]==0xee
  assert struct.unpack_from('<I',stream,4)[0]==len(source);cases+=1
 for size,stored,available in itertools.product((1,12),(0,-5),(0,7,40)):
  source=(b'\xff\xff'+struct.pack('<i',stored)) if stored else b'\0\0';source+=bytes(range(0x90,0x90+size))
  c=run(candidate,source,available,size,0,1);assert c==run(native,source,available,size,0,1)
  trace,out,stream=c;assert not any(t[0] in('lzo','vector','callback') for t in trace) and out[:size]==bytes(range(0x90,0x90+size)) and out[size]==0xee;cases+=1
 for size in (0,-1,-100):
  c=run(candidate,b'\x05\x00abcdefgh',8,size,0,1);assert c==run(native,b'\x05\x00abcdefgh',8,size,0,1)
  trace,out,stream=c;assert not trace and out==b'\xee'*64 and struct.unpack_from('<I',stream,4)[0]==0;cases+=1
 return cases
