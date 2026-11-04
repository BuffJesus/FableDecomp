"""Execute original/rebuilt bank priority member and actual file wrapper."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *

ROOT,FILE,MANAGER,GLOBAL=0x9d5630,0x98f0e0,0x98eff0,0x13bc870

def execute(bodies,seed,uid,priority,ordinal):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 ARENA,STACK,STOP=0x2000000,0x3000000,0x4000000
 for address,size in ((0x9d5000,0x1000),(0x98e000,0x2000),(0x13bc000,0x1000),(ARENA,0x4000),(STACK,0x10000),(STOP,0x1000)):
  u.mem_map(address,size);u.mem_write(address,b'\xcc'*size)
 for address,body in bodies.items():u.mem_write(address,body)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 obj=ARENA+0x100+ordinal*0x80;bank=ARENA+0x1000+ordinal*0x200;file=ARENA+0x2800+ordinal*0x100
 u.mem_write(ARENA,bytes((i*37+seed*19)&255 for i in range(0x4000)))
 u.mem_write(0x13bc000,bytes((i*11+seed*23)&255 for i in range(0x1000)))
 put(obj+0x18,uid);put(obj+0x28,bank if uid else 0xdead0000);put(bank+0x110,file)
 before=bytes(u.mem_read(ARENA,0x4000));gbefore=bytes(u.mem_read(0x13bc000,0x1000));expected=bytearray(gbefore)
 trace=[];sp=STACK+0x8000;put(sp,STOP);put(sp+4,priority)
 def hook(uc,a,size,_):
  if a==FILE:
   esp=uc.reg_read(UC_X86_REG_ESP)
   assert uid and uc.reg_read(UC_X86_REG_ECX)==file
   assert get(esp+4)==uid and get(esp+8)==priority
   trace.append(('file',file,uid,priority));return
  if a!=MANAGER:return
  esp=uc.reg_read(UC_X86_REG_ESP)
  assert uid and uc.reg_read(UC_X86_REG_ECX)==GLOBAL and get(esp+4)==uid and get(esp+8)==priority
  assert bytes(uc.mem_read(ARENA,0x4000))==before
  trace.append(('manager',GLOBAL,uid,priority))
  value=(uid^priority^0xa5a55a5a)&0xffffffff
  put(GLOBAL+0x100,value);struct.pack_into('<I',expected,GLOBAL-0x13bc000+0x100,value)
  uc.reg_write(UC_X86_REG_EAX,0x76543210);uc.reg_write(UC_X86_REG_ECX,0xdeadbeef);uc.reg_write(UC_X86_REG_EDX,0x87654321)
  uc.reg_write(UC_X86_REG_EIP,get(esp));uc.reg_write(UC_X86_REG_ESP,esp+12)
 u.hook_add(UC_HOOK_CODE,hook)
 saved=(UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP)
 for i,r in enumerate(saved):u.reg_write(r,0x55660000+seed+i)
 u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,obj)
 u.emu_start(ROOT,STOP,count=128)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+8
 assert all(u.reg_read(r)==0x55660000+seed+i for i,r in enumerate(saved))
 assert len(trace)==(2 if uid else 0)
 state=bytes(u.mem_read(ARENA,0x4000));gs=bytes(u.mem_read(0x13bc000,0x1000))
 assert state==before and gs==bytes(expected)
 return state,gs,trace
