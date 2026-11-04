"""Shared engine component lifetime, owner links and default virtual methods."""
import itertools,json,struct
from pathlib import Path
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from pe_oracle import EXE,pe_sections
BASE=0x24000000
SPECS=json.loads(Path(__file__).with_name('native_engine_component.json').read_text())
def collect():
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000);u.mem_map(BASE,0x10000);raw=EXE.read_bytes()
 for _,va,_,off,n in pe_sections(raw):
  if n:u.mem_write(0x400000+va,raw[off:off+n])
 inputs=bytearray();expected=bytearray();cases=snapshots=0
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def event(kind,self=0,arg=0):
  nonlocal snapshots
  expected.extend(struct.pack('<3I',kind,self,arg));expected.extend(u.mem_read(BASE,1024));snapshots+=1
 def hook(u,a,n,_):
  if a not in (0xbfe9bc,0x99ebf0):return
  sp=u.reg_read(UC_X86_REG_ESP);cleanup=0
  if a==0xbfe9bc:
   self=get(sp+4);assert self==BASE;event(1,self)
   if mode:put(BASE+4,BASE+0x420);put(BASE+0x400-4,0x11223344)
  else:
   self=u.reg_read(UC_X86_REG_ECX);assert self==BASE+0x200 and get(sp+4)==0x122d70e and get(sp+8)==0xffffffff;event(2,self,get(sp+8));put(self,BASE+0x300);cleanup=8
   if mode:put(BASE+8,0x87654321)
  u.reg_write(UC_X86_REG_EAX,self);u.reg_write(UC_X86_REG_ESP,sp+4+cleanup);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook)
 def call(address,*args):
  sp=BASE+0xe000;stop=BASE+0xf000;u.mem_write(sp,struct.pack('<'+'I'*(len(args)+1),stop,*args));u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,BASE);u.emu_start(address,stop,count=100);assert u.reg_read(UC_X86_REG_ESP)==sp+4+len(args)*4;return u.reg_read(UC_X86_REG_EAX)
 owners=(0,BASE+0x380,BASE,BASE+4,BASE+8)
 scenarios=[]
 for fill in range(256):
  scenarios.append((fill,0,0,0,0))
  for owner in owners:
   scenarios.append((fill,1,0,owner,0))
   for flags,mode in itertools.product((0,1,2,0xffffffff),range(2)):scenarios.append((fill,3,mode,owner,flags))
   for flags,mode in itertools.product((0,1),range(2)):scenarios.append((fill,6,mode,owner,flags))
  for owner in owners+(0xffffffff,):scenarios.append((fill,2,0,owner,0))
  for mode in range(2):scenarios.append((fill,4,mode,0,0))
  for pattern in range(4):scenarios.append((fill,5,0,0,pattern))
 for fill,op,mode,owner,parameter in scenarios:
  arena=bytearray([fill])*1024;struct.pack_into('<I',arena,4,owner);inputs.extend(struct.pack('<4I',op,mode,owner,parameter));inputs.extend(arena);u.mem_write(BASE,bytes(arena))
  if op==0:event(3,call(0xb59710))
  elif op==1:call(0xb59730);event(4)
  elif op==2:call(0xb59750,owner);event(5)
  elif op==3:event(6,call(0xb59780,parameter))
  elif op==4:event(7,call(0xb28ca0,BASE+0x200))
  elif op==5:
   arg=(BASE+0x300,0,0xffffffff,0x12345678)[parameter]
   for index,e in enumerate(SPECS[:10]):
    value=call(e['address'],*([arg]if e['param']else []));event(10+index,0,value&255 if e['ret']=='bool'else 0)
  else:
   event(3,call(0xb59710));call(0xb59750,owner);event(5);event(6,call(0xb59780,parameter))
  cases+=1
 return inputs,expected,dict(cases=cases,snapshots=snapshots)
