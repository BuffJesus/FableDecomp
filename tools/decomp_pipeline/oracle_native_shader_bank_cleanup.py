"""Native shader-bank cleanup and symbol-return behavior with mutating callbacks."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from pe_oracle import EXE,pe_sections
BASE=0x24000000;WIDTH=0x1000

def scenarios():
 for op,n,pattern,mode in itertools.product([0,1],range(7),range(3),range(4)):yield(op,n,pattern,mode,0,0,0)
 for n in range(1,7):
  for index,pattern,mode in itertools.product(range(n),range(3),range(4)):yield(2,n,pattern,mode,index,0,0)
 for fill in range(256):yield(3,0,0,0,0,fill,0)
 for refs,mode in itertools.product([None,0,1,2,0xffffffff],range(4)):yield(4,0,0,mode,0,0 if refs is None else refs,int(refs is None))
 for empty,index,mode in itertools.product([0,1,2],[0,1,3],range(4)):yield(5,empty,0,mode,index,0,0)

def collect():
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000);u.mem_map(BASE,0x10000)
 raw=EXE.read_bytes()
 for _,va,_,off,n in pe_sections(raw):
  if n:u.mem_write(0x400000+va,raw[off:off+n])
 inputs=bytearray();expected=bytearray();cases=snapshots=0;delete_count=0
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def event(kind,a=0,b=0):
  nonlocal snapshots
  state=bytearray(u.mem_read(BASE,WIDTH));state[0x804:0x808]=b'\0'*4
  expected.extend(struct.pack('<III',kind,a,b));expected.extend(state);snapshots+=1
 def hook(u,a,n,_):
  nonlocal delete_count
  if a not in [BASE+0x9100,BASE+0x9140,0x9ce360,0xbfe9bc,0x99ebf0,0x99ec30]:return
  sp=u.reg_read(UC_X86_REG_ESP);this=u.reg_read(UC_X86_REG_ECX);cleanup=0;result=0
  if a==BASE+0x9100:
   assert BASE+0x400<=this<BASE+0x600 and get(sp+4)==1
   event(1,this,1);cleanup=4;result=this;delete_count+=1
   if mode==1:put(BASE+0x110,BASE+0x280);put(BASE+0x114,BASE+0x280+count*4)
   elif mode==2:put(BASE+0x114,get(BASE+0x110)+4)
   elif mode==3 and delete_count==1:put(BASE+0x114,get(BASE+0x110)+(count+2)*4)
  elif a==0x9ce360:
   assert this==BASE and get(BASE+0x114)==get(BASE+0x110);event(2,this);put(BASE+4,0x12345678)
  elif a==BASE+0x9140:
   assert this==BASE+0x880;event(3,this)
   if mode:put(BASE+0x800,0x89abcdef);put(BASE+0x808,BASE+0x8c0)
  elif a==0xbfe9bc:assert get(sp+4)==BASE+0x800;event(4,get(sp+4))
  elif a==0x99ebf0:
   assert this==BASE+0x700 and get(sp+8)==0xffffffff and bytes(u.mem_read(get(sp+4),10))==b'(Unknown)\0'
   event(5,0x55443322);put(this,0x55443322);cleanup=8;result=this
   if mode:put(BASE+8,BASE+0x680)
  else:
   source=get(sp+4);assert this==BASE+0x700 and source==BASE+0x600+index*4
   event(6,source,get(source));put(this,get(source));cleanup=4;result=this
   if mode:put(BASE+8,BASE+0x680);put(source,0xabcdef98)
  u.reg_write(UC_X86_REG_EAX,result);u.reg_write(UC_X86_REG_EIP,get(sp));u.reg_write(UC_X86_REG_ESP,sp+4+cleanup)
 u.hook_add(UC_HOOK_CODE,hook);put(BASE+0x9000,BASE+0x9100)
 for op,count,pattern,mode,index,refs,null_info in scenarios():
  arena=bytearray((i*17+cases*13)&255 for i in range(WIDTH))
  def word(o,v):struct.pack_into('<I',arena,o,v&0xffffffff)
  word(0x110,BASE+0x200);word(0x114,BASE+0x200+count*4);word(0x118,BASE+0x240)
  for i in range(10):
   value=BASE+0x400+i*0x20 if pattern==1 or(pattern==2 and i%2==0)else 0
   word(0x200+i*4,value);word(0x280+i*4,value);word(0x400+i*0x20,BASE+0x9000)
  word(0x800,refs);word(0x804,BASE+0x9140);word(0x808,BASE+0x880)
  if op==5:
   word(8,BASE+0x600 if count!=2 else 0);word(12,BASE+0x610 if count==0 else BASE+0x600 if count==1 else 0)
  inputs.extend(struct.pack('<7I',op,count,pattern,mode,index,refs,null_info));inputs.extend(arena);u.mem_write(BASE,bytes(arena));delete_count=0
  args=[]
  if op==2:args=[index]
  elif op==4:args=[123,BASE+0x900,456,BASE+0x880,0 if null_info else BASE+0x800,BASE+0x920,BASE+0x940]
  elif op==5:args=[BASE+0x700,index]
  sp=BASE+0xe000;stop=BASE+0xf000;u.mem_write(sp,struct.pack('<'+'I'*(len(args)+1),stop,*args));u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,BASE)
  u.emu_start([0xa62050,0xa62110,0xa61fc0,0xa61f90,0xa62000,0x9cc1d0][op],stop,count=10000)
  assert u.reg_read(UC_X86_REG_ESP)==sp+4+len(args)*4
  result=u.reg_read(UC_X86_REG_EAX) if op in [3,5] else 0
  if op==3:assert result==107
  if op==5:assert result==BASE+0x700
  event(7,op,result);cases+=1
 return inputs,expected,dict(cases=cases,snapshots=snapshots)
