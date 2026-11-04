"""Direct native comparisons for resource-vector growth, copying and filling."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
BASE=0x2000000;OLD=BASE+0x1000;NEW=BASE+0x2000;INPUT=BASE+0x3000;BLOCKS=BASE+0x4000;STOP=0x2200000;STACK=0x3000000

def execute(codes,used,pos,n,shared,alias):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,size in [(0x980000,0x10000),(0xbfe000,0x1000),(BASE,0x10000),(STOP,0x1000),(STACK,0x10000)]:u.mem_map(a,size)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return int.from_bytes(u.mem_read(a,4),'little')
 for a,c in codes.items():
  if 0x980000<=a<0x990000:u.mem_write(a,bytes(c))
 u.mem_write(NEW,b'\xa5'*128)
 for i in range(6):put(BLOCKS+12*i,100)
 original=[]
 for i in range(used):
  pair=(0x456700+i*16,0 if not shared else BLOCKS+12*(0 if shared==1 else i));original.append(pair)
  put(OLD+i*8,pair[0]);put(OLD+i*8+4,pair[1])
 put(INPUT,0x345600);put(INPUT+4,BLOCKS+60 if shared else 0)
 x=OLD+(used//2)*8 if alias else INPUT;pair=(get(x),get(x+4));expected=original[:pos]+[pair]*n+original[pos:]
 put(BASE,OLD if used else 0);put(BASE+4,OLD+used*8 if used else 0);put(BASE+8,OLD+used*8 if used else 0)
 events=[]
 def hook(u,a,size,data):
  if a not in (0xbfea0e,0xbfea14,0xbfe9bc):return
  sp=u.reg_read(UC_X86_REG_ESP)
  events.append((a,get(sp+4)))
  assert a!=0xbfe9bc,'balanced copies must not delete control blocks'
  u.reg_write(UC_X86_REG_EAX,NEW if a==0xbfea0e else 0);u.reg_write(UC_X86_REG_ESP,sp+4);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000
 for i,v in enumerate([STOP,OLD+pos*8 if used else 0,x,INPUT+8,n,int(pos==used)]):put(sp+4*i,v)
 u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,BASE)
 regs=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(regs):u.reg_write(r,0x554400+i)
 u.emu_start(0x98dc30,STOP,count=20000)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+24
 assert all(u.reg_read(r)==0x554400+i for i,r in enumerate(regs))
 start=NEW if used+n else 0
 assert (get(BASE),get(BASE+4),get(BASE+8))==(start,start+8*(used+n),start+8*(used+max(used,n))), (used,pos,n,shared,alias,tuple(get(BASE+i*4) for i in range(3)),events)
 assert [(get(NEW+i*8),get(NEW+i*8+4)) for i in range(used+n)]==expected
 assert [get(BLOCKS+i*12) for i in range(6)]==[100+(n if pair[1]==BLOCKS+i*12 else 0) for i in range(6)]
 assert events==([(0xbfea0e,8*(used+max(used,n)))] if used+n else [])+([(0xbfea14,OLD)] if used else [])
 return events,bytes(u.mem_read(BASE,12)),bytes(u.mem_read(OLD,48)),bytes(u.mem_read(NEW,128)),bytes(u.mem_read(BLOCKS,72))

def compare_growth(candidate,native):
 cases=0
 for used in range(5):
  for pos in range(used+1):
   for n in range(6):
    for shared in range(3):
     for alias in range(2):
      if alias and not used:continue
      assert execute(candidate,used,pos,n,shared,alias)==execute(native,used,pos,n,shared,alias)
      cases+=1
 return cases


def execute_destroy(codes,refcount,mode):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,size in [(0x980000,0x10000),(0xbfe000,0x1000),(BASE,0x10000),(STOP,0x1000),(STACK,0x10000)]:u.mem_map(a,size)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return int.from_bytes(u.mem_read(a,4),'little')
 u.mem_write(0x98cee0,bytes(codes[0x98cee0]));put(OLD,0x654321);put(OLD+4,BLOCKS if refcount is not None else 0)
 put(BLOCKS,refcount or 0);put(BLOCKS+4,STOP+0x100);put(BLOCKS+8,0x123456);put(BLOCKS+12,77)
 events=[]
 def hook(u,a,size,data):
  if a not in (STOP+0x100,0xbfe9bc):return
  sp=u.reg_read(UC_X86_REG_ESP)
  if a==STOP+0x100:
   assert u.reg_read(UC_X86_REG_ECX)==0x123456;events.append(('callback',))
   if mode==1:put(OLD+4,BLOCKS+12)
   if mode==2:put(OLD+4,0)
  else:events.append(('delete',get(sp+4)))
  u.reg_write(UC_X86_REG_ESP,sp+4);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP);put(sp+4,INPUT)
 u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,OLD);u.reg_write(UC_X86_REG_EDX,OLD+(0 if mode==3 else 8))
 regs=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(regs):u.reg_write(r,0x554400+i)
 u.emu_start(0x98cee0,STOP,count=1000)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+8
 assert all(u.reg_read(r)==0x554400+i for i,r in enumerate(regs))
 expected=[]
 if mode!=3:
  assert get(OLD)==get(OLD+4)==0
  if refcount is not None:assert get(BLOCKS)==(refcount-1)&0xffffffff
  if refcount==1:expected=[('callback',),('delete',BLOCKS if mode==0 else BLOCKS+12 if mode==1 else 0)]
 assert events==expected
 return events,bytes(u.mem_read(OLD,8)),bytes(u.mem_read(BLOCKS,24))

def compare_prior(candidate,native):
 cases=compare_growth(candidate,native)
 for refcount in [None,0,1,2,0xffffffff]:
  for mode in range(4):
   assert execute_destroy(candidate,refcount,mode)==execute_destroy(native,refcount,mode)
   cases+=1
 return cases


def execute_copy_fill(codes,fill,n,refcount,null_info,null_dest):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,size in [(0x980000,0x10000),(BASE,0x10000),(STOP,0x1000),(STACK,0x10000)]:u.mem_map(a,size)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return int.from_bytes(u.mem_read(a,4),'little')
 entry=0x98c030 if fill else 0x98bff0;u.mem_write(entry,bytes(codes[entry]));u.mem_write(NEW,b'\xa5'*48);put(BLOCKS,refcount)
 for i in range(6):put(OLD+8*i,0x234500+i*16);put(OLD+8*i+4,0 if null_info else BLOCKS)
 dest=0 if null_dest else NEW;sp=STACK+0x8000;put(sp,STOP);put(sp+4,OLD if fill else dest);put(sp+8,INPUT)
 u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,dest if fill else OLD);u.reg_write(UC_X86_REG_EDX,n if fill else OLD+8*n)
 regs=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(regs):u.reg_write(r,0x554400+i)
 u.emu_start(entry,STOP,count=1000)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+12 and u.reg_read(UC_X86_REG_EAX)==dest+8*n
 assert all(u.reg_read(r)==0x554400+i for i,r in enumerate(regs))
 assert get(BLOCKS)==(refcount+(0 if null_info or null_dest else n))&0xffffffff
 if not null_dest:
  assert [(get(NEW+8*i),get(NEW+8*i+4)) for i in range(n)]==[(0x234500+(0 if fill else i*16),0 if null_info else BLOCKS) for i in range(n)]
  assert bytes(u.mem_read(NEW+8*n,48-8*n))==b'\xa5'*(48-8*n)
 return bytes(u.mem_read(OLD,48)),bytes(u.mem_read(NEW,48)),get(BLOCKS)

def compare(candidate,native):
 cases=compare_prior(candidate,native)
 for fill in range(2):
  for n in range(6):
   for refcount in [0,1,0xffffffff]:
    for null_info in range(2):
     for null_dest in range(2):
      if null_dest and n>1:continue
      assert execute_copy_fill(candidate,fill,n,refcount,null_info,null_dest)==execute_copy_fill(native,fill,n,refcount,null_info,null_dest)
      cases+=1
 return cases
