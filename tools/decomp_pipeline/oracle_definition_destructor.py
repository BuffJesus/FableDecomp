"""Retail byte binary traits with source-window overflow mocked."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EIP
from pe_oracle import EXE,pe_sections
def collect():
 raw=EXE.read_bytes();sections=pe_sections(raw);rows=[]
 for nodes in range(4):
  for mode in [0]:
   u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000)
   for _,va,vs,off,size in sections:
    if size:u.mem_write(va+0x400000,raw[off:off+size])
   u.mem_map(0x2000000,0x20000);stream=0x2001000;data=0x2002000;output=0x2003000;stack=0x2010000;stop=0x200f000;calls=[]
   def read32(p):return struct.unpack('<I',u.mem_read(p,4))[0]
   def write32(p,v):u.mem_write(p,struct.pack('<I',v))
   def hook(uc,address,size,user):
    if address==stop:uc.emu_stop()
    elif address in (0x9ad950,0x99b510,0x99eae0,0x9fc5c0,0x200a000):
     calls.append((address,uc.reg_read(UC_X86_REG_ECX)));sp=uc.reg_read(UC_X86_REG_ESP);uc.reg_write(UC_X86_REG_ESP,sp+4);uc.reg_write(UC_X86_REG_EIP,read32(sp))
    elif address in (0xbfea14,0xbfe9bc):
     sp=uc.reg_read(UC_X86_REG_ESP);calls.append((address,read32(sp+4)));uc.reg_write(UC_X86_REG_ESP,sp+4);uc.reg_write(UC_X86_REG_EIP,read32(sp))
   manager=stream;head=0x2004000;info=0x2005000;u.mem_write(manager,bytes(208));write32(manager+196,head);write32(manager+200,nodes);write32(head+8,head);write32(head+12,head);prior=0;pointees=[]
   for i in range(nodes):
    node=0x2006000+i*32;pointee=0x2007000+i*32;write32(node+(12 if i==2 else 8),prior);write32(node+20,pointee);write32(pointee+4,2);pointees.append(pointee);prior=node
   write32(head+4,prior);write32(manager+88,1);write32(manager+92,info);u.mem_write(info,struct.pack('<III',1,0x200a000,0));u.mem_write(stack,struct.pack('<I',stop));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,manager);u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0x9b02e0,stop,count=100000)
   assert read32(manager+200)==0 and read32(head+4)==0 and read32(head+8)==head and read32(head+12)==head
   assert all(read32(p+4)==1 for p in pointees);assert read32(manager+88)==0 and read32(manager+92)==0
   assert calls[0]==(0x9ad950,manager) and calls[-1]==(0x9fc5c0,manager),calls
   frees=[ptr for address,ptr in calls if address==0xbfea14];assert len(frees)==nodes+1 and frees[-1]==head,(nodes,frees)
   assert sum(address==0x200a000 for address,ptr in calls)==1
   rows.append(dict(nodes=nodes,remaining_references=[read32(p+4) for p in pointees],free_count=len(frees),callback_count=1))
 return rows
if __name__=='__main__':print('RETAIL_DEFINITION_DTOR_PASS cases='+str(len(collect())))
