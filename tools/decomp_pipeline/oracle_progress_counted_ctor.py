"""Independent retail counted progress-display constructor allocation/callback model."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_EAX
from pe_oracle import EXE,pe_sections
BASE=0x2004000;WIDTH=0x8000

def collect():
 raw=EXE.read_bytes();uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1200000)
 for _,va,vs,offset,size in pe_sections(raw):
  if size:uc.mem_write(0x400000+va,raw[offset:offset+size])
 uc.mem_map(0x2000000,0x20000);rows=[];events=[];args=();stack=0x2010000;stop=0x200f000
 def word(a,v):uc.mem_write(a,struct.pack('<I',v&0xffffffff))
 def read(a):return struct.unpack('<I',uc.mem_read(a,4))[0]
 def hook(uc,a,n,user):
  if a!=0xbfea1a:return
  sp=uc.reg_read(UC_X86_REG_ESP);assert read(sp+4)==12;events.append(bytes(uc.mem_read(BASE,WIDTH)))
  if args[2]:word(BASE,0 if args[2]==1 else BASE+0x2000);word(BASE+4,0x12345678)
  uc.reg_write(UC_X86_REG_EAX,args[1]);uc.reg_write(UC_X86_REG_EIP,read(sp));uc.reg_write(UC_X86_REG_ESP,sp+4)
 uc.hook_add(UC_HOOK_CODE,hook)
 for args in itertools.product([0,BASE+0x1000],[0,BASE+0x3000],[0,1,2],[0,BASE+0x4000]):
  value,block,mutation,old=args;initial=bytearray(b'\xa5'*WIDTH);struct.pack_into('<2I',initial,0,0xabcdef01,old);expected=bytearray(initial);struct.pack_into('<I',expected,0,value);predicted=[]
  if value:
   predicted.append(bytes(expected))
   if mutation:struct.pack_into('<2I',expected,0,0 if mutation==1 else BASE+0x2000,0x12345678)
   if block:struct.pack_into('<3I',expected,block-BASE,1,0x403600,struct.unpack_from('<I',expected,0)[0])
   struct.pack_into('<I',expected,4,block)
  else:struct.pack_into('<I',expected,4,0)
  events=[];uc.mem_write(BASE,bytes(initial));word(stack,stop);word(stack+4,value);uc.reg_write(UC_X86_REG_ESP,stack);uc.reg_write(UC_X86_REG_ECX,BASE);uc.emu_start(0x403d20,stop,count=1000)
  assert uc.reg_read(UC_X86_REG_EIP)==stop and uc.reg_read(UC_X86_REG_ESP)==stack+8 and uc.reg_read(UC_X86_REG_EAX)==BASE
  assert events==predicted and bytes(uc.mem_read(BASE,WIDTH))==expected,args
  rows.append(dict(args=(*args,len(events)),initial=bytes(initial),after=bytes(expected),events=events[:]))
 return rows
if __name__=='__main__':print('PROGRESS_COUNTED_RETAIL_OK cases='+str(len(collect())))
