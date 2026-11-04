"""Retail texture assignment, cached binding and full Uninitialise semantics."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_EAX,UC_X86_REG_ESP,UC_X86_REG_EIP
from pe_oracle import EXE,pe_sections
BASE=0x24000000;WIDTH=0x5000
def collect():
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000);u.mem_map(BASE,0x10000);raw=EXE.read_bytes()
 for _,va,_,off,size in pe_sections(raw):
  if size:u.mem_write(0x400000+va,raw[off:off+size])
 inputs=bytearray();expected=bytearray();cases=snapshots=0
 def read(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def word(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def event(kind,receiver=0,stage=0,texture=0):
  nonlocal snapshots
  expected.extend(struct.pack('<5I',kind,phase,receiver,stage,texture));expected.extend(u.mem_read(BASE,WIDTH));snapshots+=1
 def hook(u,a,n,_):
  nonlocal calls
  if a not in (BASE+0xc000,BASE+0xc100,BASE+0xc200):return
  kind={BASE+0xc000:1,BASE+0xc100:2,BASE+0xc200:3}[a];sp=u.reg_read(UC_X86_REG_ESP);stage=read(sp+8)if kind==3 else 0
  event(kind,read(sp+4),stage,read(sp+12)if kind==3 else 0);calls+=1
  if mutation:
   word(BASE+0x4004,read(BASE+0x4004)^0x76543210);word(BASE+0x4014,read(BASE+0x4014)+0x01000011)
   if phase==0:word(BASE+0x4010,BASE+(0x4100 if calls&1 else 0x4120))
   if kind==3:word(BASE+0x3d00+stage*4,BASE+0x4120);word(BASE+0x3cb8,0xffffffff if calls&1 else 8)
  u.reg_write(UC_X86_REG_EAX,(0x80004005 if mutation else 0)if kind==3 else(7 if kind==1 else 9))
  u.reg_write(UC_X86_REG_EIP,read(sp));u.reg_write(UC_X86_REG_ESP,sp+(16 if kind==3 else 8))
 u.hook_add(UC_HOOK_CODE,hook);word(BASE+0x8004,BASE+0xc100);word(BASE+0x8008,BASE+0xc000);word(BASE+0x8400+65*4,BASE+0xc200)
 for live_dest,live_source,self,stage,stages,cached,mutation in itertools.product(range(2),range(2),range(2),(0,1,3,7),(-1,0,8),range(2),range(2)):
  arena=bytearray((i*17+cases*13)&255 for i in range(WIDTH))
  def put(o,v):struct.pack_into('<I',arena,o,v&0xffffffff)
  put(0x4000,BASE+0x4100 if live_dest else 0);put(0x4010,BASE+0x4120 if live_source else 0)
  put(0x4100,BASE+0x8000);put(0x4120,BASE+0x8000);put(0x4200,BASE+0x8400);put(0x3cf0,BASE+0x4200);put(0x3cb8,stages)
  put(0x3d00+stage*4,BASE+0x4120 if cached else 0)
  inputs.extend(struct.pack('<3I',mutation,self,stage));inputs.extend(arena);u.mem_write(BASE,bytes(arena));calls=0;source=BASE+(0x4000 if self else 0x4010)
  sequence=[(0x9fa1c0,BASE+0x4000,(source,)),(0x9a0cf0,BASE,(stage,BASE+0x4000)),(0x9a0cf0,BASE,(stage,BASE+0x4000)),(0x9f9f70,BASE+0x4000,()),(0x9a0cf0,BASE,(stage,BASE+0x4000)),(0x9f9f70,source,())]
  for phase,(address,receiver,args)in enumerate(sequence):
   stack=BASE+0xe000;stop=BASE+0xf000;u.mem_write(stack,struct.pack('<'+'I'*(len(args)+1),stop,*args));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,receiver)
   u.emu_start(address,stop,count=10000);assert u.reg_read(UC_X86_REG_ESP)==stack+4+len(args)*4;event(4,u.reg_read(UC_X86_REG_EAX)if phase==0 else 0)
  cases+=1
 return inputs,expected,dict(cases=cases,snapshots=snapshots)
