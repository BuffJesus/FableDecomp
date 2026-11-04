"""Execute stream-vector assignment and its recovered stream copy/destruction chain."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
OBJECT=0x2000000;SOURCE=OBJECT+0x100;OLD=OBJECT+0x1000;SRC=OBJECT+0x2000;GROW=OBJECT+0x3000;DATA=OBJECT+0x4000;FRESH=OBJECT+0x8000;STOP=0x2200000;STACK=0x3000000

def execute(codes,self,used,cap,n,nested):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,size in [(0xa50000,0x10000),(0x990000,0x10000),(0xbfe000,0x1000),(OBJECT,0x20000),(STOP,0x1000),(STACK,0x10000)]:u.mem_map(a,size)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return int.from_bytes(u.mem_read(a,4),'little')
 for a,c in codes.items():u.mem_write(a,bytes(c))
 def vector(at,begin,count,capacity,stride):
  put(at,begin);put(at+4,begin+count*stride);put(at+8,begin+capacity*stride)
 vector(OBJECT,OLD if cap else 0,used,cap,20);vector(SOURCE,SRC if n else 0,n,n,20)
 for begin,count,idbase,offset in [(OLD,used,100,0),(SRC,n,200,0x1000)]:
  for i in range(count):
   e=begin+i*20;id=idbase+i;length=(i+nested)%3;storage=DATA+offset+i*0x100 if length else 0
   put(e,id);put(e+4,0x12345678);vector(e+8,storage,length,length,8)
   for j in range(length):put(storage+j*8,id*10+j);put(storage+j*8+4,(id+j)%14)
 events=[];fresh=FRESH;counts=[0,0,0]
 def hook(u,a,size,data):
  nonlocal fresh
  if a not in (0xbfea0e,0xbfea14,0xa5e0a0,0xa5e4d0,0x99a2e0):return
  sp=u.reg_read(UC_X86_REG_ESP);this=u.reg_read(UC_X86_REG_ECX);value=0;popped=4
  if a==0xbfea0e:events.append(('malloc',get(sp+4)));value=GROW
  elif a==0xbfea14:events.append(('free',get(sp+4)))
  elif a==0x99a2e0:events.append(('base_dtor',this));counts[2]+=1
  else:
   other=get(sp+4);length=(get(other+4)-get(other))//8;events.append(('copy' if a==0xa5e0a0 else 'assign',this,other,length));counts[0 if a==0xa5e0a0 else 1]+=1
   if this!=other:
    storage=fresh if length else 0;fresh+=0x100
    if length:u.mem_write(storage,bytes(u.mem_read(get(other),length*8)))
    vector(this,storage,length,length,8)
   popped=8;value=this
  u.reg_write(UC_X86_REG_EAX,value);u.reg_write(UC_X86_REG_ESP,sp+popped);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP);put(sp+4,OBJECT if self else SOURCE);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,OBJECT)
 regs=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(regs):u.reg_write(r,0x554400+i)
 u.emu_start(0xa5e9c0,STOP,count=10000)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+8 and u.reg_read(UC_X86_REG_EAX)==OBJECT
 assert all(u.reg_read(r)==0x554400+i for i,r in enumerate(regs))
 size=used if self else n;capacity=cap if self or n<=cap else n;begin=GROW if not self and n>cap else OLD if cap else 0
 assert (get(OBJECT),get(OBJECT+4),get(OBJECT+8))==(begin,begin+size*20,begin+capacity*20)
 expectedcounts=[0,0,0] if self else [n if n>cap else max(n-used,0),0 if n>cap else min(n,used),used if n>cap else max(used-n,0)]
 assert counts==expectedcounts
 result=[]
 for i in range(size):
  e=begin+i*20;id=(100 if self else 200)+i;length=(i+nested)%3;storage=get(e+8)
  assert get(e)==id and get(e+12)==storage+length*8
  assert get(e+4)==(0x129d578 if not self and (n>cap or i>=used) else 0x12345678)
  values=[(get(storage+j*8),get(storage+j*8+4)) for j in range(length)]
  assert values==[(id*10+j,(id+j)%14) for j in range(length)];result.append((bytes(u.mem_read(e,20)),values))
 if self:assert not events
 return events,bytes(u.mem_read(OBJECT,12)),bytes(u.mem_read(SOURCE,12)),bytes(u.mem_read(SRC,n*20)),result

def compare(candidate,native):
 cases=0
 for self in range(2):
  for used in range(4):
   for cap in range(used,6):
    for n in range(5):
     for nested in range(3):
      assert execute(candidate,self,used,cap,n,nested)==execute(native,self,used,cap,n,nested)
      cases+=1
 return cases
