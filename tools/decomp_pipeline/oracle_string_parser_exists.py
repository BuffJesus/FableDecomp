"""Retail conditional consume: raw string matching and cursor restoration."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EIP
from pe_oracle import EXE,pe_sections
CASES=[('  abc','',0,0xa1b2c300,7),('  ','',0,0xa1b2c301,7),('','',0,0xa1b2c300,7),('xx\n\t abc','',2,0xa1b2c301,7),('xx\n\t ','',2,0xa1b2c300,7)]
def collect():
 raw=EXE.read_bytes();rows=[]
 for source,needle,pos,quote,line in CASES:
  u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000)
  for _,va,vs,off,size in pe_sections(raw):
   if size:u.mem_write(va+0x400000,raw[off:off+size])
  u.mem_map(0x2000000,0x100000);parser=0x2001000;data=0x2002000;arg=0x2003000;block=0x2003100;buffer=0x2003200;stack=0x2010000;stop=0x200f000
  def w(p,v):u.mem_write(p,struct.pack('<I',v&0xffffffff))
  u.mem_write(parser,bytes(40));w(parser+4,data);w(parser+28,quote);w(parser+32,pos);w(parser+36,line);u.mem_write(data,source.encode()+b'\0');w(arg,block);w(block,buffer);w(block+4,len(needle));u.mem_write(buffer,needle.encode()+b'\0');u.mem_write(stack,struct.pack('<I',stop));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,parser)
  def hook(uc,a,size,user):
   if a==stop:uc.emu_stop()
   elif a==0xbfedbe:
    sp=uc.reg_read(UC_X86_REG_ESP);dest,c=struct.unpack('<II',uc.mem_read(sp,8));uc.reg_write(UC_X86_REG_EAX,int(c in (9,10,11,12,13,32)));uc.reg_write(UC_X86_REG_ESP,sp+4);uc.reg_write(UC_X86_REG_EIP,dest)
  u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0x9ba4f0,stop,count=10000)
  rows.append(dict(source=source,needle=needle,initial_position=pos,initial_quote=quote,line=line,result=bool(u.reg_read(UC_X86_REG_EAX)&255),cursor=list(struct.unpack('<III',u.mem_read(parser+28,12)))))
 return rows
if __name__=='__main__':
 import json
 print(json.dumps(collect(),indent=2))
