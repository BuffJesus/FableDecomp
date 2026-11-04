"""Run real insertion helpers and comparator with counted-string ownership boundaries."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
BASE=0x2000000;REFS=BASE+0x1000;KEYS=BASE+0x2000;STOP=0x2200000;STACK=0x3000000

def execute(codes,count,pattern,method):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in [(0x9e6000,0x2000),(0x99e000,0x1000),(BASE,0x10000),(STOP,0x1000),(STACK,0x10000)]:u.mem_map(a,n)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return int.from_bytes(u.mem_read(a,4),'little')
 for a,b in codes.items():u.mem_write(a,bytes(b))
 for a in (0x99ec30,0x99eae0,0x99efb0):u.mem_write(a,b"\xc3")
 n=count+1;values=[]
 for i in range(n):
  if method<2:v=i*2+(2 if method==0 else 0) if i<count else [0,1,count,count*2+4,count*2,(count*7)%(count*2)][pattern]
  else:v=0 if i==0 else [n-i,0,i%2,(i*13+count)%17,i,n][pattern]
  values.append(v);ref=REFS+i*32;put(ref,KEYS+i*4);put(ref+4,4);put(ref+13,4);put(KEYS+i*4,v)
  for j in range(3):put(BASE+i*16+j*4,ref)
  put(BASE+i*16+12,i)
 events=[]
 def own(p,delta):
  if p:assert REFS<=p<REFS+n*32 and (p-REFS)%32==0;put(p+13,get(p+13)+delta);assert get(p+13)>=1
 def hook(u,a,z,data):
  if a not in (0x99ec30,0x99eae0,0x99efb0):return
  sp=u.reg_read(UC_X86_REG_ESP);this=u.reg_read(UC_X86_REG_ECX);pop=0
  if a==0x99eae0:own(get(this),-1);put(this,0);events.append(('destroy',))
  else:
   src=get(sp+4);value=get(src);own(value,1);pop=4
   if a==0x99efb0:own(get(this),-1)
   put(this,value);events.append(('copy' if a==0x99ec30 else 'assign',(value-REFS)//32))
  u.reg_write(UC_X86_REG_EAX,this);u.reg_write(UC_X86_REG_ESP,sp+4+pop);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP)
 if method<2:
  for j in range(3):put(sp+4+j*4,REFS+count*32);own(REFS+count*32,1)
  put(sp+16,count);put(sp+20,0);u.reg_write(UC_X86_REG_ECX,BASE if method==0 else BASE+count*16);u.reg_write(UC_X86_REG_EDX,BASE+count*16);address=0x9e6fa0 if method==0 else 0x9e66f0;cleanup=24
 else:
  put(sp+4,0);put(sp+8,0);u.reg_write(UC_X86_REG_ECX,BASE+16);u.reg_write(UC_X86_REG_EDX,BASE+n*16);address=0x9e6d10;cleanup=12
 u.reg_write(UC_X86_REG_ESP,sp);regs=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(regs):u.reg_write(r,0x776600+i)
 try:u.emu_start(address,STOP,count=100000)
 except Exception as error:raise AssertionError((count,pattern,method,hex(u.reg_read(UC_X86_REG_EIP)),hex(u.reg_read(UC_X86_REG_ESP)))) from error
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+cleanup
 assert all(u.reg_read(r)==0x776600+i for i,r in enumerate(regs))
 expected=sorted(range(n),key=lambda i:values[i]);result=[]
 for i,wanted in enumerate(expected):
  row=[get(BASE+i*16+j*4) for j in range(4)];assert row==[REFS+wanted*32]*3+[wanted];result.append(row)
 assert all(get(REFS+i*32+13)==4 for i in range(n))
 return result,events

def compare(candidate,native):
 cases=0
 for n in range(1,33):
  for pattern in range(6):
   for method in range(3):assert execute(candidate,n,pattern,method)==execute(native,n,pattern,method);cases+=1
 return cases
