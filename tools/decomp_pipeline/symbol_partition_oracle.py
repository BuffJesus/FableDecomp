"""Compare partition and swap with actual comparator and counted-string boundaries."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
BASE=0x2000000;REFS=BASE+0x1000;KEYS=BASE+0x2000;STOP=0x2200000;STACK=0x3000000

def execute(codes,n,pattern,choice,swap=None):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,z in [(0x9e6000,0x2000),(0x99e000,0x1000),(BASE,0x10000),(STOP,0x1000),(STACK,0x10000)]:u.mem_map(a,z)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return int.from_bytes(u.mem_read(a,4),'little')
 for a,b in codes.items():u.mem_write(a,bytes(b))
 for a in (0x99ec30,0x99eae0,0x99efb0):u.mem_write(a,b'\xc3')
 values=[];nulls=[];counts=[]
 for i in range(n):
  v=[i,n-i,7,i%3,(i*13+n)%17,(i*13+n)%17][pattern];null=pattern==5 and i%3==0;values.append(v);nulls.append(null);counts.append(1 if null else 4)
  ref=REFS+i*32;put(ref,KEYS+i*4);put(ref+4,4);put(ref+13,counts[i]);put(KEYS+i*4,v)
  for j in range(3):put(BASE+i*16+j*4,0 if null else ref)
  put(BASE+i*16+12,i)
 events=[]
 def own(p,delta):
  if p:assert REFS<=p<REFS+n*32 and (p-REFS)%32==0;put(p+13,get(p+13)+delta);assert get(p+13)>=1
 def hook(u,a,z,data):
  if a not in (0x99ec30,0x99eae0,0x99efb0):return
  sp=u.reg_read(UC_X86_REG_ESP);this=u.reg_read(UC_X86_REG_ECX);pop=0
  if a==0x99eae0:own(get(this),-1);put(this,0);events.append(('destroy',))
  else:
   value=get(get(sp+4));own(value,1);pop=4
   if a==0x99efb0:own(get(this),-1)
   put(this,value);events.append(('copy' if a==0x99ec30 else 'assign',(value-REFS)//32 if value else -1))
  u.reg_write(UC_X86_REG_EAX,this);u.reg_write(UC_X86_REG_ESP,sp+4+pop);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP)
 if swap is None:
  pivot=[0,n//2,n-1][choice];ref=0 if nulls[pivot] else REFS+pivot*32
  for j in range(3):put(sp+4+j*4,ref);own(ref,1)
  put(sp+16,pivot);put(sp+20,0);u.reg_write(UC_X86_REG_ECX,BASE);u.reg_write(UC_X86_REG_EDX,BASE+n*16);address=0x9e6c80;cleanup=24
 else:u.reg_write(UC_X86_REG_ECX,BASE+16*swap[0]);u.reg_write(UC_X86_REG_EDX,BASE+16*swap[1]);address=0x9e6480;cleanup=4
 u.reg_write(UC_X86_REG_ESP,sp);regs=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(regs):u.reg_write(r,0x776600+i)
 u.emu_start(address,STOP,count=100000)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+cleanup
 assert all(u.reg_read(r)==0x776600+i for i,r in enumerate(regs));ids=[get(BASE+i*16+12) for i in range(n)];assert sorted(ids)==list(range(n))
 for i,j in enumerate(ids):assert [get(BASE+i*16+k*4) for k in range(3)]==[0 if nulls[j] else REFS+j*32]*3
 assert [get(REFS+i*32+13) for i in range(n)]==counts
 cut=None
 if swap is None:
  end=u.reg_read(UC_X86_REG_EAX);assert BASE<=end<=BASE+n*16 and (end-BASE)%16==0;cut=(end-BASE)//16
  rank=lambda i:(not nulls[i],values[i] if not nulls[i] else 0)
  assert all(rank(i)<=rank(pivot) for i in ids[:cut]) and all(rank(i)>=rank(pivot) for i in ids[cut:])
 else:
  expected=list(range(n));expected[swap[0]],expected[swap[1]]=expected[swap[1]],expected[swap[0]];assert ids==expected
 return ids,cut,events

def compare(candidate,native):
 cases=0
 for n in range(1,33):
  for pattern in range(6):
   for choice in range(3):assert execute(candidate,n,pattern,choice)==execute(native,n,pattern,choice);cases+=1
 for pattern in (0,1,5):
  for i in range(8):
   for j in range(8):assert execute(candidate,8,pattern,0,(i,j))==execute(native,8,pattern,0,(i,j));cases+=1
 return cases
