"""Compare native character search at the CRT strchr boundary."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
OBJECT=0x2000000;STORAGE=OBJECT+16;TEXT=OBJECT+64;STOP=0x2200000;STACK=0x3000000
TEXTS=[None,b'',b'a',b'aba',b'[name]',b'\x80\xff\x80',b'abc\0hidden',b'\x01\x7f\xfe\xff',b'01234567890123456789']
def execute(code,text,ch):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in [(0x99e000,0x1000),(0xbfe000,0x1000),(0x129a000,0x1000),(OBJECT,0x1000),(STOP,0x1000),(STACK,0x10000)]:u.mem_map(a,n)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return int.from_bytes(u.mem_read(a,4),'little')
 u.mem_write(0x99e5a0,bytes(code));put(OBJECT,STORAGE if text is not None else 0);put(STORAGE,TEXT);u.mem_write(TEXT,(text or b'')+b'\0');u.mem_write(0x129aaf4,b'\0');calls=[]
 def hook(u,a,n,data):
  if a!=0xbfeda0:return
  sp=u.reg_read(UC_X86_REG_ESP);p=get(sp+4);c=get(sp+8);assert c==(ch if ch<128 else ch|0xffffff00);assert p==(TEXT if text is not None else 0x129aaf4);calls.append(c)
  result=0
  while True:
   v=u.mem_read(p,1)[0]
   if v==(c&255):result=p;break
   if v==0:break
   p+=1
  u.reg_write(UC_X86_REG_EAX,result);u.reg_write(UC_X86_REG_ESP,sp+4);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP);put(sp+4,ch);u.reg_write(UC_X86_REG_ECX,OBJECT);u.reg_write(UC_X86_REG_ESP,sp)
 regs=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(regs):u.reg_write(r,0x123400+i)
 before=bytes(u.mem_read(OBJECT,512));u.emu_start(0x99e5a0,STOP,count=100)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+8 and len(calls)==1 and before==bytes(u.mem_read(OBJECT,512))
 assert all(u.reg_read(r)==0x123400+i for i,r in enumerate(regs))
 result=u.reg_read(UC_X86_REG_EAX);expected=((text or b'').split(b'\0')[0]+b'\0').find(bytes([ch]));assert result==expected&0xffffffff
 return result,calls
def compare(candidate,native):
 cases=0
 for text in TEXTS:
  for ch in range(256):assert execute(candidate,text,ch)==execute(native,text,ch);cases+=1
 return cases
