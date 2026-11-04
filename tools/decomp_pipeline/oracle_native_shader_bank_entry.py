"""Shader bank entry constructor: data ownership, inline stream reads and strings."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from pe_oracle import EXE,pe_sections
BASE=0x24000000;WIDTH=0x4000

def scenarios():
 for words,constants,chunk,position,mode,empty in itertools.product([0,1,3],[0,1,3],[0,4,0x100],[0,0x7ffffffc,0xfffffffc],range(3),[0,1]):
  payload=struct.pack('<I',words*4+(mode if words else 0))+b''.join(struct.pack('<I',0x76540000+i)for i in range(words))+struct.pack('<I',constants)+b'x\0'*(constants+1)
  yield(words,constants,chunk,position,mode,empty,payload)

def collect():
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000);u.mem_map(BASE,0x10000);raw=EXE.read_bytes()
 for _,va,_,off,n in pe_sections(raw):
  if n:u.mem_write(0x400000+va,raw[off:off+n])
 inputs=bytearray();expected=bytearray();cases=snapshots=0;stream=0;strings=0
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def event(kind,a=0,b=0,c=0,d=0):
  nonlocal snapshots
  arena=bytearray(u.mem_read(BASE,WIDTH));arena[:4]=b'\0'*4
  expected.extend(struct.pack('<5I',kind,a,b,c,d));expected.extend(arena)
  state=bytearray(u.mem_read(stream,28)) if stream else bytearray(28)
  state[:4]=b'\0'*4;expected.extend(state);snapshots+=1
 def hook(u,a,n,_):
  nonlocal stream,strings
  if a not in [0x99a2f0,0x99e4b0,0x9cc240,0xbfea0e,0x9cc2a0,0x994930,0x993ca0,0x464931,0x995d30,0x99ec30,0x433530,0x99eae0,0x99efb0,0x993b10,0xbfea14]:return
  sp=u.reg_read(UC_X86_REG_ESP);this=u.reg_read(UC_X86_REG_ECX);cleanup=0;result=0
  if a==0x99a2f0:assert this==BASE;event(1);put(this,0x129a038);result=this
  elif a==0x99e4b0:assert this==BASE+0x20;event(2,this);put(this,0);result=this
  elif a==0x9cc240:
   assert this==BASE+0x800 and get(sp+4)==4;event(3,this,4);cleanup=4;result=size
   if mode==1:put(BASE+4,0xabcdef01)
  elif a==0xbfea0e:assert get(sp+4)==size and size>0;event(4,size);result=BASE+0x1000
  elif a==0x9cc2a0:
   index,off,dest,length=[get(sp+4+i*4)for i in range(4)];assert this==BASE+0x800 and index==4 and off==0 and length==size
   assert size==0 or dest==BASE+0x1000;event(5,index,off,dest if size else 0,size)
   if size:u.mem_write(dest,bytes(u.mem_read(BASE+0x3000,size)))
   cleanup=16
  elif a==0x994930:
   source=get(sp+4);assert get(sp+8)==size and (size==0 or source==BASE+0x1000)
   event(6,source if size else 0,size);stream=this;source=source if size else BASE+0x3000
   u.mem_write(this,struct.pack('<7I',0x123675c,position,size,source,0,chunk,source));result=this;cleanup=8
  elif a==0x993ca0:
   dest=get(sp+4);length=get(sp+8);assert this==stream and length<=12
   event(7,dest if BASE<=dest<BASE+WIDTH else 0,length)
   u.mem_write(dest,bytes(u.mem_read(get(stream+12),length)));put(stream+12,get(stream+12)+length);put(stream+4,get(stream+4)+length);put(stream+20,0 if mode==2 else 0x100);cleanup=8
  elif a==0x464931:
   count=get(sp+4);value=get(get(sp+8));assert this==BASE+8 and count<=3 and value==0
   event(8,this,count,value);start=BASE+0x1800 if count else 0
   put(this,start);put(this+4,start+count*4);put(this+8,start+count*4)
   if count:u.mem_write(start,b'\0'*(count*4))
   cleanup=8
  elif a==0x995d30:
   assert this==stream;out=get(sp+4);token=0x40000000+strings;event(9,token);strings+=1;put(out,token)
   put(stream+12,get(stream+12)+2);put(stream+4,get(stream+4)+2);put(stream+20,get(stream+20)-2);result=out;cleanup=4
  elif a==0x99ec30:
   token=get(get(sp+4));assert BASE+0x2000<=this<BASE+0x2040;event(10,this,token);put(this,token);result=this;cleanup=4
  elif a==0x433530:
   dest,value,tag,fill,atend=[get(sp+4+i*4)for i in range(5)];assert this==BASE+0x14 and dest==get(BASE+0x18) and fill==1 and atend==1
   token=get(value);event(11,this,token);start=get(this);end=get(this+4);count=(end-start)//4 if start else 0
   if not start:start=BASE+0x2000
   put(start+count*4,token);put(this,start);put(this+4,start+(count+1)*4);put(this+8,start+(count+1 if mode==1 else 8)*4);cleanup=20
  elif a==0x99eae0:
   event(12,get(this));put(this,0)
   if mode==2:put(BASE+4,get(BASE+4)^1)
  elif a==0x99efb0:
   assert this==BASE+0x20;token=get(get(sp+4));event(13,this,token);put(this,token);result=this;cleanup=4
  elif a==0x993b10:assert this==stream;event(14);stream=0
  else:assert get(sp+4)==BASE+0x1000;event(15,get(sp+4))
  u.reg_write(UC_X86_REG_EAX,result);u.reg_write(UC_X86_REG_ESP,sp+4+cleanup);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook)
 for words,constants,chunk,position,mode,empty,payload in scenarios():
  arena=bytearray((i*17+cases*13)&255 for i in range(WIDTH));arena[0x3000:0x3000+len(payload)]=payload;size=0 if empty else len(payload)
  params=(words,constants,chunk,position,mode,empty,size);inputs.extend(struct.pack('<7I',*params));inputs.extend(arena);u.mem_write(BASE,bytes(arena));stream=strings=0
  sp=BASE+0xe000;stop=BASE+0xf000;u.mem_write(sp,struct.pack('<4I',stop,mode,4,BASE+0x800));u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,BASE)
  u.emu_start(0xa62120,stop,count=100000);assert u.reg_read(UC_X86_REG_ESP)==sp+16 and u.reg_read(UC_X86_REG_EAX)==BASE
  event(16);cases+=1
 return inputs,expected,dict(cases=cases,snapshots=snapshots)
