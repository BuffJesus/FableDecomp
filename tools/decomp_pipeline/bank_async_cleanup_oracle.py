"""Validate asynchronous disposal ownership, locking and close ordering."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
BASE=0x2000000;HEAD=BASE+0x1000;NODES=BASE+0x2000;CONTROLS=BASE+0x4000;STACK=0x3000000;STOP=0x4000000;DESTROY=STOP+0x100
def execute(codes,n,stop,mode,shared):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,z in [(0x9d0000,0x10000),(0x9c0000,0x10000),(0x98e000,0x1000),(0xa62000,0x1000),(0xbfe000,0x1000),(BASE,0x10000),(STACK,0x10000),(STOP,0x1000)]:u.mem_map(a,z)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 for a,b in codes.items():u.mem_write(a,bytes(b))
 boundaries=[0xa62920,0xa629a0,0x9d6350,0xbfe9bc,0xbfea14,DESTROY,0x98ec40,0x9ce360]
 for a in boundaries:u.mem_write(a,b'\xc3')
 u.mem_write(BASE,b'\xa5'*0x164);put(BASE+0x144,HEAD);put(HEAD,NODES if n else HEAD);put(HEAD+4,NODES+(n-1)*16 if n else HEAD);refs=0 if not mode else n+(mode!=1) if shared else 1 if mode==1 else 2 if mode==2 else 0xffffffff
 for i in range(n):
  node=NODES+i*16;control=CONTROLS+(0 if shared else i*16);put(node,node+16 if i+1<n else HEAD);put(node+4,node-16 if i else HEAD);put(node+8,0x1000+i*0x100);put(node+12,control if mode else 0);put(control,refs);put(control+4,DESTROY);put(control+8,0x76540000)
 before=bytes(u.mem_read(BASE,0x164));trace=[];state=dict(locked=False,aborts=0,lock=0,removed=0);freed=[];deleted=[]
 def hook(uc,a,size,data):
  if a not in boundaries:return
  sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);pop=0;result=0
  if a==0xa62920:
   assert get(sp+4)==BASE+0x14c and get(sp+8)==0 and not state['locked'];put(this,BASE+0x14c);uc.mem_write(this+4,b'\1');state['locked']=True;state['lock']=this;pop=8;trace.append('lock')
  elif a==0xa629a0:
   assert this==state['lock'] and state['locked'];state['locked']=False;trace.append('unlock')
  elif a==0x9d6350:
   i=state['aborts'];assert state['locked'] and this==0x1000+i*0x100;state['aborts']+=1;result=int(i!=stop);trace.append(('abort',i,result))
  elif a==DESTROY:
   assert state['locked'] and this==0x76540000;trace.append('destroy')
  elif a==0xbfe9bc:
   pointer=get(sp+4);assert state['locked'] and get(pointer)==0;deleted.append(pointer);trace.append(('delete',pointer))
  elif a==0xbfea14:
   pointer=get(sp+4);i=state['removed'];assert state['locked'] and pointer==NODES+i*16 and get(pointer+8)==0 and get(pointer+12)==0;assert get(HEAD)==(NODES+(i+1)*16 if i+1<n else HEAD);assert get(get(HEAD)+4)==HEAD;freed.append(pointer);state['removed']+=1;trace.append(('free',pointer))
  elif a==0x98ec40:assert this==0x12340000;trace.append('thread-close')
  elif a==0x9ce360:assert this==BASE;trace.append('base-close')
  uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
 u.hook_add(UC_HOOK_CODE,hook);regs=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 def call(address):
  sp=STACK+0x8000;put(sp,STOP);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,BASE)
  for i,r in enumerate(regs):u.reg_write(r,0x778800+i)
  u.emu_start(address,STOP,count=10000);assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+4;assert all(u.reg_read(r)==0x778800+i for i,r in enumerate(regs))
 call(0x9d6730);assert before==bytes(u.mem_read(BASE,0x164));assert state['aborts']==stop+(stop<n) and state['removed']==stop and not state['locked'];assert len(deleted)==(int(bool(n and stop==n)) if shared and mode==1 else stop if mode==1 else 0)
 for i in range(n):
  if mode:assert get(CONTROLS+(0 if shared else i*16))==(refs-(stop if shared else i<stop))&0xffffffff
 remaining=[];node=get(HEAD);previous=HEAD
 while node!=HEAD:assert get(node+4)==previous;remaining.append(node);previous=node;node=get(node)
 assert remaining==[NODES+i*16 for i in range(stop,n)] and get(HEAD+4)==previous
 for present in range(2):
  for retail in range(2):
   put(BASE+0x110,0x12340000 if present else 0);u.mem_write(BASE+0x8c,bytes([retail]));before=bytes(u.mem_read(BASE,0x164));start=len(trace);call(0x9d58a0);assert trace[start:]==(['thread-close'] if present and not retail else [])+['base-close'];assert before==bytes(u.mem_read(BASE,0x164))
 return trace,bytes(u.mem_read(HEAD,8)),bytes(u.mem_read(NODES,n*16)),bytes(u.mem_read(CONTROLS,n*16))
def compare(candidate,native):
 cases=0
 for n in range(17):
  for stop in range(n+1):
   for mode in range(4):
    for shared in range(2):assert execute(candidate,n,stop,mode,shared)==execute(native,n,stop,mode,shared);cases+=1
 return cases
