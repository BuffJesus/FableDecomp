"""CRT layout initialization, virtual names and registered destruction in retail."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_EAX,UC_X86_REG_ESP,UC_X86_REG_EIP
from pe_oracle import EXE,pe_sections
BASE=0x24000000;GLOBAL=0x13ca8a8
def collect():
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000);u.mem_map(BASE,0x10000);raw=EXE.read_bytes()
 for _,va,_,off,size in pe_sections(raw):
  if size:u.mem_write(0x400000+va,raw[off:off+size])
 expected=bytearray();registered=[];snapshots=0
 def word(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def read(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def event(kind,argument=0,text=b''):
  nonlocal snapshots
  expected.extend(struct.pack('<2I',kind,argument));expected.extend(text+bytes(16-len(text)));expected.extend(u.mem_read(GLOBAL,332));snapshots+=1
 def hook(u,a,n,_):
  if a not in (0x4012bc,0x99ebf0,0xbfea14,0xbfe9bc):return
  sp=u.reg_read(UC_X86_REG_ESP);count=0
  if a==0x4012bc:registered.append(read(sp+4));u.reg_write(UC_X86_REG_EAX,0)
  elif a==0x99ebf0:
   pointer=read(sp+4);text=bytearray()
   while u.mem_read(pointer+len(text),1)!=b'\0':text.extend(u.mem_read(pointer+len(text),1))
   event(2,read(sp+8),bytes(text));receiver=u.reg_read(UC_X86_REG_ECX);word(receiver,BASE+(0x2010 if text else 0x2000));u.reg_write(UC_X86_REG_EAX,receiver);count=2
  else:event(4 if a==0xbfea14 else 5,read(sp+4))
  u.reg_write(UC_X86_REG_EIP,read(sp));u.reg_write(UC_X86_REG_ESP,sp+4+4*count)
 u.hook_add(UC_HOOK_CODE,hook)
 def call(address,receiver=0,args=()):
  stack=BASE+0xe000;stop=BASE+0xf000;u.mem_write(stack,struct.pack('<'+'I'*(len(args)+1),stop,*args));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,receiver)
  u.emu_start(address,stop,count=10000);assert u.reg_read(UC_X86_REG_ESP)==stack+4+4*len(args);return u.reg_read(UC_X86_REG_EAX)
 call(0x1224140);assert registered==[0x122a820];event(1)
 call(0x98d490,BASE+0x1000)
 for address,receiver in [(0x98d6f0,BASE+0x1000),(0x9e16b0,GLOBAL)]:
  assert call(address,receiver,(BASE+0x3000,))==BASE+0x3000;event(3,read(BASE+0x3000))
 for flag in range(2):
  receiver=BASE+0x1800+flag*0x400;call(0x98d490,receiver);word(receiver,0x129b9b0);event(6,call(0x9e1850,receiver,(flag,)))
 for offset,value in [(0x140,BASE+0x400),(0x144,BASE+0x420),(0x148,BASE+0x440)]:word(GLOBAL+offset,value)
 call(registered[0]);return expected,dict(snapshots=snapshots,registered_destructor='0122a820')
