"""Compare stream-vector growth and its copy/destruction chain."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
OBJ=0x2000000;OLD=OBJ+0x1000;VALUE=OBJ+0x2000;DATA=OBJ+0x3000;GROW=OBJ+0x5000;FRESH=OBJ+0x6000;STOP=0x2200000;STACK=0x3000000
def execute(codes,used,pos,count,atend,alias,nested):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in [(0xab0000,0x10000),(0xa50000,0x10000),(0x990000,0x10000),(0xbfe000,0x1000),(OBJ,0x20000),(STOP,0x1000),(STACK,0x10000)]:u.mem_map(a,n)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return int.from_bytes(u.mem_read(a,4),'little')
 def vector(a,b,n,c,stride):put(a,b);put(a+4,b+n*stride);put(a+8,b+c*stride)
 for a,b in codes.items():u.mem_write(a,bytes(b))
 vector(OBJ,OLD,used,used+2,20)
 def make(a,index,offset):
  length=(index+nested)%3;storage=DATA+offset if length else 0;put(a,index);put(a+4,0x129d578);vector(a+8,storage,length,length,8)
  for j in range(length):put(storage+8*j,index*10+j);put(storage+8*j+4,j)
 def state(a):
  begin,end=get(a+8),get(a+12);return get(a),bytes(u.mem_read(begin,end-begin)) if end!=begin else b''
 for i in range(used):make(OLD+20*i,100+i,i*0x100)
 make(VALUE,200,0x1000);value=OLD+20*(used//2) if alias else VALUE
 before=[state(OLD+20*i) for i in range(used)];selected=state(value);expected=before[:pos]+[selected]*count+([] if atend else before[pos:]);events=[];fresh=FRESH;copies=0;bases=0
 def hook(u,a,n,data):
  nonlocal fresh,copies,bases
  if a not in (0xbfea0e,0xbfea14,0xa5e0a0,0x99a2e0):return
  sp=u.reg_read(UC_X86_REG_ESP);this=u.reg_read(UC_X86_REG_ECX);result=0;pop=4
  if a==0xbfea0e:events.append(('malloc',get(sp+4)));result=GROW
  elif a==0xbfea14:events.append(('free',get(sp+4)))
  elif a==0x99a2e0:events.append(('base',this));bases+=1
  else:
   other=get(sp+4);first=get(other);length=(get(other+4)-first)//8;storage=fresh if length else 0;fresh+=0x100
   if length:u.mem_write(storage,bytes(u.mem_read(first,length*8)))
   vector(this,storage,length,length,8);events.append(('copy',length));copies+=1;result=this;pop=8
  u.reg_write(UC_X86_REG_EAX,result);u.reg_write(UC_X86_REG_ESP,sp+pop);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000
 for i,v in enumerate([STOP,OLD+pos*20,value,OBJ+0x100,count,atend]):put(sp+4*i,v)
 u.reg_write(UC_X86_REG_ECX,OBJ);u.reg_write(UC_X86_REG_ESP,sp);saved=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(saved):u.reg_write(r,0x778800+i)
 u.emu_start(0xab4540,STOP,count=10000)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+24
 assert all(u.reg_read(r)==0x778800+i for i,r in enumerate(saved))
 capacity=used+max(used,count);begin=GROW if capacity else 0
 assert (get(OBJ),get(OBJ+4),get(OBJ+8))==(begin,begin+len(expected)*20,begin+capacity*20)
 actual=[state(begin+i*20) for i in range(len(expected))];assert actual==expected and copies==len(expected) and bases==used
 assert events[-1]==('free',OLD)
 assert [e for e in events if e[0]=='malloc']==([('malloc',capacity*20)] if capacity else [])
 assert all(get(begin+i*20+4)==0x129d578 for i in range(len(expected)))
 return events,actual,bytes(u.mem_read(OBJ,12))
def cases():
 for used in range(5):
  for pos in range(used+1):
   for count in range(5):
    for atend in range(2):
     for alias in range(2):
      if alias and not used:continue
      for nested in range(3):yield used,pos,count,atend,alias,nested
def compare(candidate,native):
 n=0
 for case in cases():assert execute(candidate,*case)==execute(native,*case),case;n+=1
 return n
