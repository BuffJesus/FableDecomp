"""Compare list ownership operations including every native free callback."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
OBJECT=0x2000000;HEAP=OBJECT+0x1000;STACK=0x3000000;STOP=0x2200000
CLEAR=0xa61a80;FREE=0xbfea14

def execute(codes,entry,first_count,second_count,mutate):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in [(0xa61000,4096),(0xbfe000,4096),(OBJECT,0x5000),(STACK,0x10000),(STOP,4096)]:u.mem_map(a,n)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return int.from_bytes(u.mem_read(a,4),'little')
 for a,b in codes.items():u.mem_write(a,bytes(b))
 u.mem_write(OBJECT,b'\xa5'*20);live=set();serial=0
 def make(count):
  nonlocal serial
  head=HEAP+serial*16;serial+=1;live.add(head);put(head,head);put(head+4,head)
  for i in range(count):
   node=HEAP+serial*16;serial+=1;live.add(node);u.mem_write(node,b'\x5a'*16);previous=get(head+4);put(node,head);put(node+4,previous);put(previous,node);put(head+4,node)
  return head
 put(OBJECT,make(first_count));put(OBJECT+4,make(second_count));events=[]
 def hook(u,a,size,data):
  if a not in [CLEAR,FREE]:return
  sp=u.reg_read(UC_X86_REG_ESP)
  if a==CLEAR:
   assert u.reg_read(UC_X86_REG_ECX)==OBJECT;events.append(('clear_queries',))
   if mutate:
    x=get(OBJECT);put(OBJECT,get(OBJECT+4));put(OBJECT+4,x)
  else:
   node=get(sp+4);assert node in live;live.remove(node);events.append(('free',node,bytes(u.mem_read(node,16)).hex()));u.mem_write(node,b'\xdd'*16)
  u.reg_write(UC_X86_REG_EAX,0);u.reg_write(UC_X86_REG_ESP,sp+4);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,OBJECT)
 u.emu_start(entry,STOP,count=10000);assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+4
 return bytes(u.mem_read(OBJECT,20)),events,[(a,bytes(u.mem_read(a,16))) for a in sorted(live)]
