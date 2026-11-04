"""Run the complete native bank-name lookup and red-black-tree search."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
OBJECT=0x2000000;HEADER=OBJECT+0x200;NODES=OBJECT+0x300;DATA=OBJECT+0x1000;QUERY=OBJECT+0x2000;STOP=0x2200000;STACK=0x3000000
TEXTS=[None,b'\x80',b'\xff',b'',b'a',b'ab',b'b',b'z']
def execute(codes,count,shape,query,variant):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in [(0x9c0000,0x20000),(0x429000,0x1000),(OBJECT,0x10000),(STOP,0x1000),(STACK,0x10000)]:u.mem_map(a,n)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return int.from_bytes(u.mem_read(a,4),'little')
 for a,b in codes.items():u.mem_write(a,bytes(b))
 values=[0 if variant==0 else i*17+3 if variant==1 else 0xffffffff-i for i in range(8)]
 def build(lo,hi,parent):
  if lo==hi:return 0
  i=(lo+hi)//2 if shape==0 else lo if shape==1 else hi-1;p=NODES+32*i
  put(p+4,parent);put(p+8,build(lo,i,p));put(p+12,build(i+1,hi,p));return p
 for i,t in enumerate(TEXTS):
  p=NODES+32*i;d=DATA+64*i;put(p+16,d if i else 0);put(p+20,values[i]);put(d,d+8)
  if t is not None:u.mem_write(d+8,t+b'\0')
 put(HEADER+4,build(0,count,HEADER));put(OBJECT+0xac,HEADER);put(OBJECT+0xb0,count)
 wanted=query%8 if query<16 else -1
 t=TEXTS[wanted] if wanted>=0 else b'c' if query==16 else b'zz'
 if wanted==0:put(QUERY,0)
 elif query<8:put(QUERY,DATA+64*query)
 else:put(QUERY,QUERY+16);put(QUERY+16,QUERY+32);u.mem_write(QUERY+32,t+b'\0')
 def readtext(p):
  result=[]
  while True:
   v=u.mem_read(p,1)[0]
   if v==0:return result+[0]
   result.append(v if v<128 else v-256);p+=1
 def hook(u,a,size,data):
  if a!=0x429950:return
  sp=u.reg_read(UC_X86_REG_ESP);left=readtext(get(u.reg_read(UC_X86_REG_ECX)));right=readtext(get(get(sp+4)))
  u.reg_write(UC_X86_REG_EAX,int(left<right));u.reg_write(UC_X86_REG_ESP,sp+8);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP);put(sp+4,QUERY);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,OBJECT)
 regs=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(regs):u.reg_write(r,0x887700+i)
 before=bytes(u.mem_read(OBJECT,0x2100));u.emu_start(0x9ccdc0,STOP,count=1000)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+8 and before==bytes(u.mem_read(OBJECT,0x2100))
 assert all(u.reg_read(r)==0x887700+i for i,r in enumerate(regs))
 result=u.reg_read(UC_X86_REG_EAX);assert result==(values[wanted] if 0<=wanted<count else 0)
 return result

def compare(candidate,native):
 cases=0
 for count in range(9):
  for shape in range(3):
   for query in range(18):
    for variant in range(3):
     assert execute(candidate,count,shape,query,variant)==execute(native,count,shape,query,variant);cases+=1
 return cases
