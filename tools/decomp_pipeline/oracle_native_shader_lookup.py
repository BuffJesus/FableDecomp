"""Retail typed shader lookup, with actual bank indexing and callback mutations."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from pe_oracle import EXE,pe_sections
BASE=0x24000000;WIDTH=0x1000

def collect(compose=False):
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000);u.mem_map(BASE,0x10000)
 raw=EXE.read_bytes()
 for _,va,_,off,n in pe_sections(raw):
  if n:u.mem_write(0x400000+va,raw[off:off+n])
 inputs=bytearray();expected=bytearray();cases=snapshots=0
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def event(kind,a=0,b=0):
  nonlocal snapshots
  state=bytearray(u.mem_read(BASE,WIDTH))
  for offset in [0xa00,0xa20,0xa40]:state[offset:offset+4]=b'\0'*4
  expected.extend(struct.pack('<III',kind,a,b));expected.extend(state);snapshots+=1
 def hook(u,a,n,_):
  if compose and a==0x9cc1d0:return
  if a not in [0x9ccdc0,0xbfea1a,0x99ec30,0x9cc1d0,0xa63020,0x99eae0]:return
  sp=u.reg_read(UC_X86_REG_ESP);this=u.reg_read(UC_X86_REG_ECX);cleanup=0;result=0
  if a==0x9ccdc0:
   assert this==BASE+0x200 and get(sp+4)==BASE+0x700
   event(1,this,get(BASE+0x700));cleanup=4;result=index
   if mode==1:put(BASE+4,BASE+0x400);put(BASE+8,BASE+0x100)
  elif a==0xbfea1a:
   assert get(sp+4)==8;event(2,8);result=BASE+0xa20
   if mode:put(BASE+0x34,0x12345678);put(BASE+4,BASE+0x400)
  elif a==0x99ec30:
   source=get(sp+4);assert source in [BASE+0x620,BASE+0x660] or (compose and (BASE+0xd00<=source<BASE+0xd20 or BASE+0xd80<=source<BASE+0xda0))
   event(3,source,get(source));put(this,get(source));cleanup=4;result=this
   if mode==1:put(BASE+4,BASE+0x200)
   if compose and source>=BASE+0xd00 and mode==2:put(BASE+0x34,0x87654321)
  elif a==0x9cc1d0:
   out=get(sp+4);assert get(sp+8)==index and this in [BASE+0x200,BASE+0x400]
   event(4,this,index);put(out,0x222000+(this-BASE));result=out;cleanup=8
   if mode==2:put(BASE+0x34,0x87654321)
  elif a==0xa63020:
   code,decl,name,layout=[get(sp+4+i*4)for i in range(4)]
   assert this==BASE+0xa20 and code in [BASE+0x608,BASE+0x648]
   put(this,0x129b3d0);event(5,code,decl);event(6,get(name),get(layout))
   put(this+4,get(BASE+0xc00));result=this;cleanup=16
   if mode==2:put(BASE+8,BASE+0x100)
  else:
   event(7,get(this));put(this,0)
   if mode==2:put(BASE+0x34,get(BASE+0x34)^0x100)
  u.reg_write(UC_X86_REG_EAX,result);u.reg_write(UC_X86_REG_EIP,get(sp));u.reg_write(UC_X86_REG_ESP,sp+4+cleanup)
 u.hook_add(UC_HOOK_CODE,hook)
 for index,cached,data,refs,mode,byname in itertools.product([0,1,7],[0,1],[0,1],[0,1,2,0xffffffff],range(3),[0,1]):
  arena=bytearray((i*17+cases*13)&255 for i in range(WIDTH))
  def word(o,v):struct.pack_into('<I',arena,o,v&0xffffffff)
  word(4,BASE+0x200);word(8,BASE+0x80);word(12,BASE+0xa0);word(16,BASE+0xa0)
  for off in [0x80,0x100]:word(off+index*4,BASE+0xa00 if cached else 0)
  word(0x310,BASE+0x800);word(0x510,BASE+0x840)
  word(0x800+index*4,BASE+0x600);word(0x840+index*4,BASE+0x640)
  word(0xa00,0x129b3d0);word(0xa04,BASE+0xb00 if data else 0);word(0xb10,refs)
  word(0xc00,BASE+0xb00 if data else 0)
  if compose:
   word(0x208,BASE+0xd00);word(0x20c,BASE+0xd20);word(0x408,BASE+0xd80);word(0x40c,BASE+0xda0)
  inputs.extend(struct.pack('<III',index,mode,byname));inputs.extend(arena);u.mem_write(BASE,bytes(arena))
  sp=BASE+0xe000;stop=BASE+0xf000
  u.mem_write(sp,struct.pack('<III',stop,BASE+0xa40,BASE+0x700 if byname else index))
  u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,BASE)
  regs=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
  for i,r in enumerate(regs):u.reg_write(r,0x987600+i)
  u.emu_start(0xa5d5f0 if byname else 0xa5d510,stop,count=10000)
  assert u.reg_read(UC_X86_REG_EIP)==stop and u.reg_read(UC_X86_REG_ESP)==sp+12 and u.reg_read(UC_X86_REG_EAX)==BASE+0xa40
  assert all(u.reg_read(r)==0x987600+i for i,r in enumerate(regs))
  assert get(BASE+0xa40)==0x129b3d0 and get(BASE+0xa44)==(BASE+0xb00 if data else 0)
  assert get(BASE+0xb10)==((refs+data)&0xffffffff)
  event(8);cases+=1
 return inputs,expected,dict(cases=cases,snapshots=snapshots)

def allocation_failure(candidate):
 """Both executable bodies retain retail's null-allocation fault after cache store."""
 from unicorn import UcError,UC_HOOK_MEM_INVALID
 outcomes=[]
 for code in [None,candidate]:
  for index in [0,1,7]:
   u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000);u.mem_map(BASE,0x10000)
   raw=EXE.read_bytes()
   for _,va,_,off,n in pe_sections(raw):
    if n:u.mem_write(0x400000+va,raw[off:off+n])
   if code is not None:u.mem_write(0xa5d510,bytes(code))
   def put(a,v):u.mem_write(a,struct.pack('<I',v))
   def get(a):return int.from_bytes(u.mem_read(a,4),'little')
   put(BASE+4,BASE+0x200);put(BASE+8,BASE+0x80);put(BASE+0x310,BASE+0x800);put(BASE+0x800+index*4,BASE+0x600)
   trace=[]
   def hook(u,a,n,_):
    if a==0xbfea1a:
     sp=u.reg_read(UC_X86_REG_ESP);assert get(sp+4)==8
     trace.append(('allocation',8));u.reg_write(UC_X86_REG_EAX,0);u.reg_write(UC_X86_REG_ESP,sp+4);u.reg_write(UC_X86_REG_EIP,get(sp))
    assert a not in [0x99ec30,0x9cc1d0,0xa63020,0x99eae0],'no temporary may exist on allocation failure'
   def fault(u,access,a,size,value,_):trace.append(('fault',a,size));return False
   u.hook_add(UC_HOOK_CODE,hook);u.hook_add(UC_HOOK_MEM_INVALID,fault)
   sp=BASE+0xe000;u.mem_write(sp,struct.pack('<III',BASE+0xf000,BASE+0xa40,index));u.reg_write(UC_X86_REG_ECX,BASE);u.reg_write(UC_X86_REG_ESP,sp)
   try:u.emu_start(0xa5d510,BASE+0xf000,count=1000)
   except UcError:pass
   else:raise AssertionError('retail allocation failure must fault')
   assert trace==[('allocation',8),('fault',4,4)] and get(BASE+0x80+index*4)==0
   outcomes.append((trace,bytes(u.mem_read(BASE,WIDTH))))
 assert outcomes[:3]==outcomes[3:]
 return 3
