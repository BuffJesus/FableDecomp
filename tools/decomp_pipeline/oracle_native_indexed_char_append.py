"""Indexed string append: cached index, map insertion, then live vector append."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from pe_oracle import EXE,pe_sections
BASE=0x24000000;WIDTH=0x1000
def collect():
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000);u.mem_map(BASE,0x10000);raw=EXE.read_bytes()
 for _,va,_,off,n in pe_sections(raw):
  if n:u.mem_write(0x400000+va,raw[off:off+n])
 inputs=bytearray();expected=bytearray();cases=snapshots=copies=0
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def event(kind,a=0,b=0,c=0):
  nonlocal snapshots
  expected.extend(struct.pack('<4I',kind,a,b,c));expected.extend(u.mem_read(BASE,WIDTH));snapshots+=1
 def replace(size,capacity):put(BASE,BASE+0x400);put(BASE+4,BASE+0x400+size*4);put(BASE+8,BASE+0x400+capacity*4)
 def hook(u,a,n,_):
  nonlocal copies
  if a not in [0x99ec30,0x431606,0x99eae0,0x433530]:return
  sp=u.reg_read(UC_X86_REG_ESP);this=u.reg_read(UC_X86_REG_ECX);cleanup=0;result=this
  if a==0x99ec30:
   source=get(sp+4);value=get(source);event(1,this if BASE<=this<BASE+WIDTH else 0,value);cleanup=4;copies+=1
   if mode==1 and copies==1:replace(count+1,count+3)
   elif mode==4 and copies==2:replace(count+1,count+3)
   put(this,value)
  elif a==0x431606:
   output,pair=get(sp+4),get(sp+8);assert this==BASE+12;event(2,get(pair),get(pair+4),duplicate);cleanup=8;result=output;put(output,BASE+0xb00);put(output+4,0 if duplicate else 1)
   if not duplicate:put(BASE+0xa00,get(pair+4))
   if mode==2:replace(count,count+3);put(value_pointer,get(value_pointer)^0x101)
  elif a==0x99eae0:
   event(3,get(this));put(this,0)
   if mode==3:replace(count,count)
  else:
   position,value,tag,fill,atend=[get(sp+4+i*4)for i in range(5)];assert this==BASE and fill==1 and atend==1;event(4,position,get(value),1);cleanup=20
   n=(get(BASE+4)-get(BASE))//4;contents=bytes(u.mem_read(get(BASE),n*4));v=get(value);u.mem_write(BASE+0x600,contents);put(BASE+0x600+n*4,v);put(BASE,BASE+0x600);put(BASE+4,BASE+0x600+(n+1)*4);put(BASE+8,BASE+0x600+(n+2)*4)
  u.reg_write(UC_X86_REG_EAX,result);u.reg_write(UC_X86_REG_ESP,sp+4+cleanup);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook)
 for count in range(6):
  for spare,alias,duplicate,mode in itertools.product([0,3],[0,1],[0,1],range(5)):
   if alias and not count:continue
   arena=bytearray((i*19+cases*11)&255 for i in range(WIDTH));struct.pack_into('<3I',arena,0,BASE+0x200,BASE+0x200+count*4,BASE+0x200+(count+spare)*4);struct.pack_into('<I',arena,0x900,0x12345678);struct.pack_into('<I',arena,0xa00,0x87654321)
   value_pointer=BASE+(0x200 if alias else 0x900);inputs.extend(struct.pack('<5I',count,spare,alias,duplicate,mode));inputs.extend(arena);u.mem_write(BASE,bytes(arena));copies=0
   sp=BASE+0xe000;stop=BASE+0xf000;u.mem_write(sp,struct.pack('<2I',stop,value_pointer));u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,BASE);u.emu_start(0x98db20,stop,count=100000);assert u.reg_read(UC_X86_REG_ESP)==sp+8
   event(5);cases+=1
 return inputs,expected,dict(cases=cases,snapshots=snapshots)
