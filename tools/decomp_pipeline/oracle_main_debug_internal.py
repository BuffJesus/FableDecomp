"""Independent retail five-argument InternalAllocate trailer/cookie callback model."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_EAX
from pe_oracle import EXE,pe_sections
BASE=0x2004000;WIDTH=0x8000;POINTER=BASE+0x6000

def collect():
 raw=EXE.read_bytes();uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1200000)
 for _,va,vs,offset,size in pe_sections(raw):
  if size:uc.mem_write(0x400000+va,raw[offset:offset+size])
 uc.mem_map(0x2000000,0x20000);rows=[];events=[];queries=0;args=();stack=0x2010000;stop=0x200f000
 def word(a,v):uc.mem_write(a,struct.pack('<I',v&0xffffffff))
 def read(a):return struct.unpack('<I',uc.mem_read(a,4))[0]
 def snapshot():return bytes(uc.mem_read(BASE,WIDTH))
 def hook(uc,a,n,user):
  nonlocal queries
  if a not in [0x9f2a40,0x9f2390,0xbfe9f9]:return
  sp=uc.reg_read(UC_X86_REG_ESP);owner=uc.reg_read(UC_X86_REG_ECX);size,category,tracking,capacity,mutation,cookie=args;cleanup=4
  if a==0x9f2a40:
   assert owner==BASE and read(sp+4)==size and read(sp+8)==category and read(sp+12)==1
   events.append((1,owner,size,category,snapshot()));cleanup=16;uc.reg_write(UC_X86_REG_EAX,POINTER)
   if mutation:uc.mem_write(BASE+0x414,b'\x07');word(0x139c8a8,cookie^0x12345678)
  elif a==0x9f2390:
   assert owner==BASE and read(sp+4)==POINTER
   events.append((2,owner,POINTER,queries,snapshot()));cleanup=8
   uc.reg_write(UC_X86_REG_EAX,0xffffffff if queries==0 else capacity)
   if mutation:uc.mem_write(BASE+0x414,bytes([255 if queries==0 else 3]))
   queries+=1
  else:events.append((3,owner,0,0,snapshot()));assert owner==cookie
  uc.reg_write(UC_X86_REG_EIP,read(sp));uc.reg_write(UC_X86_REG_ESP,sp+cleanup)
 uc.hook_add(UC_HOOK_CODE,hook)
 for args in itertools.product([0,1,236,0x7fffffff,0x80000000,0xffffffff],[0,9],[0,255],[20,24,64,256],[0,1],[0,0xbb40e64e]):
  size,category,tracking,capacity,mutation,cookie=args;initial=bytearray(b'\xa5'*WIDTH);initial[0x414]=tracking;expected=bytearray(initial);predicted=[]
  def event(kind,a,b,c):predicted.append((kind,a,b,c,bytes(expected)))
  event(1,BASE,size,category)
  if mutation:expected[0x414]=7
  event(2,BASE,POINTER,0)
  if mutation:expected[0x414]=255
  event(2,BASE,POINTER,1)
  if mutation:expected[0x414]=3
  struct.pack_into('<3I',expected,POINTER-BASE+capacity-20,0xffffffff,expected[0x414],size)
  event(3,cookie,0,0)
  events=[];queries=0;uc.mem_write(BASE,bytes(initial));word(0x139c8a8,cookie);word(stack,stop)
  for i,v in enumerate([size,category,0xdeadbeef,0xffffffff,0xbaadf00d]):word(stack+4+i*4,v)
  uc.reg_write(UC_X86_REG_ESP,stack);uc.reg_write(UC_X86_REG_ECX,BASE);uc.emu_start(0x9f2ac0,stop,count=1000)
  assert uc.reg_read(UC_X86_REG_EIP)==stop and uc.reg_read(UC_X86_REG_ESP)==stack+24 and uc.reg_read(UC_X86_REG_EAX)==POINTER
  assert events==predicted and snapshot()==expected,args
  rows.append(dict(args=args,initial=bytes(initial),after=bytes(expected),events=events[:]))
 return rows
if __name__=='__main__':print('MAIN_DEBUG_INTERNAL_RETAIL_OK cases='+str(len(collect())))
