"""Texture adoption and byte accounting with hostile-but-defined callbacks."""
import itertools,struct
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
 def event(kind,receiver=0,value=0):
  nonlocal snapshots
  expected.extend(struct.pack('<3I',kind,receiver,value));expected.extend(u.mem_read(BASE,WIDTH));snapshots+=1
 def hook(u,a,n,_):
  nonlocal calls
  if a not in (BASE+0xc000,BASE+0xc100,BASE+0xc200,0x9e3830,0x9e3820):return
  kind={BASE+0xc000:1,BASE+0xc100:2,BASE+0xc200:3,0x9e3830:4,0x9e3820:5}[a];sp=u.reg_read(UC_X86_REG_ESP)
  receiver=read(sp+4)if kind<4 else u.reg_read(UC_X86_REG_ECX);value=read(sp+8)if kind==3 else read(sp+4)if kind==4 else read(receiver)if kind==5 else 0
  event(kind,receiver if kind<4 else 0,value);calls+=1
  if mutation:
   word(BASE+4,read(BASE+4)^0xa7654321)
   if kind in (2,3):word(BASE,BASE+(0x200 if calls&1 else 0x100))
  result=7
  if kind==2:result=levels
  elif kind==3:
   width,height=((0,17),(257>>value,129>>value),(0xfffffff1-value,0x81234567+value))[dimensions]
   words=[0x15+(calls%3),1,2,3,4,5,width,height];u.mem_write(read(sp+12),struct.pack('<8I',*(v&0xffffffff for v in words)));result=0x80004005 if mutation else 0
  elif kind==4:word(receiver,value^0x13579)
  elif kind==5:result=(0,1,8,24,32,0x10000001)[value%6]
  u.reg_write(UC_X86_REG_EAX,result&0xffffffff);u.reg_write(UC_X86_REG_EIP,read(sp));u.reg_write(UC_X86_REG_ESP,sp+4+({1:1,2:1,3:3,4:1,5:0}[kind])*4)
 u.hook_add(UC_HOOK_CODE,hook);word(BASE+0x8008,BASE+0xc000);word(BASE+0x8000+13*4,BASE+0xc100);word(BASE+0x8000+17*4,BASE+0xc200)
 for old,new,levels,dimensions,mutation,owner in itertools.product(range(2),range(2),(-1,0,1,3),range(3),range(2),(0,1,15)):
  arena=bytearray((i*17+cases*13)&255 for i in range(WIDTH))
  def put(o,v):struct.pack_into('<I',arena,o,v&0xffffffff)
  put(0,BASE+0x100 if old else 0);put(4,(owner<<28)|0x7654321);put(0x100,BASE+0x8000);put(0x200,BASE+0x8000)
  inputs.extend(struct.pack('<5I',new,levels&0xffffffff,dimensions,mutation,0));inputs.extend(arena);u.mem_write(BASE,bytes(arena));calls=0
  for texture in (BASE+0x200 if new else 0,0):
   stack=BASE+0xe000;stop=BASE+0xf000;u.mem_write(stack,struct.pack('<2I',stop,texture));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,BASE)
   u.emu_start(0x9fa230,stop,count=10000);assert u.reg_read(UC_X86_REG_ESP)==stack+8;event(6,0,u.reg_read(UC_X86_REG_EAX)&255)
  cases+=1
 return inputs,expected,dict(cases=cases,snapshots=snapshots)
