"""Retail renderer teardown, composed draw-vector destructors and callback replay."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from pe_oracle import EXE,pe_sections
BASE=0x24000000;WIDTH=0x6000
def scenarios():
 for op,n,mask,shader,control,mode in itertools.product([0,1],range(3),[0,1,7],[None,0,1,2],[None,0,1,2],range(4)):
  yield(op,n,mask,0 if shader is None else shader,int(shader is None),0 if control is None else control,int(control is None),mode)
 for op,n,mode in itertools.product([3,4,5],[0,2],range(4)):yield(op,n,7,1,0,1,0,mode)
 for n,mode in itertools.product(range(7),range(4)):yield(2,n,0,0,1,0,1,mode)
def collect(deleting_address=0x9e16e0):
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000);u.mem_map(BASE,0x10000);raw=EXE.read_bytes()
 for _,va,_,off,n in pe_sections(raw):
  if n:u.mem_write(0x400000+va,raw[off:off+n])
 inputs=bytearray();expected=bytearray();cases=snapshots=0
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def event(kind,a=0,b=0):
  nonlocal snapshots
  state=bytearray(u.mem_read(BASE,WIDTH));state[0x5104:0x5108]=b'\0'*4;state[0x5144:0x5148]=b'\0'*4
  expected.extend(struct.pack('<3I',kind,a,b));expected.extend(state);snapshots+=1
 def hook(u,a,n,_):
  if a not in [0x9f9f70,0x99b510,0xbfea14,0xbfe9bc,0xa620d0,0xa0b060,BASE+0x9100,BASE+0x9140]:return
  sp=u.reg_read(UC_X86_REG_ESP);this=u.reg_read(UC_X86_REG_ECX);cleanup=0
  if a==0x9f9f70:
   event(1,this);put(this,0)
   if mode==1:
    for v in [0x3e64,0x3e7c,0x3e94]:put(BASE+v,BASE+0x4800);put(BASE+v+4,BASE+0x4800)
  elif a==0x99b510:event(2,this);put(this,0)
  elif a==0xbfea14:
   p=get(sp+4);event(3,p)
   if mode==2:
    for v in [0x3e58,0x3e70,0x3e88]:put(BASE+v,BASE+0x4a00)
  elif a==BASE+0x9100:
   assert this==BASE+0x5000 and get(sp+4)==1;event(4,this,1);cleanup=4
   if mode==3:put(BASE+0x3e4c,BASE+0x5140)
  elif a==BASE+0x9140:
   event(5,this)
   if mode:put(BASE+0x3e4c,BASE+0x5140)
  elif a==0xbfe9bc:event(6,get(sp+4))
  elif a==0xa620d0:event(7,this);put(this,0x12344321)
  else:event(8,this);put(this,0x87654321)
  u.reg_write(UC_X86_REG_ESP,sp+4+cleanup);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook);put(BASE+0x8000,BASE+0x9100)
 for op,count,mask,shader,shader_null,control,control_null,mode in scenarios():
  arena=bytearray((i*13+cases*7)&255 for i in range(WIDTH))
  def pack(offset,*v):struct.pack_into('<'+'I'*len(v),arena,offset,*v)
  for j,(v,t)in enumerate([(0x3e58,0x3e64),(0x3e70,0x3e7c),(0x3e88,0x3e94)]):
   p=BASE+0x4000+j*0x80 if mask&(1<<j)else 0;pack(v,p,p,p)
   p=BASE+0x4200+j*0x200;pack(t,p,p+count*60,p+count*60)
  pack(0x3e50,0x11223344,0 if shader_null else BASE+0x5000);pack(0x5000,BASE+0x8000);pack(0x5010,shader)
  pack(0x3e48,BASE+0x5200,0 if control_null else BASE+0x5100);pack(0x5100,control,BASE+0x9140,BASE+0x5200);pack(0x5140,1,BASE+0x9140,BASE+0x5240)
  inputs.extend(struct.pack('<8I',op,count,mask,shader,shader_null,control,control_null,mode));inputs.extend(arena);u.mem_write(BASE,bytes(arena))
  deleting=op not in [0,2];flag={1:1,3:0,4:2,5:0xffffffff}.get(op,0)
  sp=BASE+0xe000;stop=BASE+0xf000;u.mem_write(sp,struct.pack('<2I',stop,flag));u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,BASE+(0x3e94 if op==2 else 0))
  u.emu_start(deleting_address if deleting else 0x9c2fd0 if op==0 else 0x9c2f50,stop,count=100000);assert u.reg_read(UC_X86_REG_ESP)==sp+4+(4 if deleting else 0)
  event(9,op,u.reg_read(UC_X86_REG_EAX)if deleting else 0);cases+=1
 return inputs,expected,dict(cases=cases,snapshots=snapshots)
