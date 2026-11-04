"""Retail byte binary traits with source-window overflow mocked."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EIP
from pe_oracle import EXE,pe_sections
def collect():
 raw=EXE.read_bytes();sections=pe_sections(raw);rows=[]
 for value in [0,1,255,256,511,0xffffffff]:
  for slow in [False,True]:
   u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000)
   for _,va,vs,off,size in sections:
    if size:u.mem_write(va+0x400000,raw[off:off+size])
   u.mem_map(0x2000000,0x20000);stream=0x2001000;data=0x2002000;output=0x2003000;stack=0x2010000;stop=0x200f000;calls=[]
   def read32(p):return struct.unpack('<I',u.mem_read(p,4))[0]
   def write32(p,v):u.mem_write(p,struct.pack('<I',v))
   def hook(uc,address,size,user):
    if address==stop:uc.emu_stop()
    elif address==0x993ca0:
     sp=uc.reg_read(UC_X86_REG_ESP);self=uc.reg_read(UC_X86_REG_ECX);dest=read32(sp+4);assert read32(sp+8)==4;u.mem_write(dest,bytes(u.mem_read(read32(self+12),4)));write32(self+12,read32(self+12)+4);write32(self+4,read32(self+4)+4);write32(self+20,0);calls.append(4);uc.reg_write(UC_X86_REG_ESP,sp+12);uc.reg_write(UC_X86_REG_EIP,read32(sp))
   u.mem_write(stream,struct.pack('<IIIIIII',0,8,32,data,0,2 if slow else 8,0));write32(data,value);u.mem_write(output,bytes([77]));u.mem_write(stack,struct.pack('<III',stop,stream,output));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,0x2004000);u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0x405940,stop,count=100000)
   result=u.mem_read(output,1)[0];assert result==value&255 and read32(stream+4)==12 and read32(stream+12)==data+4 and read32(stream+20)==(0 if slow else 4) and len(calls)==slow,(value,slow,result)
   rows.append(dict(value=value,slow=slow,result=result,position=12,overflow_calls=len(calls)))
 return rows
if __name__=='__main__':print('RETAIL_BYTE_BINARY_PASS cases='+str(len(collect())))
