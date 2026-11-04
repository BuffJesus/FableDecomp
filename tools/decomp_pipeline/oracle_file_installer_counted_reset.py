"""Independent retail installer reset: old ownership, callbacks and new-block failure."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_EAX
from pe_oracle import EXE,pe_sections
BASE=0x2004000;WIDTH=0x8000;OLD=BASE+0x3000;NEW=BASE+0x4000;OTHER=BASE+0x5000

def collect():
 raw=EXE.read_bytes();uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1200000)
 for _,va,vs,offset,size in pe_sections(raw):
  if size:uc.mem_write(0x400000+va,raw[offset:offset+size])
 uc.mem_map(0x2000000,0x20000);rows=[];events=[];args=();stack=0x2010000;stop=0x200f000
 def word(a,v):uc.mem_write(a,struct.pack('<I',v&0xffffffff))
 def read(a):return struct.unpack('<I',uc.mem_read(a,4))[0]
 def hook(uc,a,n,user):
  if a not in [0xbfea1a,0xbfe9bc,0x200d200]:return
  sp=uc.reg_read(UC_X86_REG_ESP);value,reference,count,block,mutation=args
  kind=1 if a==0x200d200 else 2 if a==0xbfe9bc else 3;parameter=uc.reg_read(UC_X86_REG_ECX)if kind==1 else read(sp+4);events.append((kind,parameter,bytes(uc.mem_read(BASE,WIDTH))))
  if kind==1 and mutation:word(BASE+4,OTHER);word(BASE,BASE+0x2000)
  elif kind==2 and mutation==2:word(BASE+4,OLD);word(BASE,0)
  elif kind==3:
   assert parameter==12
   if mutation:word(BASE,0 if mutation==1 else BASE+0x2000);word(BASE+4,0x12345678)
   uc.reg_write(UC_X86_REG_EAX,block)
  uc.reg_write(UC_X86_REG_EIP,read(sp));uc.reg_write(UC_X86_REG_ESP,sp+4)
 uc.hook_add(UC_HOOK_CODE,hook)
 for args in itertools.product([0,BASE+0x1000,BASE+0x2000],[0,OLD],[0,1,2,0xffffffff],[0,NEW],[0,1,2]):
  value,reference,count,block,mutation=args;initial=bytearray(b'\xa5'*WIDTH)
  def put(state,a,v):struct.pack_into('<I',state,a-BASE,v&0xffffffff)
  put(initial,BASE,BASE+0x1000);put(initial,BASE+4,reference)
  for ref in [OLD,OTHER]:struct.pack_into('<3I',initial,ref-BASE,count,0x200d200,BASE+0x1000)
  expected=bytearray(initial);predicted=[];active=reference
  def event(kind,p):predicted.append((kind,p,bytes(expected)))
  if active:
   put(expected,active,count-1)
   if ((count-1)&0xffffffff)==0:
    event(1,BASE+0x1000)
    if mutation:active=OTHER;put(expected,BASE+4,active);put(expected,BASE,BASE+0x2000)
    event(2,active)
    if mutation==2:put(expected,BASE+4,OLD);put(expected,BASE,0)
  put(expected,BASE+4,0);put(expected,BASE,value)
  if value:
   event(3,12)
   if mutation:put(expected,BASE,0 if mutation==1 else BASE+0x2000);put(expected,BASE+4,0x12345678)
   if block:struct.pack_into('<3I',expected,block-BASE,1,0x403630,struct.unpack_from('<I',expected,0)[0])
   put(expected,BASE+4,block)
  events=[];uc.mem_write(BASE,bytes(initial));word(stack,stop);word(stack+4,value);uc.reg_write(UC_X86_REG_ESP,stack);uc.reg_write(UC_X86_REG_ECX,BASE);uc.emu_start(0x403db0,stop,count=1000)
  assert uc.reg_read(UC_X86_REG_EIP)==stop and uc.reg_read(UC_X86_REG_ESP)==stack+8
  assert events==predicted and bytes(uc.mem_read(BASE,WIDTH))==expected,args
  rows.append(dict(args=(*args,len(events)),initial=bytes(initial),after=bytes(expected),events=events[:]))
 return rows
if __name__=='__main__':print('FILE_INSTALLER_RESET_RETAIL_OK cases='+str(len(collect())))
