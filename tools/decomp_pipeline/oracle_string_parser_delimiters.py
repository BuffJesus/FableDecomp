"""Retail conditional consume: raw string matching and cursor restoration."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EIP
from pe_oracle import EXE,pe_sections
CASES=[('abc;tail',';',0,0xa1b2c300,7),('a\"b;tail',';',0,0xa1b2c300,7),('abc','?',0,0xa1b2c300,7),('','',0,0xa1b2c300,7),('abc','',1,0xa1b2c301,7),('abc;',';',4,0xa1b2c301,7),('a\n b;',';',0,0xa1b2c300,7),('a\"x','\"',0,0xa1b2c300,7)]
def collect():
 raw=EXE.read_bytes();rows=[]
 for address in (0x9b9a20,0x9b9ba0,0x9ba330,0x9ba3e0,0x9b9ce0,0x9ba4a0):
  for source,needle,pos,quote,line in CASES:
   u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000)
   for _,va,vs,off,size in pe_sections(raw):
    if size:u.mem_write(va+0x400000,raw[off:off+size])
   u.mem_map(0x2000000,0x100000);parser=0x2001000;data=0x2002000;arg=0x2003000;block=0x2003100;buffer=0x2003200;stack=0x2010000;stop=0x200f000
   def w(p,v):u.mem_write(p,struct.pack('<I',v&0xffffffff))
   u.mem_write(parser,bytes(40));w(parser+4,data);w(parser+28,quote);w(parser+32,pos);w(parser+36,line);u.mem_write(data,source.encode()+b'\0');w(arg,block);w(block,buffer);w(block+4,len(needle));u.mem_write(buffer,needle.encode()+b'\0');u.mem_write(stack,struct.pack('<II',stop,0x2004000) if address==0x9ba4a0 else struct.pack('<I',stop) if address==0x9b9ce0 else struct.pack('<III',stop,0x2004000,arg) if address in (0x9ba330,0x9ba3e0) else struct.pack('<II',stop,arg));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,parser)
   def hook(uc,a,size,user):
    if a==stop:uc.emu_stop()
    elif a==0xbfedbe:
     sp=uc.reg_read(UC_X86_REG_ESP);dest,c=struct.unpack('<II',uc.mem_read(sp,8));uc.reg_write(UC_X86_REG_EAX,int(c in (9,10,11,12,13,32)));uc.reg_write(UC_X86_REG_ESP,sp+4);uc.reg_write(UC_X86_REG_EIP,dest)
    elif a==0x99e4c0:
     sp=uc.reg_read(UC_X86_REG_ESP);dest=struct.unpack('<I',uc.mem_read(sp,4))[0];uc.reg_write(UC_X86_REG_EAX,buffer);uc.reg_write(UC_X86_REG_ESP,sp+4);uc.reg_write(UC_X86_REG_EIP,dest)
    elif a==0xbfeb3a:
     sp=uc.reg_read(UC_X86_REG_ESP);dest,hay,need=struct.unpack('<III',uc.mem_read(sp,12))
     def cs(p):
      out=bytearray()
      while uc.mem_read(p+len(out),1)[0]:out+=uc.mem_read(p+len(out),1)
      return bytes(out)
     ix=cs(hay).find(cs(need));uc.reg_write(UC_X86_REG_EAX,hay+ix if ix>=0 else 0);uc.reg_write(UC_X86_REG_ESP,sp+4);uc.reg_write(UC_X86_REG_EIP,dest)
    elif a==0x99ebf0:
     sp=uc.reg_read(UC_X86_REG_ESP);dest,ptr,length=struct.unpack('<III',uc.mem_read(sp,12));self=uc.reg_read(UC_X86_REG_ECX)
     if length==0xffffffff:
      length=0
      while uc.mem_read(ptr+length,1)[0]:length+=1
     uc.mem_write(0x2005000,bytes(uc.mem_read(ptr,length))+b'\0');w(self,0x2005100);w(0x2005100,0x2005000);w(0x2005104,length);uc.reg_write(UC_X86_REG_EAX,self);uc.reg_write(UC_X86_REG_ESP,sp+12);uc.reg_write(UC_X86_REG_EIP,dest)
   u.hook_add(UC_HOOK_CODE,hook);u.emu_start(address,stop,count=10000)
   rows.append(dict(address=address,value=u.reg_read(UC_X86_REG_EAX),output=bytes(u.mem_read(0x2005000,struct.unpack("<I",u.mem_read(0x2005104,4))[0])).decode() if address in (0x9ba330,0x9ba3e0,0x9ba4a0) else None,source=source,needle=needle,initial_position=pos,initial_quote=quote,line=line,result=bool(u.reg_read(UC_X86_REG_EAX)&255),cursor=list(struct.unpack('<III',u.mem_read(parser+28,12)))))
 return rows
if __name__=='__main__':
 import json
 print(json.dumps(collect(),indent=2))
