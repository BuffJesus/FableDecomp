"""Retail byte binary traits with source-window overflow mocked."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_EDX,UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EIP
from pe_oracle import EXE,pe_sections
def collect():
 raw=EXE.read_bytes();sections=pe_sections(raw);rows=[]
 for count in range(4,33):
  for mode in [0]:
   u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000)
   for _,va,vs,off,size in sections:
    if size:u.mem_write(va+0x400000,raw[off:off+size])
   u.mem_map(0x2000000,0x20000);stream=0x2001000;data=0x2002000;output=0x2003000;stack=0x2010000;stop=0x200f000;calls=[]
   def read32(p):return struct.unpack('<I',u.mem_read(p,4))[0]
   def write32(p,v):u.mem_write(p,struct.pack('<I',v))
   def hook(uc,address,size,user):
    if address==stop:uc.emu_stop()
    elif address==0x9b7650:
     left=uc.reg_read(UC_X86_REG_ECX);right=uc.reg_read(UC_X86_REG_EDX);x=bytes(u.mem_read(left,24));y=bytes(u.mem_read(right,24));u.mem_write(left,y);u.mem_write(right,x);sp=uc.reg_read(UC_X86_REG_ESP);uc.reg_write(UC_X86_REG_ESP,sp+4);uc.reg_write(UC_X86_REG_EIP,read32(sp));calls.append((left,right))
    elif address in (0xbfea14,0x99eae0):
     sp=uc.reg_read(UC_X86_REG_ESP);uc.reg_write(UC_X86_REG_ESP,sp+4);uc.reg_write(UC_X86_REG_EIP,read32(sp))
   keys=[0]+[(i*17)%31+1 for i in range(1,count)];pivot=keys[count//2]
   for i,key in enumerate(keys):u.mem_write(stream+i*24,struct.pack('<IIIIII',key,0,key+100,0,0,0))
   u.mem_write(stack,struct.pack('<IIIIIIII',stop,pivot,0,pivot+100,0,0,0,0));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,stream);u.reg_write(UC_X86_REG_EDX,stream+count*24);u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0x9b8630,stop,count=100000)
   cut=(u.reg_read(UC_X86_REG_EAX)-stream)//24;result=[read32(stream+i*24) for i in range(count)];assert sorted(result)==sorted(keys) and all(v<=pivot for v in result[:cut]) and all(v>=pivot for v in result[cut:]),(count,cut,result)
   assert all(read32(stream+i*24+8)==result[i]+100 for i in range(count))
   rows.append(dict(count=count,pivot=pivot,cut=cut,result=result))
 return rows
if __name__=='__main__':print('RETAIL_DEFCLASS_PARTITION_PASS cases='+str(len(collect())))

