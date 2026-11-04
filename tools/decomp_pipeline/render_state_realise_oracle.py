"""Comparative render-state resend with mutating D3D boundaries."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ENTRY=0xa058c0
OBJECT=0x2000000
STATES=OBJECT+0x4000
DEVICE=OBJECT+0x6000
TABLE=OBJECT+0x6100
STACK=0x3000000
CALLBACK=0x2200000
STOP=CALLBACK+0x100

def execute(code,count,kind,mutation,failure,duplicate,equal):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in [(0xa05000,4096),(OBJECT,0x8000),(STACK,0x10000),(CALLBACK,4096)]:u.mem_map(a,n)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 u.mem_write(ENTRY,bytes(code));u.mem_write(OBJECT,b'\xa5'*0x3a3c);u.mem_write(STATES,b'\xb7'*(256*20));put(OBJECT+4,DEVICE);put(OBJECT+0x280c,count)
 for i in range(2):put(DEVICE+i*4,TABLE)
 for offset,k in [(0xe4,1),(0x10c,2),(0x114,3)]:put(TABLE+offset,CALLBACK+k*16)
 types=[0,1,2,3,4,255]
 for i in range(256):
  state=STATES+i*20;u.mem_write(state+17,bytes([types[(kind+i)%6],i]));put(state+4,i*7+13);put(state,i*7+13 if equal else 0xffffffff);put(state+12,i+19);put(OBJECT+0x2008+i*4,STATES+(0 if duplicate else i)*20)
 events=[];returned=False
 def hook(u,a,size,data):
  nonlocal returned
  if a==STOP:returned=True;u.emu_stop();return
  if a not in [CALLBACK+16,CALLBACK+32,CALLBACK+48]:return
  k=(a-CALLBACK)//16;sp=u.reg_read(UC_X86_REG_ESP);argc=3 if k==1 else 4;args=[get(sp+4+i*4) for i in range(argc)]
  device=args[0];param=args[-2];value=args[-1];index=param-19;state=STATES+index*20;assert 0<=index<256 and u.mem_read(state+16,1)==b'\0'
  call=len(events);events.append((k,args))
  if mutation&1:put(OBJECT+4,DEVICE+((call+1)%2)*4)
  if mutation&2 and call==0:put(OBJECT+0x280c,0)
  if mutation&4:put(state+4,get(state+4)^0x80);put(state,get(state)^0x40);u.mem_write(state+16,b'\1')
  u.reg_write(UC_X86_REG_EAX,0x80004005 if failure else 0);u.reg_write(UC_X86_REG_ECX,0xdead0001);u.reg_write(UC_X86_REG_EDX,0xdead0002)
  u.reg_write(UC_X86_REG_EIP,get(sp));u.reg_write(UC_X86_REG_ESP,sp+4+argc*4)
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,OBJECT)
 preserved=[UC_X86_REG_EBX,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI]
 for i,r in enumerate(preserved):u.reg_write(r,0xabcd0000+i)
 u.emu_start(ENTRY,STOP+1,count=20000)
 assert returned and u.reg_read(UC_X86_REG_ESP)==sp+4 and get(OBJECT+0x280c)==0
 for i,r in enumerate(preserved):assert u.reg_read(r)==0xabcd0000+i
 return events,bytes(u.mem_read(OBJECT,0x3a3c)),bytes(u.mem_read(STATES,256*20))
