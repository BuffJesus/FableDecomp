"""Execute handle-pool Free with real free-list insertion and neighbour coalescing."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
POOL=0x2000000;ARENA=0x2100000;HANDLES=0x2200000;EXTRA=0x2300000;STACK=0x3000000;STOP=0x4000000;IAT=0x143f000;ENTER,LEAVE=0x5000000,0x5000010
FREE,INSERT,COMBINE=0xa02b30,0xa01da0,0xa01e40
SIZES=(100,512*7+3,512*300);STATES=('none','free','allocated')
def area(i):return ARENA+i*0x10000
def execute(codes,multi,prev,nxt,size,padding,handle,first,extras,largest):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,z in [(0xa01000,0x2000),(IAT,0x1000),(POOL,0x1000),(ARENA,0x30000),(HANDLES,0x1000),(EXTRA,0x1000),(STACK,0x10000),(STOP,0x1000),(ENTER,0x1000)]:u.mem_map(a,z)
 def put(a,v,n=4):u.mem_write(a,(v&(1<<8*n)-1).to_bytes(n,'little'))
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 for a,b in codes.items():u.mem_write(a,bytes(b))
 put(0x143fe0c,ENTER);put(0x143fe10,LEAVE);u.mem_write(ENTER,b'\xc3'*0x20)
 u.mem_write(POOL,b'\x5a'*0x464);put(POOL+0x24,multi,1);put(POOL+0x10,0x100000);put(POOL+0x14,7);put(POOL+0x1c,largest);put(POOL+0x438,HANDLES);put(POOL+0x444,first)
 for i in range(256):put(POOL+0x38+4*i,0)
 for i in range(16):put(HANDLES+4*i,0x700+i)
 def insert(h):
  # Mirror an existing free-list entry exactly as retail stores it.
  bucket=min(get(h+8)//512,255);slot=POOL+0x38+4*bucket;head=get(slot);put(h+0x10,slot);put(h+0x14,head)
  if head:put(head+0x10,h+0x14)
  put(slot,h)
 sizes={0:300+64*extras,2:512*9+5}
 links=[0,area(0) if prev!='none' else 0,area(1),area(2) if nxt!='none' else 0,0]
 for i in range(3):
  h=area(i)
  if (i==0 and prev=='none') or (i==2 and nxt=='none'):continue
  state='allocated' if i==1 else prev if i==0 else nxt;u.mem_write(h,b'\x3c'*0x20);put(h,0x99000000+i);put(h+4,i);put(h+8,size if i==1 else sizes[i]);put(h+0xc,padding if i==1 else 0 if state=='free' else 4*i,2);put(h+0x10,0);put(h+0x14,0);put(h+0x18,links[i]);put(h+0x1c,links[i+2])
  put(h+0xe,0x4e if state=='free' else 0x54,1);put(h+0xf,1,1)
 put(HANDLES+4*handle,area(1))
 for k in range(extras):
  h=EXTRA+k*0x40;u.mem_write(h,b'\x2d'*0x20);put(h+8,[size,300,512*9+5][k%3]);put(h+0xc,0,2);put(h+0xe,0x4e,1);insert(h)
 for i,state in ((0,prev),(2,nxt)):
  if state=='free':insert(area(i))
 trace=[]
 def hook(uc,a,s,user):
  if a not in (ENTER,LEAVE):return
  sp=uc.reg_read(UC_X86_REG_ESP);trace.append(('enter' if a==ENTER else 'leave',get(sp+4)));uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+8)
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP);put(sp+4,handle);regs=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(regs):u.reg_write(r,0x778800+i)
 u.reg_write(UC_X86_REG_ECX,POOL);u.reg_write(UC_X86_REG_ESP,sp);u.emu_start(FREE,STOP,count=5000)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+8 and all(u.reg_read(r)==0x778800+i for i,r in enumerate(regs))
 assert trace==([('enter',POOL+0x44c),('leave',POOL+0x44c)] if multi else [])
 pool=bytes(u.mem_read(POOL,0x464));arena=b''.join(bytes(u.mem_read(area(i),0x20)) for i in range(3));handles=bytes(u.mem_read(HANDLES,64));extra=bytes(u.mem_read(EXTRA,0x100))
 # Independent semantic checks of the retail contract.
 g=lambda off:struct.unpack_from('<i',pool,off)[0]
 assert get(POOL+0x444)==handle and get(HANDLES+4*handle)==first&0xffffffff and g(0x14)==8 and g(0x10)==0x100000-(size+padding+32)
 survivor=area(0) if prev=='free' else area(1);total=size+padding+(sizes[0]+32 if prev=='free' else 0)+(sizes[2]+32 if nxt=='free' else 0)
 assert get(survivor+8)==total and u.mem_read(survivor+0xe,1)[0]==0x4e and get(survivor+0xc)&0xffff==0
 bucket=min(total//512,255);node=get(POOL+0x38+4*bucket);seen=[]
 while node:seen.append(node);node=get(node+0x14)
 assert seen[0]==survivor and get(survivor+0x10)==POOL+0x38+4*bucket
 assert g(0x1c)==max(largest,total)
 return trace,pool,arena,handles,extra
def compare(candidate,native):
 cases=0
 for multi,prev,nxt,size,padding,handle,first,extras,largest in itertools.product((0,1),STATES,STATES,SIZES,(0,12),(0,5,15),(0xffffffff,3),(0,2),(0,0x7fffffff)):
  assert execute(candidate,multi,prev,nxt,size,padding,handle,first,extras,largest)==execute(native,multi,prev,nxt,size,padding,handle,first,extras,largest);cases+=1
 return cases
