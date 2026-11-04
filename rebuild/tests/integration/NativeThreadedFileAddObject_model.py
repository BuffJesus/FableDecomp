"""Independent expected full-memory model and immutable native wrapper runner."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ROOT=0x98f350;CHILD=0x98f0f0;MANAGER=0x13bc870;G=MANAGER&~4095
A=0x2000000;N=32768;S=0x3000000;STOP=0x4000000
BODY=bytes.fromhex('8b4424045150b970c83b01e890fdffffc20400')
ROOT_SYMBOL='?AddObject@CThreadedFile@@QAEKABVCThreadedFileObjectDesc@@@Z'
CHILD_SYMBOL='?AddObject@CThreadedFileManager@@QAEKABVCThreadedFileObjectDesc@@AAVCThreadedFile@@@Z'
def word(n):return struct.pack('<I',n&0xffffffff)
def mutate(arena,glob,c):
 if c['mutation'] in (1,3):arena[c['file']-A+8:c['file']-A+12]=word(c['result']^0x5a5a5a5a)
 if c['mutation'] in (2,3):arena[c['desc']-A:c['desc']-A+16]=bytes([c['result']&255])*16
 if c['mutation']==3:glob[MANAGER-G:MANAGER-G+16]=word(c['result'])*4
def expected(arena,glob,c):
 a=bytearray(arena);g=bytearray(glob);snapshot=lambda:bytes(a)+bytes(g)
 events=[('AddObject',MANAGER,c['desc'],c['file'])];snaps=[snapshot(),snapshot()]
 mutate(a,g,c);snaps.append(snapshot());return events,snaps,c['result']
class Runner:
 def __init__(self,body=BODY):
  u=self.u=Uc(UC_ARCH_X86,UC_MODE_32)
  for p in {ROOT&~4095,CHILD&~4095,G}:u.mem_map(p,4096)
  u.mem_map(A,N);u.mem_map(S,4096);u.mem_map(STOP,4096);u.mem_write(ROOT,body)
  self.cpu=u.context_save();self.active=None;self.hits={ROOT:0,CHILD:0}
  u.hook_add(UC_HOOK_CODE,lambda uc,pc,size,data:self.active(uc,pc))
 def run(self,arena,glob,c):
  u=self.u;u.context_restore(self.cpu);u.mem_write(A,arena);u.mem_write(G,glob);u.mem_write(S,bytes(4096))
  sp=S+0x800;u.mem_write(sp,word(STOP)+word(c['desc']));u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,c['file'])
  regs=(UC_X86_REG_EBX,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI)
  for i,r in enumerate(regs):u.reg_write(r,0x12340000+i)
  snap=lambda:bytes(u.mem_read(A,N))+bytes(u.mem_read(G,4096));events=[];snaps=[snap()]
  def hook(uc,pc):
   if pc in self.hits:self.hits[pc]+=1
   if pc==STOP:uc.emu_stop();return
   if pc!=CHILD:return
   stack=uc.reg_read(UC_X86_REG_ESP);ret,desc,file=struct.unpack('<III',uc.mem_read(stack,12))
   assert (desc,file)==(c['desc'],c['file']) and uc.reg_read(UC_X86_REG_ECX)==MANAGER and ret==ROOT+16
   events.append(('AddObject',MANAGER,desc,file));snaps.append(snap())
   a=bytearray(uc.mem_read(A,N));g=bytearray(uc.mem_read(G,4096));mutate(a,g,c);uc.mem_write(A,bytes(a));uc.mem_write(G,bytes(g))
   uc.reg_write(UC_X86_REG_EAX,c['result']);uc.reg_write(UC_X86_REG_ECX,0xabcdef01);uc.reg_write(UC_X86_REG_EDX,0xabcdef02)
   uc.reg_write(UC_X86_REG_ESP,stack+12);uc.reg_write(UC_X86_REG_EIP,ret) # child thiscall RET8
  self.active=hook;u.emu_start(ROOT,STOP,count=100)
  assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+8
  assert [u.reg_read(r) for r in regs]==[0x12340000+i for i in range(4)]
  snaps.append(snap());return events,snaps,u.reg_read(UC_X86_REG_EAX)
