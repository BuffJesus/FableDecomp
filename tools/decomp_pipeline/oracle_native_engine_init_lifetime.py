"""Typed engine-init lifetime, callback mutations and actual empty strings."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from pe_oracle import EXE,pe_sections
BASE=0x24000000;COUNTER=0x13bca20
def collect(composed=False):
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000);u.mem_map(BASE,0x10000);raw=EXE.read_bytes()
 for _,va,_,off,n in pe_sections(raw):
  if n:u.mem_write(0x400000+va,raw[off:off+n])
 inputs=bytearray();expected=bytearray();cases=snapshots=0
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def event(kind,label=0):
  nonlocal snapshots
  expected.extend(struct.pack('<3I',kind,label,get(COUNTER)));expected.extend(u.mem_read(BASE,256));snapshots+=1
 def hook(u,a,n,_):
  if composed or a not in (0x99aed0,0x99b510):return
  sp=u.reg_read(UC_X86_REG_ESP);this=u.reg_read(UC_X86_REG_ECX);label=(this-BASE-0x20)//4;assert 0<=label<5
  kind=1 if a==0x99aed0 else 2;event(kind,label);put(this,BASE+0x300+label*16 if kind==1 else 0)
  if kind==1:
   if mode==1:put(BASE+0x34,0x12345678);put(BASE+0x60,0x88776655)
   elif mode==2:put(BASE+label*4,BASE+0x700+label*16)
   elif mode==3 and label<4:put(this+4,0xabcdef01)
  else:
   if mode==1 and label<4:put(this+4,0x12345678)
   elif mode==2:put(BASE+0x5c,0xdeadbeef)
   elif mode==3 and label>0:put(this-4,BASE+0x600+label*16)
  u.reg_write(UC_X86_REG_EAX,this);u.reg_write(UC_X86_REG_ESP,sp+4);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook)
 def call(a):
  sp=BASE+0xe000;stop=BASE+0xf000;put(sp,stop);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,BASE);u.emu_start(a,stop,count=10000);assert u.reg_read(UC_X86_REG_ESP)==sp+4
 rows=((fill,0,2)for fill in range(256))if composed else itertools.product(range(256),range(4),range(3))
 for fill,mode,operation in rows:
  inputs.extend(struct.pack('<3I',fill,mode,operation));u.mem_write(BASE,bytes([fill])*256);put(COUNTER,fill)
  if operation!=1:call(0x42d830);assert u.reg_read(UC_X86_REG_EAX)==BASE;event(3)
  if operation!=0:call(0x42f7c1);event(4)
  cases+=1
 return inputs,expected,dict(cases=cases,snapshots=snapshots)
