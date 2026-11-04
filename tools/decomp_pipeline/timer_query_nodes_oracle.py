"""Execute timer list allocation/insertion against the retail x86 bodies."""
import struct
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import *
CREATE=0x9f7ad0
FRONT=0x9f7e40
BACK=0x9f7ec0
MALLOC=0xbfea0e
PLACEMENT=0x405c10
ITERATOR=0x41c780
OBJECT=0x2000000
HEAD=OBJECT+0x100
NEW=OBJECT+0x2000
EXTRA=NEW+0x100
STACK=0x3000000
STOP=0x2200000

def execute(bodies,entry,count,pattern,alias,mutation):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in [(0x400000,0x20000),(0x9f7000,4096),(0xbfe000,4096),(OBJECT,0x4000),(STACK,0x10000),(STOP,4096)]:u.mem_map(a,n)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 for a,b in bodies.items():u.mem_write(a,bytes(b))
 u.mem_write(OBJECT,b'\xa7'*0x4000);put(OBJECT,HEAD)
 nodes=[HEAD]+[OBJECT+0x200+i*24 for i in range(count)]
 for i,a in enumerate(nodes):
  put(a,nodes[(i+1)%len(nodes)]);put(a+4,nodes[(i-1)%len(nodes)])
  u.mem_write(a+8,bytes((pattern+j*17+i*7)&255 for j in range(16)))
 source=OBJECT+0x1000 if not count or not alias else nodes[1 if alias==1 else -1]+8
 if source==OBJECT+0x1000:u.mem_write(source,bytes((pattern+j*17)&255 for j in range(16)))
 expected=bytes(u.mem_read(source,16));events=[];returned=False
 def hook(u,a,size,data):
  nonlocal returned
  if a==STOP:returned=True;u.emu_stop();return
  if a not in [MALLOC,PLACEMENT,ITERATOR]:return
  sp=u.reg_read(UC_X86_REG_ESP);cleanup=0;result=0
  if a==MALLOC:
   assert get(sp+4)==24;events.append(('malloc',24));result=NEW
   if mutation and entry!=CREATE:
    # Insert before the position captured by push_front/push_back. The
    # recovered body must reload position->previous after allocation.
    following=get(HEAD) if entry==FRONT else HEAD;previous=get(following+4)
    put(EXTRA,following);put(EXTRA+4,previous);put(previous,EXTRA);put(following+4,EXTRA)
  elif a==PLACEMENT:
   assert get(sp+4)==16 and get(sp+8)==NEW+8;result=NEW+8;events.append(('placement',16,NEW+8))
  else:
   obj=u.reg_read(UC_X86_REG_ECX);put(obj,get(sp+4));result=obj;cleanup=4
  u.reg_write(UC_X86_REG_EAX,result);u.reg_write(UC_X86_REG_ECX,0xdead0001);u.reg_write(UC_X86_REG_EDX,0xdead0002)
  u.reg_write(UC_X86_REG_EIP,get(sp));u.reg_write(UC_X86_REG_ESP,sp+4+cleanup)
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP);put(sp+4,source)
 u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,OBJECT)
 saved=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(saved):u.reg_write(r,0x12340000+i)
 u.emu_start(entry,STOP+1,count=10000)
 assert returned and u.reg_read(UC_X86_REG_ESP)==sp+8
 assert [u.reg_read(r) for r in saved]==[0x12340000+i for i in range(4)]
 assert bytes(u.mem_read(NEW+8,16))==expected
 if entry==CREATE:assert u.reg_read(UC_X86_REG_EAX)==NEW
 return events,bytes(u.mem_read(OBJECT,0x4000))
