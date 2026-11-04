"""Retail five sentinel-null failures, independently predicted partial state."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_MEM_WRITE_UNMAPPED,UcError
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_EAX
from pe_oracle import EXE,pe_sections
BASE=0x2004000;WIDTH=0x8000

def collect():
 raw=EXE.read_bytes();rows=[]
 for failed in range(5):
  uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1200000)
  for _,va,vs,offset,size in pe_sections(raw):
   if size:uc.mem_write(0x400000+va,raw[offset:offset+size])
  uc.mem_map(0x2000000,0x20000);events=[];faults=[];allocs=0
  def read(a):return struct.unpack('<I',uc.mem_read(a,4))[0]
  def hook(uc,a,n,user):
   nonlocal allocs
   if a!=0xbfea0e:return
   sp=uc.reg_read(UC_X86_REG_ESP);events.append((1,read(sp+4),allocs,0,bytes(uc.mem_read(BASE,WIDTH))));uc.reg_write(UC_X86_REG_EAX,0 if allocs==failed else BASE+0x1000*(allocs+1));allocs+=1;uc.reg_write(UC_X86_REG_EIP,read(sp));uc.reg_write(UC_X86_REG_ESP,sp+4)
  def fault(uc,access,address,size,value,user):faults.append((address,size,value));return False
  uc.hook_add(UC_HOOK_CODE,hook);uc.hook_add(UC_HOOK_MEM_WRITE_UNMAPPED,fault)
  initial=b'\xa5'*WIDTH;expected=bytearray(initial);predicted=[]
  def put(o,v):struct.pack_into('<I',expected,o,v&0xffffffff)
  put(0,0x129b43c)
  for o in [0x1c,0x20,0x24]:put(o,0)
  for i,(o,size)in enumerate([(0x38,16),(0x3c,16),(0x40,184),(0x44,20),(0x94,24)]):
   put(o,0);predicted.append((1,size,i,0,bytes(expected)))
   if i==failed:
    if i>=3:put(o+4,0)
    break
   node=BASE+0x1000*(i+1);put(o,node)
   if i<3:put(node-BASE,node);put(node-BASE+4,node)
   else:put(o+4,0);expected[node-BASE]=0;put(node-BASE+4,0);put(node-BASE+8,node);put(node-BASE+12,node)
  uc.mem_write(BASE,initial);uc.mem_write(0x2010000,struct.pack('<I',0x200f000));uc.reg_write(UC_X86_REG_ESP,0x2010000);uc.reg_write(UC_X86_REG_ECX,BASE)
  try:uc.emu_start(0x9c5890,0x200f000,count=3000)
  except UcError:pass
  assert faults==[(0,4 if failed<3 else 1,0)]and events==predicted and bytes(uc.mem_read(BASE,WIDTH))==expected
  rows.append(dict(args=(15,0,failed),initial=initial,after=bytes(expected),events=events[:]))
 return rows
if __name__=='__main__':print('FILE_INSTALLER_CTOR_FAILURE_RETAIL_OK cases='+str(len(collect())))
