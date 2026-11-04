"""Retail byte binary traits with source-window overflow mocked."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_EDX,UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EIP
from pe_oracle import EXE,pe_sections
def collect():
 raw=EXE.read_bytes();sections=pe_sections(raw);rows=[]
 for count in range(0,41):
  for mode in [0,1]:
   u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000)
   for _,va,vs,off,size in sections:
    if size:u.mem_write(va+0x400000,raw[off:off+size])
   u.mem_map(0x2000000,0x20000);stream=0x2001000;data=0x2002000;output=0x2003000;stack=0x2010000;stop=0x200f000;calls=[]
   def read32(p):return struct.unpack('<I',u.mem_read(p,4))[0]
   def write32(p,v):u.mem_write(p,struct.pack('<I',v))
   def hook(uc,address,size,user):
    if address==stop:uc.emu_stop()
    elif address in (0x99ec30,0x99efb0,0x535470,0x45bc09):
     sp=uc.reg_read(UC_X86_REG_ESP);self=uc.reg_read(UC_X86_REG_ECX);source=read32(sp+4);u.mem_write(self,bytes(u.mem_read(source,12 if address in (0x535470,0x45bc09) else 4)));uc.reg_write(UC_X86_REG_EAX,self);uc.reg_write(UC_X86_REG_ESP,sp+8);uc.reg_write(UC_X86_REG_EIP,read32(sp))
    elif address in (0xbfea14,0x99eae0):
     sp=uc.reg_read(UC_X86_REG_ESP);uc.reg_write(UC_X86_REG_ESP,sp+4);uc.reg_write(UC_X86_REG_EIP,read32(sp))
   keys=([0]+[(i*17)%31+1 for i in range(1,count)]) if count else [];middle=count//2 if mode else count
   for i,key in enumerate(keys):u.mem_write(stream+i*24,struct.pack('<IIIIII',key,0,key+100,0,0,0))
   u.mem_write(stack,struct.pack('<IIII',stop,stream+count*24,0,0));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,stream);u.reg_write(UC_X86_REG_EDX,stream+middle*24);u.hook_add(UC_HOOK_CODE,hook)
   try:u.emu_start(0x9b9030,stop,count=1000000)
   except Exception as exc:raise AssertionError((count,mode,hex(u.reg_read(UC_X86_REG_EIP)),str(exc)))
   result=[read32(stream+i*24) for i in range(count)];assert sorted(result)==sorted(keys) and result[:middle]==sorted(keys)[:middle],(count,middle,result)
   assert all(read32(stream+i*24+8)==result[i]+100 for i in range(count))
   rows.append(dict(count=count,middle=middle,result=result))
 return rows
if __name__=='__main__':print('RETAIL_DEFCLASS_PARTIAL_SORT_PASS cases='+str(len(collect())))
