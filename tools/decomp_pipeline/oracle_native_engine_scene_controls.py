"""Engine scene/preload transitions and independent 2D/3D filter bits."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32
from unicorn.x86_const import *
from pe_oracle import EXE,pe_sections
BASE=0x24000000
def collect():
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000);u.mem_map(BASE,0x10000);raw=EXE.read_bytes()
 for _,va,_,off,n in pe_sections(raw):
  if n:u.mem_write(0x400000+va,raw[off:off+n])
 inputs=bytearray();expected=bytearray();cases=snapshots=0
 def event(kind,value=0):
  nonlocal snapshots
  expected.extend(struct.pack('<2I',kind,value));expected.extend(u.mem_read(BASE,512));snapshots+=1
 def call(address,*args):
  sp=BASE+0xe000;stop=BASE+0xf000;u.mem_write(sp,struct.pack('<'+'I'*(len(args)+1),stop,*args));u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,BASE);u.emu_start(address,stop,count=100);assert u.reg_read(UC_X86_REG_ESP)==sp+4+4*len(args);return u.reg_read(UC_X86_REG_EAX)&255
 values=(0,0x80000000,0x3f800000,0xbf800000,0x7f800000,0xff800000,0x7fc12345,0x7f812345,1,0x7f7fffff)
 for fill,new,preload,fast,unused,index,flags in itertools.product((0,1,0x55,0xff),range(2),range(2),range(2),range(2),range(len(values)),(0,1,2,3,0x80000000,0xffffffff)):
  arena=bytearray([fill])*512;arena[0x18]=new;arena[0x24]=preload;struct.pack_into('<I',arena,0xb8,flags);struct.pack_into('<I',arena,0x28,values[index]);struct.pack_into('<I',arena,0x2c,values[(index+3)%len(values)]);timeout=values[(index+7)%len(values)]
  inputs.extend(struct.pack('<4I',fast,unused,flags,timeout));inputs.extend(arena);u.mem_write(BASE,bytes(arena))
  event(1,call(0xb23a70));call(0xb23a10,fast,unused);event(2);event(3,call(0xb23a70));call(0xb239a0,flags,timeout);event(4);call(0xb23a10,1-fast,1-unused);event(5);event(6,call(0xb23a70))
  for kind,enable,setter,getter in [(7,fast,0xb23f90,0xb23fc0),(8,unused,0xb23fd0,0xb24000),(9,1-fast,0xb23f90,0xb23fc0),(10,1-unused,0xb23fd0,0xb24000)]:call(setter,enable);event(kind,call(getter))
  cases+=1
 return inputs,expected,dict(cases=cases,snapshots=snapshots)
