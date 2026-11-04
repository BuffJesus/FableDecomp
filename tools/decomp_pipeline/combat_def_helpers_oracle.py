import struct,itertools
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
M=0x2000000;OUT=M;OBJS=(0,M+0x100,M+0x200,M+0x300);VT=M+0x1000;DELETE=M+0x1100;QUERY=M+0x2000;STACK=0x3000000;STOP=0x4000000
def execute(code,address,counts,old,new,mode,index):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in [(0x669000,0x1000),(0x9ad000,0x1000),(M,0x4000),(STACK,0x10000),(STOP,0x1000)]:u.mem_map(a,n)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 u.mem_write(M,b'\x5a'*0x4000);u.mem_write(address,bytes(code));put(OUT,OBJS[old]);put(VT+4,DELETE)
 for obj,count in zip(OBJS[1:],counts):put(obj,VT);put(obj+4,count)
 for a in (0x9ad9e0,0x9ada40,DELETE):u.mem_write(a,b'\xc3')
 trace=[];temporary=[0]
 def hook(uc,a,n,user):
  if a not in (0x9ad9e0,0x9ada40,DELETE):return
  sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX)
  if a==DELETE:
   trace.append(('delete',this,get(this+4),get(OUT),get(temporary[0])))
   if mode==1:put(temporary[0],0)
   if mode==2:put(temporary[0],OBJS[3])
   if mode==3:put(OUT,OBJS[3])
   cleanup=4;result=0xbad0bad0
  else:
   assert this==M+0x3000
   result=get(sp+4);arg=get(sp+8);assert arg==(QUERY if address==0x669770 else index&0xffffffff)
   trace.append(('lookup',this,arg));temporary[0]=result;put(result,OBJS[new])
   if new:put(OBJS[new]+4,get(OBJS[new]+4)+1)
   cleanup=12
  uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+cleanup)
  uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_ECX,0xbad1bad1);uc.reg_write(UC_X86_REG_EDX,0xbad2bad2)
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP);put(sp+4,QUERY if address==0x669770 else index);put(sp+8,OUT)
 u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,M+0x3000)
 regs=(UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP)
 for i,r in enumerate(regs):u.reg_write(r,0x123400+i)
 u.emu_start(address,STOP,count=1000)
 assert u.reg_read(UC_X86_REG_ESP)==sp+12 and all(u.reg_read(r)==0x123400+i for i,r in enumerate(regs))
 result=u.reg_read(UC_X86_REG_EAX)&255;called=address==0x669770 or index>0;assert result==int(called and new!=0)
 assert sum(t[0]=='lookup' for t in trace)==int(called)
 if not result:assert get(OUT)==OBJS[old]
 for obj in OBJS[1:]:assert bytes(u.mem_read(obj+8,0x14))==b'\x5a'*0x14
 assert bytes(u.mem_read(OUT+4,0xfc))==b'\x5a'*0xfc
 return result,get(OUT),tuple(get(p+4) for p in OBJS[1:]),trace
def compare(candidate,native):
 cases=0
 for a in candidate:
  for counts in itertools.product((0,1,2,0xffffffff),repeat=3):
   for old,new,mode,index in itertools.product(range(4),range(4),range(4),(-1,0,2)):
    assert execute(candidate[a],a,counts,old,new,mode,index)==execute(native[a],a,counts,old,new,mode,index),(a,counts,old,new,mode,index)
    cases+=1
 return cases
