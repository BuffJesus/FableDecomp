"""Retail parser copy and real list node allocation; string copies mocked."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EIP
from pe_oracle import EXE,pe_sections
def collect():
 raw=EXE.read_bytes();rows=[]
 for count in (0,1,3):
  u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000)
  for _,va,vs,off,size in pe_sections(raw):
   if size:u.mem_write(va+0x400000,raw[off:off+size])
  u.mem_map(0x2000000,0x100000);source=0x2001000;dest=0x2002000;sentinel=0x2003000;stack=0x2010000;stop=0x200f000;heap=[0x2020000];allocated=[]
  def w(p,v):u.mem_write(p,struct.pack('<I',v&0xffffffff))
  def r(p):return struct.unpack('<I',u.mem_read(p,4))[0]
  fields=[0,0x2004000,123,0x2005000,0x2006000,sentinel,0xabcdef,0xa1b2c301,9,27];u.mem_write(source,struct.pack('<10I',*fields));w(sentinel,sentinel);w(sentinel+4,sentinel)
  for i in range(count):
   node=sentinel+32*(i+1);prev=r(sentinel+4);w(node,sentinel);w(node+4,prev);w(node+8,0x12340000+i*10);w(node+12,0x12340007+i*10);w(prev,node);w(sentinel+4,node)
  u.mem_write(stack,struct.pack('<II',stop,source));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,dest)
  def hook(uc,a,size,user):
   if a==stop:uc.emu_stop();return
   sp=uc.reg_read(UC_X86_REG_ESP)
   if a==0xbfea0e:
    pointer=heap[0];heap[0]+=32;allocated.append(pointer);uc.reg_write(UC_X86_REG_EAX,pointer);uc.reg_write(UC_X86_REG_ESP,sp+4);uc.reg_write(UC_X86_REG_EIP,r(sp))
   elif a==0x99ec30:
    self=uc.reg_read(UC_X86_REG_ECX);w(self,r(r(sp+4)));uc.reg_write(UC_X86_REG_EAX,self);uc.reg_write(UC_X86_REG_ESP,sp+8);uc.reg_write(UC_X86_REG_EIP,r(sp))
  u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0x46381b,stop,count=50000)
  out=list(struct.unpack('<10I',u.mem_read(dest,40)));assert out[0]==0x122d874 and out[1:5]==fields[1:5] and out[6:]==fields[6:];head=out[5];assert head!=sentinel and len(allocated)==count+1
  node=r(head);values=[]
  while node!=head:
   assert node in allocated and r(r(node)+4)==node;values.append((r(node+8),r(node+12)));node=r(node)
  assert values==[(0x12340000+i*10,0x12340007+i*10)for i in range(count)];rows.append(dict(nodes=count,allocations=len(allocated),cursor=out[7:]))
 return rows
if __name__=='__main__':print('RETAIL_PARSER_COPY_PASS cases='+str(len(collect())))
