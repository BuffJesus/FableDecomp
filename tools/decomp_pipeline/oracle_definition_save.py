"""Retail byte binary traits with source-window overflow mocked."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EIP
from pe_oracle import EXE,pe_sections
def collect():
 raw=EXE.read_bytes();sections=pe_sections(raw);rows=[]
 for case,payload_sizes in enumerate([[],[5],[5,5],[16382,16382],[16383,16382],[40000],[32766,0,0]]):
  for mode in [0]:
   u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000)
   for _,va,vs,off,size in sections:
    if size:u.mem_write(va+0x400000,raw[off:off+size])
   u.mem_map(0x2000000,0x300000);stream=0x2001000;data=0x2002000;output=0x2003000;stack=0x2010000;stop=0x200f000;calls=[];heap=[0x2030000];streams={};blocks=[];output=bytearray()
   def read32(p):return struct.unpack('<I',u.mem_read(p,4))[0]
   def write32(p,v):u.mem_write(p,struct.pack('<I',v))
   def hook(uc,address,size,user):
    if address==stop:uc.emu_stop()
    else:
     sp=uc.reg_read(UC_X86_REG_ESP);self=uc.reg_read(UC_X86_REG_ECX)
     def ret(value=0,pop=0):uc.reg_write(UC_X86_REG_EAX,value);uc.reg_write(UC_X86_REG_ESP,sp+4+pop);uc.reg_write(UC_X86_REG_EIP,read32(sp))
     def alloc(size):ptr=heap[0];heap[0]+=((size+15)&~15)+16;return ptr
     if address in (0x9f2450,0x9f2550):calls.append(('stats',address));ret()
     elif address==0x9d4ce0:calls.append(('save_table',read32(sp+4)));ret(0,4)
     elif address in (0x993e30,0x993ea0,0x993eb0):
      value=read32(sp+4);output.extend(struct.pack('<I',value) if address!=0x993e30 else bytes([value!=0]));ret(0,4)
     elif address==0x200b020:
      output.extend(bytes(u.mem_read(read32(sp+4),read32(sp+8))));ret(0,8)
     elif address==0x996af0:streams[self]=b'';u.mem_write(self,bytes(24));ret(self)
     elif address in (0x996b30,0x996b60):streams[self]=b'';u.mem_write(self+8,bytes(16));ret()
     elif address==0x994b40:ret(len(streams[self]))
     elif address==0x995e70:
      ptr=alloc(len(streams[self])+1);u.mem_write(ptr,streams[self]+b'\0');ret(ptr)
     elif address==0x996cc0:
      ptr=read32(sp+4);size=read32(sp+8);streams[self]+=bytes(u.mem_read(ptr,size));ret(0,8)
     elif address in (0x200b000,0x200b010):
      index=read32(self+32);dest=read32(sp+4)
      if address==0x200b010:dest=read32(dest+40)
      size=payload_sizes[index] if address==0x200b000 else 0;streams[dest]+=bytes([index+1])*size;ret(0,4)
     elif address==0x9ae390:
      data_stream=read32(sp+4);offsets=read32(sp+8);first=read32(sp+12);count=(read32(offsets+4)-read32(offsets))//2;offset_values=[struct.unpack('<H',u.mem_read(read32(offsets)+i*2,2))[0] for i in range(count)];blocks.append(dict(first=first,count=count,payload=len(streams[data_stream]),offsets=offset_values))
      position=read32(self+140)-read32(self+136);compressed=read32(self+136);u.mem_write(compressed+position,bytes([90+len(blocks)-1]));write32(self+140,compressed+position+1);map_end=read32(self+152);u.mem_write(map_end,struct.pack('<II',first,position));write32(self+152,map_end+8);u.mem_write(self+161,bytes([1]));streams[data_stream]=b'';write32(offsets+4,read32(offsets));ret(0,12)
     elif address in (0x9b5db0,0x9b7b10):ret()
     elif address==0xbfea0e:ret(alloc(read32(sp+4)))
     elif address==0xbfea14:ret()
     elif address==0xbfeae6:
      dest,src,size=read32(sp+4),read32(sp+8),read32(sp+12)
      if size:u.mem_write(dest,bytes(u.mem_read(src,size)))
      ret(dest)
   manager=stream;count=len(payload_sizes);u.mem_write(manager,bytes(208));compressed=0x2008000;map_data=0x2009000;entries=0x2011000;write32(manager+188,0x12345678);u.mem_write(manager+192,bytes([1]));u.mem_write(manager+136,struct.pack('<III',compressed,compressed+2,compressed+64));u.mem_write(manager+148,struct.pack('<III',map_data,map_data+8,map_data+128));u.mem_write(manager+164,struct.pack('<III',entries,entries+count*4,entries+count*4));write32(0x13ca854,0xabcdef01);write32(0x13caa90,0x200c000 if case==6 else 0);out_stream=0x2004000;write32(out_stream,0x200a000);write32(0x200a010,0x200b020);def_vtable=0x200a100;write32(def_vtable+84,0x200b000);write32(def_vtable+72,0x200b010)
   for i in range(count):
    entry=0x2012000+i*32;definition=0x2014000+i*64;write32(entries+i*4,entry);u.mem_write(entry,struct.pack('<IIII',10+i,20+i,30+i,definition));write32(definition,def_vtable);write32(definition+4,2);write32(definition+32,i)
   u.mem_write(stack,struct.pack('<II',stop,out_stream));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,manager);u.hook_add(UC_HOOK_CODE,hook)
   try:u.emu_start(0x9af7a0,stop,count=3000000)
   except Exception as exc:raise AssertionError((case,hex(u.reg_read(UC_X86_REG_EIP)),str(exc)))
   assert calls==[('save_table',0)]+([('stats',0x9f2450),('stats',0x9f2550)]*2 if case==6 else []),calls
   assert all(read32(0x2012000+i*32+12)==0 and read32(0x2014000+i*64+4)==1 for i in range(count))
   rows.append(dict(payload_sizes=payload_sizes,blocks=blocks,output=bytes(output).hex(),references_released=True))
 return rows
if __name__=='__main__':print('RETAIL_DEFINITION_SAVE_PASS cases='+str(len(collect())))
