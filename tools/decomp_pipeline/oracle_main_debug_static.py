"""Retail static debug Allocate: independent lazy-create/lock/argument model."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_EAX
from pe_oracle import EXE,pe_sections
BASE=0x2004000;WIDTH=0x8000;OWNER_B=BASE+0x1000

def collect():
 raw=EXE.read_bytes();uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1200000)
 for _,va,vs,offset,size in pe_sections(raw):
  if size:uc.mem_write(0x400000+va,raw[offset:offset+size])
 uc.mem_map(0x2000000,0x20000);rows=[];events=[];depth=0;args=();stack=0x2010000;stop=0x200f000
 def word(a,v):uc.mem_write(a,struct.pack('<I',v&0xffffffff))
 def read(a):return struct.unpack('<I',uc.mem_read(a,4))[0]
 def snapshot():
  s=bytearray(uc.mem_read(BASE,WIDTH));s[0x3000:0x3004]=uc.mem_read(0x13caa90,4);struct.pack_into('<I',s,0x3004,depth);return bytes(s)
 def hook(uc,a,n,user):
  nonlocal depth
  if a not in [0x9f2870,0x200d000,0x200d100,0x9f2ac0]:return
  sp=uc.reg_read(UC_X86_REG_ESP);size,category,singleton,mutation,result=args;cleanup=4
  if a==0x9f2870:events.append((4,0,0,0,snapshot()));word(0x13caa90,BASE)
  elif a==0x200d000:
   events.append((1,0,0,0,snapshot()));assert read(sp+4)==0x13caa70;cleanup=8;depth+=1
   if mutation==1:word(0x13caa90,OWNER_B)
  elif a==0x200d100:
   events.append((2,0,0,0,snapshot()));assert read(sp+4)==0x13caa70;cleanup=8;depth-=1
   if mutation==2:word(0x13caa90,0)
  else:
   owner=uc.reg_read(UC_X86_REG_ECX);assert [read(sp+4+i*4)for i in range(5)]==[size,category,0xdeadbeef,0xffffffff,0xbaadf00d]
   events.append((5,owner,size,category,snapshot()));cleanup=24;uc.reg_write(UC_X86_REG_EAX,result)
   if mutation==2:word(0x13caa90,OWNER_B)
  uc.reg_write(UC_X86_REG_EIP,read(sp));uc.reg_write(UC_X86_REG_ESP,sp+cleanup)
 uc.hook_add(UC_HOOK_CODE,hook);word(0x143fe0c,0x200d000);word(0x143fe10,0x200d100)
 for args in itertools.product([0,1,236,0x7fffffff,0x80000000,0xffffffff],[0,9],[0,BASE],[0,1,2],[0,BASE+0x6000]):
  size,category,singleton,mutation,result=args;initial=bytearray(b'\xa5'*WIDTH);struct.pack_into('<2I',initial,0x3000,singleton,0);expected=bytearray(initial);predicted=[];active=singleton;held=0
  def event(kind,a=0,b=0,c=0):struct.pack_into('<2I',expected,0x3000,active,held);predicted.append((kind,a,b,c,bytes(expected)))
  if not active:event(4);active=BASE
  event(1);held+=1
  if mutation==1:active=OWNER_B
  event(5,active,size,category)
  if mutation==2:active=OWNER_B
  event(2);held-=1
  if mutation==2:active=0
  struct.pack_into('<2I',expected,0x3000,active,held);events=[];depth=0;uc.mem_write(BASE,bytes(initial));word(0x13caa90,singleton);word(stack,stop)
  for i,v in enumerate([0xdeadbeef,0xffffffff,0xbaadf00d]):word(stack+4+i*4,v)
  uc.reg_write(UC_X86_REG_ESP,stack);uc.reg_write(UC_X86_REG_ECX,size);uc.reg_write(UC_X86_REG_EDX,category);uc.emu_start(0x9f2b90,stop,count=1000)
  assert uc.reg_read(UC_X86_REG_EIP)==stop and uc.reg_read(UC_X86_REG_ESP)==stack+16 and uc.reg_read(UC_X86_REG_EAX)==result
  assert depth==0 and events==predicted and snapshot()==expected,args
  rows.append(dict(args=(*args,len(events)),initial=bytes(initial),after=bytes(expected),events=events[:]))
 return rows
if __name__=='__main__':print('MAIN_DEBUG_STATIC_RETAIL_OK cases='+str(len(collect())))
