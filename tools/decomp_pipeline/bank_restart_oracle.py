"""Compare disposal followed by tree-ordered restart and real node teardown."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
BASE=0x2000000;OWNERS=BASE+0x1000;DATA=BASE+0x2000;HEADER=BASE+0x3000;NODES=BASE+0x4000;HEAD=BASE+0x5000;LIST=BASE+0x5100;STACK=0x3000000;STOP=0x4000000
def execute(codes,n,shape,variant,disposal):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,z in [(0x9d0000,0x10000),(0x98f000,0x1000),(0xa62000,0x1000),(0xbfe000,0x1000),(BASE,0x10000),(STACK,0x10000),(STOP,0x1000)]:u.mem_map(a,z)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 for a,b in codes.items():u.mem_write(a,bytes(b))
 boundaries=[0xa62920,0xa629a0,0x98f350,0x9d6350,0xbfea14]
 for a in boundaries:u.mem_write(a,b'\xc3')
 u.mem_write(BASE,b'\xa5'*0x164);u.mem_write(OWNERS,b'\xb6'*0x600);u.mem_write(DATA,b'\xc7'*(24*0x48));seed=[0,0x87654321,0xffffffff][variant]
 for i in range(n):
  obj=DATA+i*0x48;owner=OWNERS+i%3*0x200;put(owner+0x110,0x12340000+i%3*0x100);put(obj+0x28,owner);put(obj+0x18,0x55550000+i);put(obj+0x30,i*13);put(obj+0x34,i*29)
 def build(first,last,parent):
  if first==last:return 0
  at=(first+last)//2 if shape==0 else first if shape==1 else last-1 if shape==2 else (first+last-1)//2;node=NODES+at*20;put(node+4,parent);put(node+8,build(first,at,node));put(node+12,build(at+1,last,node));put(node+16,DATA+at*0x48);return node
 put(HEADER+4,build(0,n,HEADER));put(HEADER+8,NODES if n else HEADER);put(HEADER+12,NODES+(n-1)*20 if n else HEADER);put(BASE+0x134,HEADER);put(BASE+0x138,n);put(BASE+0x144,HEAD);put(HEAD,LIST);put(HEAD+4,LIST+16)
 for i in range(2):put(LIST+i*16,LIST+16 if not i else HEAD);put(LIST+i*16+4,HEAD if not i else LIST);put(LIST+i*16+8,0x1000+i*0x100);put(LIST+i*16+12,0)
 before=bytearray(u.mem_read(DATA,24*0x48));bankBefore=bytearray(u.mem_read(BASE,0x164));trace=[];state=dict(locked=False,locks=0,adds=0,aborts=0,lock=0);freed=[]
 def hook(uc,a,size,user):
  if a not in boundaries:return
  sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);pop=0;result=0
  if a==0xa62920:
   assert not state['locked'] and get(sp+4)==BASE+0x14c and get(sp+8)==0;state['locked']=True;state['locks']+=1;state['lock']=this;put(this,BASE+0x14c);uc.mem_write(this+4,b'\1');pop=8;trace.append(('lock',state['locks']))
  elif a==0xa629a0:
   assert state['locked'] and this==state['lock'];state['locked']=False;trace.append(('unlock',state['locks']))
  elif a==0x9d6350:
   i=state['aborts'];assert state['locked'] and state['locks']==1 and this==0x1000+i*0x100;result=int(i!=disposal);state['aborts']+=1;trace.append(('abort',i,result))
  elif a==0x98f350:
   i=state['adds'];assert state['locked'] and state['locks']==2 and i<n and this==0x12340000+i%3*0x100 and get(sp+4)==DATA+i*0x48+0x2c and get(DATA+i*0x48+0x18)==0x55550000+i;result=(seed+i*7)&0xffffffff;state['adds']+=1;pop=4;trace.append(('add',i,this,result))
  elif a==0xbfea14:
   pointer=get(sp+4);assert state['locked'] and pointer not in freed
   if state['locks']==1:assert pointer==LIST+16*len(freed) and get(pointer+8)==0 and get(pointer+12)==0
   else:assert pointer in [NODES+i*20 for i in range(n)] and state['adds']==n
   freed.append(pointer);trace.append(('free',pointer))
  uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP);u.reg_write(UC_X86_REG_ECX,BASE);u.reg_write(UC_X86_REG_ESP,sp);regs=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(regs):u.reg_write(r,0x778800+i)
 u.emu_start(0x9d68b0,STOP,count=20000);assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+4;assert all(u.reg_read(r)==0x778800+i for i,r in enumerate(regs))
 assert not state['locked'] and state['locks']==2 and state['adds']==n and state['aborts']==disposal+(disposal<2)
 for i in range(n):struct.pack_into('<I',before,i*0x48+0x18,(seed+i*7)&0xffffffff)
 assert before==bytes(u.mem_read(DATA,24*0x48));struct.pack_into('<I',bankBefore,0x138,0);assert bankBefore==bytes(u.mem_read(BASE,0x164));assert get(HEADER+4)==0 and get(HEADER+8)==HEADER and get(HEADER+12)==HEADER
 assert set(freed)==set([LIST+i*16 for i in range(disposal)]+[NODES+i*20 for i in range(n)])
 assert get(HEAD)==(LIST+disposal*16 if disposal<2 else HEAD) and get(get(HEAD)+4)==HEAD
 return trace,bytes(u.mem_read(HEADER,16)),before
def compare(candidate,native):
 cases=0
 for n in range(25):
  for shape in range(4):
   for variant in range(3):
    for disposal in range(3):assert execute(candidate,n,shape,variant,disposal)==execute(native,n,shape,variant,disposal);cases+=1
 return cases
