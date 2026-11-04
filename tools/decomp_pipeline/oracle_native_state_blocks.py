"""State-block library execution against the original renderer state arrays."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_EAX,UC_X86_REG_ESP,UC_X86_REG_EIP
from pe_oracle import EXE,pe_sections
BASE=0x24000000;WIDTH=0xa000
ENTRIES=[0x9df060,0x9df730,0x9dfda0,0x9e0410,0x9e0ae0]
class Runner:
 def __init__(self):
  self.u=Uc(UC_ARCH_X86,UC_MODE_32);self.u.mem_map(0x400000,0x1200000);self.u.mem_map(BASE,0x20000);raw=EXE.read_bytes()
  for _,va,_,off,size in pe_sections(raw):
   if size:self.u.mem_write(0x400000+va,raw[off:off+size])
  self.u.hook_add(UC_HOOK_CODE,self.hook)
 def read(self,a):return struct.unpack('<I',self.u.mem_read(a,4))[0]
 def word(self,a,v):self.u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def event(self,kind):self.events.append(struct.pack('<2I',kind,self.index)+bytes(self.u.mem_read(BASE,WIDTH)))
 def hook(self,u,a,n,_):
  if a!=0x9a4ec0:return
  self.event(1)
  if self.mutation:self.word(BASE+0x8208,BASE+(0x4000 if self.read(BASE+0x8208)==BASE else 0))
  sp=u.reg_read(UC_X86_REG_ESP);u.reg_write(UC_X86_REG_EAX,BASE+0x8000);u.reg_write(UC_X86_REG_EIP,self.read(sp));u.reg_write(UC_X86_REG_ESP,sp+4)
 def run(self,arena,mutation,reverse):
  u=self.u;self.events=[];self.mutation=mutation;u.mem_write(BASE,arena)
  order=list(reversed(range(5)))if reverse else list(range(5))
  for self.index in order+order:
   stack=BASE+0x1e000;stop=BASE+0x1f000;self.word(stack,stop);u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,0x1396530+self.index*4)
   u.emu_start(ENTRIES[self.index],stop,count=10000);assert u.reg_read(UC_X86_REG_ESP)==stack+4;self.event(2)
  return self.events
def scenarios():
 for case,(desired,mask,captured,dirty,side,mutation)in enumerate(itertools.product(range(4),(0,1,2,0x80000000,0xffffffff),range(3),range(2),range(2),range(2))):
  arena=bytearray((i*17+case*13)&255 for i in range(WIDTH))
  def put(o,v):struct.pack_into('<I',arena,o,v&0xffffffff)
  put(0x8060,BASE+0x8200);put(0x8208,BASE+side*0x4000)
  for base in (0,0x4000):
   put(base+0x2808,0);put(base+0x280c,0);put(base+0x2814,mask)
   for at in range(0x2818,0x3a24,20):
    put(base+at+4,desired if desired<3 else (at*13+case)&0xffffffff)
    put(base+at+8,0 if captured==0 else mask if captured==1 else mask^0xffffffff)
    arena[base+at+16]=dirty
    if dirty:
     count=struct.unpack_from('<I',arena,base+0x280c)[0];put(base+0x2008+count*4,BASE+base+at);put(base+0x280c,count+1)
  yield mutation,case&1,bytes(arena)
