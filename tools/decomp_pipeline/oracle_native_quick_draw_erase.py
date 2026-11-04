"""Retail draw-record erasure with string and COM callback boundaries."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_EAX,UC_X86_REG_ESP,UC_X86_REG_EIP
from pe_oracle import EXE,pe_sections
BASE=0x24000000;WIDTH=0x1000

def collect():
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000);u.mem_map(BASE,0x10000);raw=EXE.read_bytes()
 for _,va,_,off,size in pe_sections(raw):
  if size:u.mem_write(0x400000+va,raw[off:off+size])
 inputs=bytearray();expected=bytearray();cases=snapshots=0
 def read(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def word(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def event(kind,receiver=0,source=0):
  nonlocal snapshots
  expected.extend(struct.pack('<3I',kind,receiver,source));expected.extend(u.mem_read(BASE,WIDTH));snapshots+=1
 def hook(u,a,n,_):
  if a not in (0x99b7d0,0x99b510,BASE+0xc000,BASE+0xc100):return
  kind={0x99b7d0:1,0x99b510:2,BASE+0xc000:3,BASE+0xc100:4}[a]
  sp=u.reg_read(UC_X86_REG_ESP);receiver=u.reg_read(UC_X86_REG_ECX)if kind<3 else read(sp+4);source=read(sp+4)if kind==1 else 0
  event(kind,receiver,source)
  if mutation:
   for i in range(8):word(BASE+0x100+i*60+28,read(BASE+0x100+i*60+28)^0x73450123)
  if kind==1:word(receiver,read(source))
  if kind==2:word(receiver,0)
  u.reg_write(UC_X86_REG_EAX,receiver if kind==1 else 7)
  u.reg_write(UC_X86_REG_EIP,read(sp));u.reg_write(UC_X86_REG_ESP,sp+(4 if kind==2 else 8))
 u.hook_add(UC_HOOK_CODE,hook);word(BASE+0x8004,BASE+0xc100);word(BASE+0x8008,BASE+0xc000)
 for length in range(9):
  for first in range(length+1):
   for last in range(first,length+1):
    for live in range(3):
     for mutation in range(2):
      arena=bytearray((i*17+cases*13)&255 for i in range(WIDTH))
      def put(o,v):struct.pack_into('<I',arena,o,v&0xffffffff)
      put(0,BASE+0x100);put(4,BASE+0x100+length*60);put(8,BASE+0x100+8*60)
      for i in range(8):
       put(0x100+i*60+20,BASE+0x600+(i%3)*16)
       put(0x100+i*60+24,BASE+0x700+(i%2)*16 if live==2 or (live==1 and i%2) else 0)
      put(0x700,BASE+0x8000);put(0x710,BASE+0x8000)
      inputs.extend(struct.pack('<3I',mutation,first,last));inputs.extend(arena);u.mem_write(BASE,bytes(arena))
      stack=BASE+0xe000;stop=BASE+0xf000;u.mem_write(stack,struct.pack('<3I',stop,BASE+0x100+first*60,BASE+0x100+last*60));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,BASE)
      u.emu_start(0x9e15e0,stop,count=10000);assert u.reg_read(UC_X86_REG_ESP)==stack+12;event(5,u.reg_read(UC_X86_REG_EAX));cases+=1
 return inputs,expected,dict(cases=cases,snapshots=snapshots)
