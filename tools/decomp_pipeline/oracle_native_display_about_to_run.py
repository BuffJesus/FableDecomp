"""Retail initial fade setup, observing mutation across the update boundary."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from pe_oracle import EXE,pe_sections
BASE=0x24000000;WIDTH=4096;GLOBAL=0x1375cdc
def collect(actual_fade=False):
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000);u.mem_map(BASE,0x10000);raw=EXE.read_bytes()
 for _,va,_,off,n in pe_sections(raw):
  if n:u.mem_write(0x400000+va,raw[off:off+n])
 inputs=bytearray();expected=bytearray();cases=snapshots=0
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def put(a,v):u.mem_write(a,struct.pack('<I',v))
 def event(kind,a=0,b=0):
  nonlocal snapshots
  expected.extend(struct.pack('<4I',kind,a,b,u.mem_read(GLOBAL,1)[0]));expected.extend(u.mem_read(BASE,WIDTH));snapshots+=1
 def hook(u,a,n,_):
  if a not in (0x434870,0x9d8250):return
  if actual_fade and a==0x434870:return
  sp=u.reg_read(UC_X86_REG_ESP);this=u.reg_read(UC_X86_REG_ECX)
  if a==0x434870:
   event(1,this,get(sp+4));assert this==BASE+0xbc and get(sp+4)==0x3dcccccd
   if mode==1:u.mem_write(BASE+0xbc,bytes([0xa5])*28)
   elif mode==2:u.mem_write(BASE+0xd8,b'\x01');u.mem_write(GLOBAL,b'\0')
   elif mode==3:put(BASE+0xf0,0x7fc12345);put(BASE+0xf4,0x12345678);u.mem_write(BASE+0xec,b'\0')
   cleanup=4
  else:
   event(2,this);assert this==1;cleanup=0
   if mode==4:u.mem_write(BASE+0xec,b'\0');put(BASE+0xf4,0x87654321)
  u.reg_write(UC_X86_REG_EAX,0xabcdef);u.reg_write(UC_X86_REG_ESP,sp+4+cleanup);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook)
 for fill,locked,allowed,mode in itertools.product(range(256),range(2),range(2),range(1) if actual_fade else range(5)):
  arena=bytearray([fill])*WIDTH;arena[0xd8]=locked;u.mem_write(BASE,bytes(arena));u.mem_write(GLOBAL,bytes([allowed]));inputs.extend(struct.pack('<2I',mode,allowed));inputs.extend(arena)
  sp=BASE+0xe000;stop=BASE+0xf000;put(sp,stop);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,BASE);u.emu_start(0x434cd0,stop,count=1000);assert u.reg_read(UC_X86_REG_ESP)==sp+4;event(3);cases+=1
 return inputs,expected,dict(cases=cases,snapshots=snapshots)
