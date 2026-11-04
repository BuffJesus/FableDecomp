"""Actual retail shader-layout construction and scalar deletion."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_EAX,UC_X86_REG_ESP,UC_X86_REG_EIP
from pe_oracle import EXE,pe_sections
BASE=0x24000000;WIDTH=4096
OFFSETS=[0,4,16,28,40,52,64,80,92,104,116,128,140,152,164,176,188,200,212,224,236,248,260,272,284,296,308]
def collect():
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000);u.mem_map(BASE,0x10000);raw=EXE.read_bytes()
 for _,va,_,off,size in pe_sections(raw):
  if size:u.mem_write(0x400000+va,raw[off:off+size])
 inputs=bytearray();expected=bytearray();cases=snapshots=0
 def read(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def word(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def event(kind,pointer=0):
  nonlocal snapshots
  expected.extend(struct.pack('<2I',kind,pointer));expected.extend(u.mem_read(BASE,WIDTH));snapshots+=1
 def hook(u,a,n,_):
  if a not in (0xbfea14,0xbfe9bc):return
  sp=u.reg_read(UC_X86_REG_ESP);event(2 if a==0xbfea14 else 3,read(sp+4))
  if a==0xbfea14 and mutation:word(BASE+0x140,0x55667788);word(BASE+8,0x87654321)
  u.reg_write(UC_X86_REG_EIP,read(sp));u.reg_write(UC_X86_REG_ESP,sp+4)
 u.hook_add(UC_HOOK_CODE,hook)
 for operation,count,mutation,seed in itertools.product(range(4),(0,1,4),range(2),range(13)):
  arena=bytes((i*17+cases*13)&255 for i in range(WIDTH));inputs.extend(struct.pack('<3I',operation,count,mutation));inputs.extend(arena);u.mem_write(BASE,arena)
  for address,receiver,args in [(0x98d490,BASE,()),(0x98d990 if operation<2 else 0x98c390,BASE+(0 if operation<2 else OFFSETS[1+cases%26]),(operation&1,))]:
   stack=BASE+0xe000;stop=BASE+0xf000;u.mem_write(stack,struct.pack('<'+'I'*(len(args)+1),stop,*args));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,receiver)
   u.emu_start(address,stop,count=10000);assert u.reg_read(UC_X86_REG_ESP)==stack+4+4*len(args)
   event(1 if address==0x98d490 else 4,0 if address==0x98d490 else u.reg_read(UC_X86_REG_EAX))
   if address==0x98d490:
    word(BASE+0x140,BASE+0x400 if count else 0);word(BASE+0x144,BASE+0x400+count*16 if count else 0);word(BASE+0x148,read(BASE+0x144))
  cases+=1
 return inputs,expected,dict(cases=cases,snapshots=snapshots)
