"""Execute CMemoryDataOutputStream's SetPosition, GetPosition, Close, destructor and scalar deleting destructor against retail."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
OBJ=0x2000000;BUF=0x2001000;STACK=0x3000000;STOP=0x4000000
SETPOS,GETPOS,CLOSE,DTOR,SDTOR=0x994b10,0x9945b0,0x9945c0,0x996b30,0x9973c0
THROW,FREE,STRING_DTOR,BASE_DTOR,DELETE=0xbfeb84,0xbfea14,0x99eae0,0x99a300,0xbfe9bc
THROW_INFO,OWN_VT,STREAM_VT=0x1369380,0x129a788,0x129a69c
STUBS=(THROW,FREE,STRING_DTOR,BASE_DTOR,DELETE)
def run(codes,entry,args,setup):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,z in [(0x994000,0x4000),(0x99a000,0x5000),(0xbfe000,0x1000),(OBJ,0x2000),(STACK,0x10000),(STOP,0x1000)]:u.mem_map(a,z)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 for a,b in codes.items():u.mem_write(a,bytes(b))
 for a in STUBS:u.mem_write(a,b'\xc3')
 u.mem_write(OBJ,b'\x3c'*0x2000);setup(u,put)
 trace=[];state={'thrown':False}
 def hook(uc,a,s,user):
  if a not in STUBS:return
  sp=uc.reg_read(UC_X86_REG_ESP);ecx=uc.reg_read(UC_X86_REG_ECX);pop=0
  if a==THROW:
   obj,info=get(sp+4),get(sp+8);trace.append(('throw',get(obj),info));state['thrown']=True;uc.emu_stop();return
  if a==FREE:trace.append(('free',get(sp+4)))
  elif a==DELETE:trace.append(('delete',get(sp+4)))
  elif a==STRING_DTOR:trace.append(('string_dtor',ecx,get(OBJ)))
  else:trace.append(('base_dtor',ecx,get(ecx)))
  uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP)
 for i,v in enumerate(args):put(sp+4+4*i,v)
 regs=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(regs):u.reg_write(r,0x778800+i)
 u.reg_write(UC_X86_REG_ECX,OBJ);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_EAX,0xdeadbeef)
 u.emu_start(entry,STOP,count=10000)
 if not state['thrown']:
  assert u.reg_read(UC_X86_REG_ESP)==sp+4+4*len(args) and all(u.reg_read(r)==0x778800+i for i,r in enumerate(regs)),hex(entry)
 return u.reg_read(UC_X86_REG_EAX),trace,bytes(u.mem_read(OBJ,0x18)),state['thrown']
def compare(candidate,native):
 cases=0
 def both(entry,args,setup):
  c=run(candidate,entry,args,setup);assert c==run(native,entry,args,setup),hex(entry);return c
 for size,position in itertools.product((0,1,7,64),(0,1,6,7,8,63,64,65,0x7fffffff,0x80000000,0xffffffff)):
  def setup(u,put,size=size):put(OBJ+8,BUF);put(OBJ+0xc,BUF+size);put(OBJ+0x10,BUF+64);put(OBJ+0x14,0x1234)
  eax,trace,mem,thrown=both(SETPOS,[position],setup);current=struct.unpack_from('<I',mem,0x14)[0]
  if position>size:assert thrown and trace==[('throw',11,THROW_INFO)] and current==0x1234
  else:assert not thrown and not trace and current==position
  cases+=1
 for value in (0,1,0x1234,0xffffffff):
  def setup(u,put,value=value):put(OBJ+0x14,value)
  eax,trace,mem,_=both(GETPOS,[],setup);assert eax==value and not trace
  eax,trace,mem,_=both(CLOSE,[],setup);assert not trace and mem==b'\x3c'*0x14+struct.pack('<I',value)
  cases+=2
 for start,flag in itertools.product((0,BUF),(0,1,2,3)):
  def setup(u,put,start=start):put(OBJ,OWN_VT);put(OBJ+8,start);put(OBJ+0xc,start+4 if start else 0)
  want=([('free',start)] if start else [])+[('string_dtor',OBJ+4,OWN_VT),('base_dtor',OBJ,STREAM_VT)]
  eax,trace,mem,_=both(DTOR,[],setup);assert trace==want;cases+=1
  eax,trace,mem,_=both(SDTOR,[flag],setup);assert eax==OBJ and trace==want+([('delete',OBJ)] if flag&1 else []);cases+=1
 return cases
