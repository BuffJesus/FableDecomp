"""Execute CParentDefClassBase's accessors, sub-def lookups, AddSubDef and InternalSaveBinary against retail."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
DEF=0x2000000;ENTRIES=0x2010000;GROWN=0x2018000;NAME=0x2020000;DATA=0x2020100;TEXT=0x2020200;STREAM=0x2030000;RET=0x2040000
STACK=0x3000000;STOP=0x4000000;EMPTY=0x129aaf4
CRC=0x4014a0;LB_CONST=0x9b2130;LB=0x9b20d0;OVERFLOW=0x9b60d0;SORT=0x9b84c0
WRITES={0x993e50:('ubyte',0xff),0x993e80:('uword',0xffff),0x993ea0:('ulong',0xffffffff),0x993eb0:('slong',0xffffffff)}
GET,SET,ISPARENT,GETMAP,FIND_CONST,FIND,ADD,SAVE=0x4303b0,0x4303c0,0x4303d0,0x4303e0,0x9ad4a0,0x9ad510,0x9b00d0,0x9b0200
VEC=DEF+0x29  # SubDefInfoMap: start, finish, end of storage, comparator byte (+0x0C), Dirty (+0x0D)
def crc(seed,data):
 h=seed^0x811c9dc5
 for b in data:h=((h^b)*0x01000193)&0xffffffff
 return h
def run(codes,entry,args,setup=None):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,z in [(0x430000,0x1000),(0x9ad000,0x1000),(0x9b0000,0x1000),(0x1290000,0x10000),(DEF,0x50000),(STACK,0x10000),(STOP,0x1000),
             (0x401000,0x1000),(0x993000,0x1000),(0x9b2000,0x1000),(0x9b6000,0x1000),(0x9b8000,0x1000)]:u.mem_map(a,z)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 for a,b in codes.items():u.mem_write(a,bytes(b))
 for a in (CRC,LB_CONST,LB,OVERFLOW,SORT,*WRITES):u.mem_write(a,b'\xc3')
 u.mem_write(DEF,b'\x3c'*0x50000);u.mem_write(EMPTY,b'\x00')
 if setup:setup(u,put)
 trace=[]
 def entries():
  s,f=get(VEC),get(VEC+4);return [struct.unpack('<3I',u.mem_read(p,12)) for p in range(s,f,12)]
 def hook(uc,a,s,user):
  if a not in (CRC,LB_CONST,LB,OVERFLOW,SORT) and a not in WRITES:return
  sp=uc.reg_read(UC_X86_REG_ESP);ecx=uc.reg_read(UC_X86_REG_ECX);edx=uc.reg_read(UC_X86_REG_EDX);pop=4;eax=None
  if a==CRC:
   n=get(sp+4);data=bytes(u.mem_read(edx,n));trace.append(('crc',ecx,data));eax=crc(ecx,data)
  elif a in (LB_CONST,LB):
   key=get(get(sp+4));trace.append(('lower_bound',a==LB_CONST,ecx,key));p=get(ecx)
   while p<get(ecx+4) and get(p)<key:p+=12
   eax=p
  elif a==OVERFLOW:
   pos,value,tag,n,atend=[get(sp+4+4*i) for i in range(5)];item=bytes(u.mem_read(value,12))
   trace.append(('overflow',ecx,pos,item,n,atend&0xff));old=bytes(u.mem_read(get(ecx),get(ecx+4)-get(ecx)))
   cap=max(1,2*(get(ecx+8)-get(ecx))//12);body=old+item*n;u.mem_write(GROWN,body);put(ecx,GROWN);put(ecx+4,GROWN+len(body));put(ecx+8,GROWN+12*cap);pop=0x14
  elif a==SORT:
   comp=get(sp+4)&0xff;trace.append(('sort',ecx,edx,comp));items=sorted(bytes(u.mem_read(p,12)) for p in range(ecx,edx,12))
   items.sort(key=lambda b:struct.unpack_from('<I',b)[0]);u.mem_write(ecx,b''.join(items))
  else:
   kind,mask=WRITES[a];trace.append((kind,ecx,get(sp+4)&mask))
  if eax is not None:uc.reg_write(UC_X86_REG_EAX,eax)
  uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP)
 for i,v in enumerate(args):put(sp+4+4*i,v)
 regs=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(regs):u.reg_write(r,0x778800+i)
 u.reg_write(UC_X86_REG_ECX,DEF);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_EAX,0xdeadbeef)
 u.emu_start(entry,STOP,count=20000)
 assert u.reg_read(UC_X86_REG_ESP)==sp+4+4*len(args) and all(u.reg_read(r)==0x778800+i for i,r in enumerate(regs)),hex(entry)
 return u.reg_read(UC_X86_REG_EAX),trace,bytes(u.mem_read(DEF,0x50000)),entries()
def table(u,put,items,spare=0,dirty=0,comp=0):
 for i,(k,d,p) in enumerate(items):u.mem_write(ENTRIES+12*i,struct.pack('<3I',k,d&0xffffffff,p&0xffffffff))
 put(VEC,ENTRIES);put(VEC+4,ENTRIES+12*len(items));put(VEC+8,ENTRIES+12*(len(items)+spare));u.mem_write(VEC+12,bytes([comp,dirty]))
def name(u,put,text):
 if text is None:put(NAME,0);return
 u.mem_write(TEXT,text+b'\x00');put(NAME,DATA);put(DATA,TEXT);put(DATA+4,len(text))
TEXTS=[None,b'',b'Sword',b'Shield',b'Hat',b'Boots']
def compare(candidate,native):
 cases=0
 def both(entry,args,setup=None):
  c=run(candidate,entry,args,setup);assert c==run(native,entry,args,setup),hex(entry);return c
 for value in (0xffffffff,0,1,0x1234,0x7fffffff):
  def setup(u,put,value=value):put(DEF+0x25,value)
  eax,trace,mem,_=both(GET,[RET],setup);assert eax==RET and struct.unpack_from('<I',mem,RET-DEF)[0]==value and not trace
  eax,trace,mem,_=both(SET,[value]);assert struct.unpack_from('<I',mem,0x25)[0]==value and mem[:0x25]==b'\x3c'*0x25 and mem[0x29:0x40]==b'\x3c'*0x17 and not trace
  cases+=2
 eax,trace,mem,_=both(ISPARENT,[]);assert eax&0xff==1 and not trace
 eax,trace,mem,_=both(GETMAP,[]);assert eax==VEC and not trace;cases+=2
 for count,text,entry in itertools.product(range(5),TEXTS,(FIND,FIND_CONST)):
  data=b'' if text is None else text;keys=sorted({crc(0,t or b'') for t in TEXTS[1:1+count]})
  items=[(k,100+i,200+i) for i,k in enumerate(keys)]
  def setup(u,put,items=items,text=text):table(u,put,items,1);name(u,put,text)
  eax,trace,mem,_=both(entry,[NAME],setup);key=crc(0,data)
  assert trace==[('crc',0,data),('lower_bound',entry==FIND_CONST,VEC,key)]
  hit=[i for i,(k,_,_) in enumerate(items) if k==key];assert eax==(ENTRIES+12*hit[0]+4 if hit else 0);cases+=1
 for count,spare,dirty,text in itertools.product(range(4),(0,1,2),(0,1),TEXTS):
  data=b'' if text is None else text;items=[(0x40000000*(3-i)+i,10+i,20+i) for i in range(count)]
  def setup(u,put,items=items,spare=spare,dirty=dirty,text=text):table(u,put,items,spare,dirty,0x5a);name(u,put,text)
  eax,trace,mem,result=both(ADD,[NAME,0x80000000|count,0xfffffff0+spare],setup);key=crc(0,data);added=(key,0x80000000|count,0xfffffff0+spare)
  start=ENTRIES if spare else GROWN
  expected=[('crc',0,data)]+([] if spare else [('overflow',VEC,ENTRIES+12*count,struct.pack('<3I',*added),1,1)])+[('sort',start,start+12*(count+1),0x5a)]
  assert trace==expected,(trace,expected);assert result==sorted(items+[added],key=lambda e:e[0]) and mem[VEC-DEF+13]==0;cases+=1
 for count,dirty,flags,junk in itertools.product(range(5),(0,1),range(8),(0,0xf8)):
  items=[(0x10000000*((7*i)%5),0x100+i,0x80000000+i) for i in range(count)]
  def setup(u,put,items=items,dirty=dirty,flags=flags,junk=junk):table(u,put,items,0,dirty,0x33);u.mem_write(DEF+0x24,bytes([flags|junk]))
  eax,trace,mem,result=both(SAVE,[STREAM],setup);order=sorted(items) if dirty else items
  expected=[('ubyte',STREAM,flags&1),('ubyte',STREAM,(flags>>1)&1),('ubyte',STREAM,(flags>>2)&1)]
  if dirty:expected.append(('sort',ENTRIES,ENTRIES+12*count,0x33))
  expected.append(('uword',STREAM,count))
  for k,d,p in order:expected+=[('ulong',STREAM,k),('slong',STREAM,d),('slong',STREAM,p)]
  assert trace==expected and result==order and mem[VEC-DEF+13]==0;cases+=1
 return cases
