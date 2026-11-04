import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ENTRY=0x9f7830
FREE=0xbfea14
OBJECT=0x2000000
HEAD=OBJECT+0x100
STACK=0x3000000
STOP=0x2200000
def execute(code,count,pattern):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in [(0x9f7000,4096),(0xbfe000,4096),(OBJECT,0x4000),(STACK,0x10000),(STOP,4096)]:u.mem_map(a,n)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 u.mem_write(ENTRY,bytes(code));u.mem_write(OBJECT,bytes([0x39+pattern])*0x4000);put(OBJECT,HEAD)
 nodes=[HEAD]+[OBJECT+0x200+i*24 for i in range(count)]
 for i,a in enumerate(nodes):put(a,nodes[(i+1)%len(nodes)]);put(a+4,nodes[(i-1)%len(nodes)])
 events=[];returned=False
 def hook(u,a,size,data):
  nonlocal returned
  if a==STOP:returned=True;u.emu_stop();return
  if a!=FREE:return
  sp=u.reg_read(UC_X86_REG_ESP);node=get(sp+4);assert node==nodes[len(events)+1];events.append(node);u.mem_write(node,b'\xdd'*24)
  u.reg_write(UC_X86_REG_EAX,0xdead0000);u.reg_write(UC_X86_REG_ECX,0xdead0001);u.reg_write(UC_X86_REG_EDX,0xdead0002);u.reg_write(UC_X86_REG_EIP,get(sp));u.reg_write(UC_X86_REG_ESP,sp+4)
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,OBJECT)
 u.emu_start(ENTRY,STOP+1,count=10000)
 assert returned and u.reg_read(UC_X86_REG_ESP)==sp+4 and len(events)==count and get(HEAD)==HEAD and get(HEAD+4)==HEAD
 return events,bytes(u.mem_read(OBJECT,0x4000))
