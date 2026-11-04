"""Display readiness countdown and live engine draw-distance query."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from pe_oracle import EXE,pe_sections
BASE=0x24000000;WIDTH=4096
GLOBALS=[0x13b89d8,0x13b89dc,0x13b8774]
def collect():
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000);u.mem_map(BASE,0x10000);raw=EXE.read_bytes()
 for _,va,_,off,n in pe_sections(raw):
  if n:u.mem_write(0x400000+va,raw[off:off+n])
 inputs=bytearray();expected=bytearray();cases=snapshots=0
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def event(kind,a=0,b=0,c=0):
  nonlocal snapshots
  expected.extend(struct.pack('<4I',kind,a,b,c))
  for address in GLOBALS:expected.extend(u.mem_read(address,4))
  expected.extend(u.mem_read(BASE,WIDTH));snapshots+=1
 def hook(u,a,n,_):
  if a not in [0x9e9f20,0x99b800,0x434cd0,BASE+0x9100]:return
  sp=u.reg_read(UC_X86_REG_ESP);this=u.reg_read(UC_X86_REG_ECX);cleanup=0
  if a==0x9e9f20:
   event(1,this&255,u.reg_read(UC_X86_REG_EDX)&255)
   if mode==1:put(BASE+0xe8,2)
   elif mode==2:put(BASE+0x2c,0)
   elif mode==3:put(BASE+0xe8,0xffffffff)
  elif a==0x99b800:
   assert get(sp+4)==0x122d70c;label=GLOBALS.index(this)+1;event(2,label);put(this,BASE+0xa00+label*16);cleanup=4
   if mode==4:put(BASE+0xe8,2 if label==1 else 0)
   elif mode==5 and label==2:put(BASE+0x2c,BASE+0x440)
  elif a==0x434cd0:
   event(3,this)
   if mode==6:put(BASE+0x2c,0)
   elif mode==7:put(BASE+0xe8,0);put(BASE+0x2c,BASE+0x440)
  else:
   event(4,this);put(BASE+0x9200,get(BASE+(0x604 if this==BASE+0x440 else 0x600)));return
  u.reg_write(UC_X86_REG_EAX,this);u.reg_write(UC_X86_REG_ESP,sp+4+cleanup);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook);put(BASE+0x804c,BASE+0x9100)
 u.mem_write(BASE+0x9100,b'\xd9\x05'+struct.pack('<I',BASE+0x9200)+b'\xc3')
 for count,present,value,mode,control in itertools.product([0,1,2,30,0x7fffffff,0x80000000,0xffffffff],range(2),[0,0x80000000,0x3f800000,0xbf800000,0x3fc00000,0x7f800000,0xff800000,0x7fc12345,0x7f812345,1],range(8),[0x27f,0x37f]):
  arena=bytearray([cases&255])*WIDTH
  for off,v in [(0xe8,count),(0x2c,BASE+0x400 if present else 0),(0x400,BASE+0x8000),(0x440,BASE+0x8000),(0x600,value),(0x604,0x40600000)]:struct.pack_into('<I',arena,off,v)
  state=struct.pack('<3I',BASE+0xa80,BASE+0xa90,0x55aa7788);inputs.extend(struct.pack('<2I',mode,control));inputs.extend(state);inputs.extend(arena);u.mem_write(BASE,bytes(arena))
  for i,a in enumerate(GLOBALS):u.mem_write(a,state[i*4:i*4+4])
  sp=BASE+0xe000;stop=BASE+0xf000;put(sp,stop);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,BASE);u.reg_write(UC_X86_REG_FPCW,control);u.reg_write(UC_X86_REG_FPTAG,0xffff);u.reg_write(UC_X86_REG_FPSW,0)
  u.emu_start(0x434f60,stop,count=10000);assert u.reg_read(UC_X86_REG_ESP)==sp+4 and u.reg_read(UC_X86_REG_FPCW)==control;event(5);cases+=1
 return inputs,expected,dict(cases=cases,snapshots=snapshots)
