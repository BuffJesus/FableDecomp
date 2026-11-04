"""Native simple-pool owner construction, cleanup, and both deleting flags."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from pe_oracle import EXE,pe_sections
BASE=0x24000000;WIDTH=0x1000
def scenarios():
 for capacity,growth,fill,mode in itertools.product([0,1,15,16,17,0xfffffff0,0xfffffff1,0xffffffff],[0,1,15,16,17,0xfffffff0,0xfffffff1,0xffffffff],[0,0x55,0xff],range(3)):yield(0,0,0,0,mode,capacity,growth,fill)
 for op in [1,2,3]:
  for n,owned,flag,mode in itertools.product(range(7),[0,1],[0,1,2,0xffffffff]if op==3 else [0],range(4)):yield(op,n,owned,flag,mode,0,0,(n*37+mode*11)&255)
def collect():
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000);u.mem_map(BASE,0x10000);raw=EXE.read_bytes()
 for _,va,_,off,n in pe_sections(raw):
  if n:u.mem_write(0x400000+va,raw[off:off+n])
 inputs=bytearray();expected=bytearray();cases=snapshots=deletes=0
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def event(kind,a=0,b=0,c=0):
  nonlocal snapshots
  expected.extend(struct.pack('<4I',kind,a,b,c));expected.extend(u.mem_read(BASE,WIDTH));snapshots+=1
 def hook(u,a,n,_):
  nonlocal deletes
  if a not in [0x99a2f0,0x99a300,BASE+0x9100,0xbfe9bc]:return
  sp=u.reg_read(UC_X86_REG_ESP);this=u.reg_read(UC_X86_REG_ECX);cleanup=0;result=this
  if a==0x99a2f0:
   event(1,this);put(this,0x129a7c4)
   if mode==1:put(BASE+24,0x11223344);put(BASE+8,0x55667788)
   elif mode==2:put(BASE+4,BASE+0x500);put(BASE+12,0x98765432)
  elif a==0x99a300:event(2,this);put(this,0x129a7c4)
  elif a==BASE+0x9100:
   assert get(sp+4)==1;event(3,this,1);cleanup=4;deletes+=1;put(this+8,0x65430000|deletes)
   if mode==1:put(this+20,0)
   elif mode==2 and get(this+20):u.mem_write(get(this+20)+12,b'\0')
   elif mode==3:put(BASE+4,BASE+0x500);put(BASE+16,0x12344321)
  else:event(4,get(sp+4))
  u.reg_write(UC_X86_REG_EAX,result);u.reg_write(UC_X86_REG_ESP,sp+4+cleanup);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook);put(BASE+0x8000,BASE+0x9100)
 for op,count,owned,flag,mode,capacity,growth,fill in scenarios():
  arena=bytearray([fill])*WIDTH
  if op:
   struct.pack_into('<I',arena,4,BASE+0x100 if count else 0)
   for i in range(count):
    off=0x100+i*0x30;struct.pack_into('<I',arena,off,BASE+0x8000);arena[off+12]=owned if i==0 else 1;struct.pack_into('<I',arena,off+20,BASE+off+0x30 if i+1<count else 0)
  inputs.extend(struct.pack('<7I',op,count,owned,flag,mode,capacity,growth));inputs.extend(arena);u.mem_write(BASE,bytes(arena));deletes=0
  args=[capacity,growth]if op==0 else [flag]if op==3 else [];sp=BASE+0xe000;stop=BASE+0xf000;u.mem_write(sp,struct.pack('<'+'I'*(len(args)+1),stop,*args));u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,BASE);u.emu_start([0xa5c570,0x987f10,0xa5c600,0x987f30][op],stop,count=100000);assert u.reg_read(UC_X86_REG_ESP)==sp+4+len(args)*4
  event(5,op,u.reg_read(UC_X86_REG_EAX)if op in [0,3]else 0);cases+=1
 return inputs,expected,dict(cases=cases,snapshots=snapshots)
