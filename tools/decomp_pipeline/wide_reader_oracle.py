"""Execute CDataInputStream::ReadNullTerminatedWString against retail with recorded CWideString and overflow-read stubs."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
STREAM=0x2000000;SRC=0x2001000;RET=0x2002000;STACK=0x3000000;STOP=0x4000000
ENTRY=0x994d20;CTOR,APPEND,COPY,DTOR,OVERFLOW=0x99aed0,0x99b9a0,0x99b720,0x99b510,0x993ca0
STUBS=(CTOR,APPEND,COPY,DTOR,OVERFLOW)
SOURCE=struct.pack('<7H',0xffff,ord('a'),0x8000,ord('z'),0,ord('t'),0)
def run(codes,offset,available):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,z in [(0x993000,0x1000),(0x994000,0x1000),(0x99a000,0x2000),(STREAM,0x4000),(STACK,0x10000),(STOP,0x1000)]:u.mem_map(a,z)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 for a,b in codes.items():u.mem_write(a,bytes(b))
 for a in STUBS:u.mem_write(a,b'\xc3')
 u.mem_write(STREAM,b'\xa5'*0x40);u.mem_write(SRC,SOURCE)
 # CDataInputStream: +4 StreamPos, +8 StreamSize, +0xC CurrentSourcePtr, +0x10 SourceChunkStreamPos, +0x14 SourceChunkBytesRemaining
 put(STREAM+4,offset);put(STREAM+8,len(SOURCE));put(STREAM+0xc,SRC+offset);put(STREAM+0x10,offset);put(STREAM+0x14,min(available,len(SOURCE)-offset))
 trace=[]
 def hook(uc,a,s,user):
  if a not in STUBS:return
  sp=uc.reg_read(UC_X86_REG_ESP);ecx=uc.reg_read(UC_X86_REG_ECX);pop=0
  if a==CTOR:trace.append(('ctor',ecx));uc.reg_write(UC_X86_REG_EAX,ecx)
  elif a==APPEND:trace.append(('append',ecx,get(sp+4)&0xffff));uc.reg_write(UC_X86_REG_EAX,ecx);pop=4
  elif a==COPY:trace.append(('copy',ecx,get(sp+4)));uc.reg_write(UC_X86_REG_EAX,ecx);pop=4
  elif a==DTOR:trace.append(('dtor',ecx))
  else:
   out,size=get(sp+4),get(sp+8);pos=get(ecx+4);trace.append(('overflow',size,pos))
   u.mem_write(out,SOURCE[pos:pos+size]);put(ecx+4,pos+size);pop=8
  uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP);put(sp+4,RET)
 regs=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(regs):u.reg_write(r,0x778800+i)
 u.reg_write(UC_X86_REG_ECX,STREAM);u.reg_write(UC_X86_REG_ESP,sp)
 u.emu_start(ENTRY,STOP,count=5000)
 assert u.reg_read(UC_X86_REG_ESP)==sp+8 and all(u.reg_read(r)==0x778800+i for i,r in enumerate(regs))
 return u.reg_read(UC_X86_REG_EAX),trace,bytes(u.mem_read(STREAM,0x18))
def compare(candidate,native):
 cases=0
 for offset,available in itertools.product((0,8,10,4,2),range(0,14)):
  c=run(candidate,offset,available);assert c==run(native,offset,available),(offset,available)
  eax,trace,stream=c;chars=[]
  pos=offset
  while True:
   ch=struct.unpack_from('<H',SOURCE,pos)[0];pos+=2
   if ch==0:break
   chars.append(ch)
  assert eax==RET and trace[0][0]=='ctor' and [t[2] for t in trace if t[0]=='append']==chars
  local=trace[0][1];assert trace[-2:]==[('copy',RET,local),('dtor',local)]
  reads=len(chars)+1;fromchunk=min(min(available,len(SOURCE)-offset)//2,reads)
  assert sum(1 for t in trace if t[0]=='overflow')==reads-fromchunk and all(t[1]==2 for t in trace if t[0]=='overflow')
  assert struct.unpack_from('<I',stream,4)[0]==pos;cases+=1
 return cases
