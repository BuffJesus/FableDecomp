"""Retail byte binary traits with source-window overflow mocked."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EIP
from pe_oracle import EXE,pe_sections
def collect():
 raw=EXE.read_bytes();sections=pe_sections(raw);rows=[]
 for case,(count,payload_size) in enumerate([(0,0),(1,5),(3,15),(2,5)]):
  for mode in [0]:
   u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000)
   for _,va,vs,off,size in sections:
    if size:u.mem_write(va+0x400000,raw[off:off+size])
   u.mem_map(0x2000000,0x100000);stream=0x2001000;data=0x2002000;output=0x2003000;stack=0x2010000;stop=0x200f000;calls=[];heap=[0x2020000];streams={}
   def read32(p):return struct.unpack('<I',u.mem_read(p,4))[0]
   def write32(p,v):u.mem_write(p,struct.pack('<I',v))
   def hook(uc,address,size,user):
    if address==stop:uc.emu_stop()
    else:
     sp=uc.reg_read(UC_X86_REG_ESP);self=uc.reg_read(UC_X86_REG_ECX)
     def ret(value=0,pop=0):uc.reg_write(UC_X86_REG_EAX,value);uc.reg_write(UC_X86_REG_ESP,sp+4+pop);uc.reg_write(UC_X86_REG_EIP,read32(sp))
     def alloc(size):ptr=heap[0];heap[0]+=((size+15)&~15)+16;return ptr
     if address==0xa3a7d0:ret()
     elif address==0xa3aea0:u.mem_write(self,bytes(28));ret(self,4)
     elif address in (0xa3aa10,0xa3acc0):ret()
     elif address==0x996af0:streams[self]=b'';u.mem_write(self,bytes(24));ret(self)
     elif address in (0x996b30,0x996b60):streams[self]=b'';u.mem_write(self+8,bytes(16));ret()
     elif address==0x994b40:ret(len(streams[self]))
     elif address==0x995e70:
      ptr=alloc(len(streams[self])+1);u.mem_write(ptr,streams[self]+b'\0');ret(ptr)
     elif address==0x996cc0:
      ptr=read32(sp+4);size=read32(sp+8);streams[self]+=bytes(u.mem_read(ptr,size));ret(0,8)
     elif address==0xa3aa40:
      src=read32(sp+4);size=read32(sp+8);dest=read32(sp+12);capacity=read32(sp+16);assert size==len(expected) and capacity==size+1024 and bytes(u.mem_read(src,size))==expected;u.mem_write(dest,b'ABC');calls.append(size);ret(3,16)
     elif address==0xbfea0e:ret(alloc(read32(sp+4)))
     elif address==0xbfea14:ret()
     elif address==0xbfeae6:
      dest,src,size=read32(sp+4),read32(sp+8),read32(sp+12)
      if size:u.mem_write(dest,bytes(u.mem_read(src,size)))
      ret(dest)
   manager=stream;input_stream=0x2004000;offsets=0x2005000;values=0x2006000;u.mem_write(manager,bytes(208));u.mem_write(data,bytes([9,8]));u.mem_write(manager+136,struct.pack('<III',data,data+2,data+2));streams[input_stream]=bytes(range(1,payload_size+1));u.mem_write(input_stream,bytes(24));u.mem_write(offsets,struct.pack('<III',values,values+count*2,values+10));expected=b''
   for i in range(count):
    offset=(65535 if i==0 else 1) if case==3 else i*4;u.mem_write(values+i*2,struct.pack('<H',offset));expected+=struct.pack('<H',(offset+count*2)&65535)
   expected+=streams[input_stream];u.mem_write(stack,struct.pack('<IIII',stop,input_stream,offsets,17));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,manager);u.hook_add(UC_HOOK_CODE,hook)
   try:u.emu_start(0x9ae390,stop,count=1000000)
   except Exception as exc:raise AssertionError((case,hex(u.reg_read(UC_X86_REG_EIP)),str(exc)))
   assert calls==[len(expected)] and read32(offsets+4)==values and read32(offsets+8)==values+10 and streams[input_stream]==b''
   start,end=read32(manager+136),read32(manager+140);assert end-start==5 and bytes(u.mem_read(start,5))==b'\x09\x08ABC'
   first,last=read32(manager+148),read32(manager+152);assert last-first==8 and read32(first)==17 and read32(first+4)==2 and u.mem_read(manager+161,1)[0]==1
   rows.append(dict(count=count,payload_size=payload_size,packed=expected.hex(),compressed_size=3,block_index=17,block_position=2,retained_offset_capacity=5))
 return rows
if __name__=='__main__':print('RETAIL_DEFINITION_COMPRESS_BLOCK_PASS cases='+str(len(collect())))
