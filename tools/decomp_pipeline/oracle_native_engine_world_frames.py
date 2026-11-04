"""Engine frame dispatch and unsigned-rate x87 time accumulation."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from pe_oracle import EXE,pe_sections
BASE=0x24000000;WIDTH=2048
def collect():
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000);u.mem_map(BASE,0x10000);raw=EXE.read_bytes()
 for _,va,_,off,n in pe_sections(raw):
  if n:u.mem_write(0x400000+va,raw[off:off+n])
 inputs=bytearray();expected=bytearray();cases=snapshots=0
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def event(kind,self=0,arg=0):
  nonlocal snapshots
  expected.extend(struct.pack('<3I',kind,self,arg));expected.extend(u.mem_read(BASE,WIDTH));snapshots+=1
 def hook(u,a,n,_):
  if a not in (BASE+0x9100,BASE+0x9200,BASE+0x9300):return
  sp=u.reg_read(UC_X86_REG_ESP);self=u.reg_read(UC_X86_REG_ECX);assert BASE+0x400<=self<BASE+0x560;idx=((self-BASE)&255)//32;cleanup=0;value=0
  if a==BASE+0x9100:
   event(1,self);value=get(self+4)
   if mode==1:put(BASE+0xb8,get(BASE+0xb8)^1);put(BASE+0x600+idx*4,BASE+0x500+idx*32)
   if mode==3:u.mem_write(BASE+0x84,b'\0');put(BASE+0x88,0)
  else:
   paused=a==BASE+0x9300;event(3 if paused else 2,self,0 if paused else get(sp+4)&255);cleanup=0 if paused else 4
   if mode==2:
    put(BASE+0xb0,2);put(BASE+(0x90 if paused else 0x8c),0xffffffff);u.mem_write(BASE+(0xa0 if paused else 0x98),struct.pack('<d',1.0));put(BASE+0x16c,BASE+0x600+(idx+1)*4)
  u.reg_write(UC_X86_REG_EAX,value);u.reg_write(UC_X86_REG_ESP,sp+4+cleanup);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook)
 for off,target in [(0x28,0x9100),(0x1c,0x9200),(0x20,0x9300)]:put(BASE+0x8000+off,BASE+target)
 def run(fill,active,render,count,mask,op,first,control,fps=30,time=0x3fd0000000000000,counter=0x7fffffff):
  nonlocal cases
  arena=bytearray([fill])*WIDTH
  def field(off,value):struct.pack_into('<I',arena,off,value&0xffffffff)
  arena[0x84]=active
  for off,value in [(0x88,render),(0x8c,counter),(0x90,counter),(0xb0,fps),(0xb8,mask),(0x168,BASE+0x600),(0x16c,BASE+0x600+count*4),(0x170,BASE+0x640)]:field(off,value)
  struct.pack_into('<Q',arena,0x98,time);struct.pack_into('<Q',arena,0xa0,time)
  for i in range(3):
   field(0x600+i*4,BASE+0x400+i*32)
   for obj in (0x400+i*32,0x500+i*32):field(obj,BASE+0x8000);field(obj+4,(1,2,0xffffffff)[i])
  inputs.extend(struct.pack('<4I',mode,op,first,control));inputs.extend(arena);u.mem_write(BASE,bytes(arena));sp=BASE+0xe000;stop=BASE+0xf000;put(sp,stop);put(sp+4,first);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,BASE);u.reg_write(UC_X86_REG_FPCW,control);u.reg_write(UC_X86_REG_FPTAG,0xffff);u.reg_write(UC_X86_REG_FPSW,0)
  u.emu_start(0xb24030 if op==0 else 0xb240c0,stop,count=1000);assert u.reg_read(UC_X86_REG_ESP)==sp+(8 if op==0 else 4);assert u.reg_read(UC_X86_REG_FPCW)==control;event(4);cases+=1
 for fill,active,render,count,mask,mode,op,first,control in itertools.product((0,1,0x55,0xff),range(2),(0,1,0xffffffff),range(4),(0,1,2,0xffffffff),range(4),range(2),range(2),(0x27f,0x37f)):
  run(fill,active,render,count,mask,op,first,control)
 dispatch_cases=cases;mode=0
 times=(0,0x8000000000000000,0x3ff0000000000000,0xbff0000000000000,0x7fefffffffffffff,1,0x7ff0000000000000,0xfff0000000000000,0x7ff8123456789abc,0x7ff0123456789abc)
 for fps,time,counter,op,control in itertools.product((0,1,2,30,60,16777217,0x7fffffff,0x80000000,0xffffffff),times,(0,1,0x7fffffff,0x80000000,0xffffffff),range(2),(0x27f,0x37f)):
  run(cases&255,0,0,0,0,op,0,control,fps,time,counter)
 return inputs,expected,dict(cases=cases,snapshots=snapshots,dispatch_cases=dispatch_cases,arithmetic_cases=cases-dispatch_cases)
