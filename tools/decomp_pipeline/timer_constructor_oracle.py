"""Timer constructor comparison with native allocation boundaries."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ENTRY=0x9f70e0
MALLOC=0xbfea0e
OBJECT=0x2000000
STACK=0x3000000
STOP=0x2200000

def execute(code,pattern,mutation):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in [(0x9f7000,4096),(0xbfe000,4096),(0x122e000,4096),(0x13cb000,4096),(OBJECT,0x3000),(STACK,0x10000),(STOP,4096)]:u.mem_map(a,n)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 u.mem_write(ENTRY,bytes(code));u.mem_write(OBJECT,bytes([pattern])*0x3000);u.mem_write(0x13cb4b0,bytes([pattern])*72)
 events=[];returned=False
 def hook(u,a,size,data):
  nonlocal returned
  if a==STOP:returned=True;u.emu_stop();return
  if a!=MALLOC:return
  sp=u.reg_read(UC_X86_REG_ESP);assert get(sp+4)==24
  events.append(bytes(u.mem_read(OBJECT,20)));index=len(events)-1;assert index<2
  node=OBJECT+0x1000+index*0x100
  if mutation&(1<<index):put(OBJECT,0xbeef0000+index)
  u.reg_write(UC_X86_REG_EAX,node);u.reg_write(UC_X86_REG_ECX,0xdead0001);u.reg_write(UC_X86_REG_EDX,0xdead0002)
  u.reg_write(UC_X86_REG_EIP,get(sp));u.reg_write(UC_X86_REG_ESP,sp+4)
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,OBJECT)
 u.emu_start(ENTRY,STOP+1,count=1000)
 assert returned and len(events)==2 and u.reg_read(UC_X86_REG_ESP)==sp+4 and u.reg_read(UC_X86_REG_EAX)==OBJECT
 assert get(OBJECT+12)==1 and get(OBJECT+16)==0
 assert bytes(u.mem_read(0x13cb4b0,72))==b'\0'*72
 return events,bytes(u.mem_read(OBJECT,0x3000))
