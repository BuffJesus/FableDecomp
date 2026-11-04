"""Execute CParentDefClassBase::RemoveSubDef, the sub-def map's UpperBound and its tail copy against retail."""
import bisect,itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
DEF=0x2000000;ENTRIES=0x2010000;NAME=0x2020000;DATA=0x2020100;TEXT=0x2020200;KEY=0x2030000;STACK=0x3000000;STOP=0x4000000;EMPTY=0x129aaf4
CRC=0x4014a0;LOWER=0x9b20d0;SORT=0x9b84c0;REMOVE=0x9b0170;UPPER=0x9b2190;COPY=0x9b1620
VEC=DEF+0x29
def crc(data):
 h=0x811c9dc5
 for b in data:h=((h^b)*0x01000193)&0xffffffff
 return h
def run(codes,entry,args,setup,ecx=DEF):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,z in [(0x401000,0x1000),(0x9b0000,0x9000),(0x1290000,0x10000),(DEF,0x40000),(STACK,0x10000),(STOP,0x1000)]:u.mem_map(a,z)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 for a,b in codes.items():u.mem_write(a,bytes(b))
 for a in (CRC,LOWER,SORT):u.mem_write(a,b'\xc3')
 u.mem_write(DEF,b'\x3c'*0x40000);u.mem_write(EMPTY,b'\x00');setup(u,put)
 trace=[]
 def hook(uc,a,s,user):
  if a not in (CRC,LOWER,SORT):return
  sp=uc.reg_read(UC_X86_REG_ESP);ecx=uc.reg_read(UC_X86_REG_ECX);edx=uc.reg_read(UC_X86_REG_EDX);eax=None
  if a==CRC:
   data=bytes(u.mem_read(edx,get(sp+4)));trace.append(('crc',ecx,data));eax=crc(data)
  elif a==LOWER:
   key=get(get(sp+4));trace.append(('lower_bound',ecx,key));p=get(ecx)
   while p<get(ecx+4) and get(p)<key:p+=12
   eax=p
  else:
   trace.append(('sort',ecx,edx,get(sp+4)&0xff));items=[bytes(u.mem_read(p,12)) for p in range(ecx,edx,12)]
   items.sort(key=lambda b:struct.unpack_from('<I',b)[0]);u.mem_write(ecx,b''.join(items))
  if eax is not None:uc.reg_write(UC_X86_REG_EAX,eax)
  uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+8)
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP)
 for i,v in enumerate(args):put(sp+4+4*i,v)
 regs=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(regs):u.reg_write(r,0x778800+i)
 u.reg_write(UC_X86_REG_ECX,ecx);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_EAX,0xdeadbeef)
 u.emu_start(entry,STOP,count=200000)
 assert u.reg_read(UC_X86_REG_ESP)==sp+4+4*len(args) and all(u.reg_read(r)==0x778800+i for i,r in enumerate(regs)),hex(entry)
 s,f=get(VEC),get(VEC+4)
 return u.reg_read(UC_X86_REG_EAX),trace,[struct.unpack('<3I',u.mem_read(p,12)) for p in range(s,f,12)],bytes(u.mem_read(VEC,0x10)),bytes(u.mem_read(DEF,0x29))
def table(u,put,items,spare,dirty,comp):
 for i,e in enumerate(items):u.mem_write(ENTRIES+12*i,struct.pack('<3I',*e))
 put(VEC,ENTRIES);put(VEC+4,ENTRIES+12*len(items));put(VEC+8,ENTRIES+12*(len(items)+spare));u.mem_write(VEC+12,bytes([comp,dirty]))
TEXTS=[None,b'',b'Sword',b'Shield',b'Hat']
def compare(candidate,native):
 cases=0
 def both(entry,args,setup,ecx=DEF):
  c=run(candidate,entry,args,setup,ecx);assert c==run(native,entry,args,setup,ecx),hex(entry);return c
 for copies,others,text,dirty,spare in itertools.product((0,1,2,3),(0,1,4),TEXTS,(0,1),(0,2)):
  data=b'' if text is None else text;key=crc(data)
  below=[(key-1-i)&0xffffffff for i in range(others)] if key>others else []
  above=[(key+1+i)&0xffffffff for i in range(others)] if key<0xffffffff-others else []
  items=sorted([(k,0x100+i,0x200+i) for i,k in enumerate(below+above)]+[(key,0x300+i,0x400+i) for i in range(copies)],key=lambda e:e[0])
  def setup(u,put,items=items,text=text,dirty=dirty,spare=spare):
   table(u,put,items,spare,dirty,0x5a)
   if text is None:put(NAME,0)
   else:u.mem_write(TEXT,text+b'\x00');put(NAME,DATA);put(DATA,TEXT);put(DATA+4,len(text))
  eax,trace,result,vec,head=both(REMOVE,[NAME],setup)
  kept=[e for e in items if e[0]!=key]
  assert result==sorted(kept,key=lambda e:e[0]) and vec[13]==0 and head==b'\x3c'*0x29
  assert struct.unpack_from('<I',vec,8)[0]==ENTRIES+12*(len(items)+spare)
  expected=[('crc',0,data),('lower_bound',VEC,key)]
  if dirty:expected.append(('sort',ENTRIES,ENTRIES+12*len(kept),0x5a))
  assert trace==expected,(trace,expected);cases+=1
 for n,probe_set in [(0,[0,5]),(1,[0,7,8,0xffffffff]),(6,[0,1,2,3,4,5,6,7,0xffffffff]),(9,list(range(0,40,3)))]:
  keys=sorted([(i*5)//3 for i in range(n)])
  items=[(k,i,~i&0xffffffff) for i,k in enumerate(keys)]
  for probe in probe_set:
   def setup(u,put,items=items,probe=probe):table(u,put,items,0,0,0);put(KEY,probe)
   eax,trace,result,vec,head=both(UPPER,[KEY],setup,VEC)
   assert eax==ENTRIES+12*bisect.bisect_right(keys,probe) and not trace and result==items;cases+=1
 return cases
