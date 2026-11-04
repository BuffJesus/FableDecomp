"""Independent constructor model with recorded allocation/OS/string/thread boundaries."""
import struct,itertools
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_EAX
from pe_oracle import EXE,pe_sections
BASE=0x2004000;WIDTH=0x8000

def collect():
 raw=EXE.read_bytes();uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1200000)
 for _,va,vs,offset,size in pe_sections(raw):
  if size:uc.mem_write(0x400000+va,raw[offset:offset+size])
 uc.mem_map(0x2000000,0x20000);rows=[];events=[];allocs=0;handles=0;temp=0;args=();stack=0x2010000;stop=0x200f000
 def word(a,v):uc.mem_write(a,struct.pack('<I',v&0xffffffff))
 def read(a):return struct.unpack('<I',uc.mem_read(a,4))[0]
 def event(k,a=0,b=0,c=0):events.append((k,a,b,c,bytes(uc.mem_read(BASE,WIDTH))))
 def hook(uc,a,n,user):
  nonlocal allocs,handles,temp
  if a not in [0xbfea0e,0x200d000,0x200d100,0x99ebf0,0x9ac700,0x99efe0,0x99eae0,0xbfed3a]:return
  sp=uc.reg_read(UC_X86_REG_ESP);owner=uc.reg_read(UC_X86_REG_ECX);cleanup=4
  if a==0xbfea0e:
   event(1,read(sp+4),allocs);uc.reg_write(UC_X86_REG_EAX,BASE+0x1000*(allocs+1));allocs+=1
   if args[2]:word(BASE+0x9c,0x90000000+allocs-1);word(BASE+0x74,17)
  elif a==0x200d000:
   assert read(sp+4)==BASE+4;event(2,BASE+4);uc.mem_write(BASE+4,b'\x5a'*24);cleanup=8
   if args[2]:uc.mem_write(BASE+0x80,b'\0\1');uc.mem_write(BASE+0x7b,b'\1')
  elif a==0x200d100:
   assert [read(sp+4+i*4)for i in range(4)]==[0,0,0,0];event(3,handles);uc.reg_write(UC_X86_REG_EAX,0x80000000+handles if args[0]&(1<<handles)else 0);handles+=1;cleanup=20
   if args[2]:uc.mem_write(BASE+0x80,b"\7");uc.mem_write(BASE+0x7a,b"\11");word(BASE+0x84,0xabc);word(BASE+0x9c,0xa0000000+handles-1)
   if args[2]==2 and handles==1:word(0x143fdc8,0x200d400)
  elif a==0x99ebf0:
   assert read(sp+4)==0x129b448 and read(sp+8)==0xffffffff;event(4,0x129b448,0xffffffff);temp=owner;word(temp,0xfe);uc.reg_write(UC_X86_REG_EAX,temp);cleanup=12
  elif a==0x9ac700:
   assert owner==BASE+0x94 and read(sp+4)==temp and read(temp)==0xfe;event(5,owner,0xfe);uc.reg_write(UC_X86_REG_EAX,BASE+0x6000);cleanup=8
  elif a==0x99efe0:
   assert owner==BASE+0x6000 and read(sp+4)==0x129b440;event(6,owner,0x129b440);word(owner,0x12345678);uc.reg_write(UC_X86_REG_EAX,owner);cleanup=8
  elif a==0x99eae0:assert owner==temp;event(7,0xfe)
  else:
   event(8,read(sp+4),read(sp+8),read(sp+12));uc.reg_write(UC_X86_REG_EAX,args[1])
   if args[2]:uc.mem_write(BASE+0x81,b"\1")
  uc.reg_write(UC_X86_REG_EIP,read(sp));uc.reg_write(UC_X86_REG_ESP,sp+cleanup)
 uc.hook_add(UC_HOOK_CODE,hook);word(0x143fee4,0x200d000);word(0x143fdc8,0x200d100)
 for args in itertools.product([0,5,10,15],[0,0xffffffff],[0,1,2]):
  initial=bytes(b'\xa5'*WIDTH);expected=bytearray(initial);predicted=[]
  def put(o,v):struct.pack_into('<I',expected,o,v&0xffffffff)
  def predict(k,a=0,b=0,c=0):predicted.append((k,a,b,c,bytes(expected)))
  put(0,0x129b43c)
  for o in [0x1c,0x20,0x24]:put(o,0)
  for i,(o,size)in enumerate([(0x38,16),(0x3c,16),(0x40,184),(0x44,20),(0x94,24)]):
   put(o,0);predict(1,size,i)
   if args[2]:put(0x9c,0x90000000+i);put(0x74,17)
   node=BASE+0x1000*(i+1);put(o,node)
   if i<3:put(node-BASE,node);put(node-BASE+4,node)
   else:
    put(o+4,0);expected[node-BASE]=0;put(node-BASE+4,0);put(node-BASE+8,node);put(node-BASE+12,node)
  predict(2,BASE+4);expected[4:28]=b'\x5a'*24
  if args[2]:expected[0x80]=0;expected[0x81]=1;expected[0x7b]=1
  for o in [0x79,0x78,0x7a,0x7b]:expected[o]=0
  expected[0x80]=1;put(0x7c,0)
  for i in range(4):
   predict(3,i)
   if args[2]:expected[0x80]=7;expected[0x7a]=9;put(0x84,0xabc);put(0x9c,0xa0000000+i)
   put(0x84+i*4,0x80000000+i if args[0]&(1<<i)else 0)
  put(0x74,0);expected[0x78]=0
  for o in [0x50,0x54,0x28,0x2c,0x64,0x68,0x30,0x34,0x6c,0x70]:put(o,0)
  expected[0x7a]=0;put(0x58,0);put(0x5c,0);expected[0x60]=0;expected[0x81]=0
  predict(4,0x129b448,0xffffffff);predict(5,BASE+0x94,0xfe);predict(6,BASE+0x6000,0x129b440);put(0x6000,0x12345678);predict(7,0xfe);predict(8,0x9c5870,0x3000,BASE)
  if args[2]:expected[0x81]=1
  predict(8,0x9c4420,0x3000,BASE)
  events=[];allocs=handles=temp=0;uc.mem_write(BASE,initial);word(0x143fdc8,0x200d100);word(stack,stop);uc.reg_write(UC_X86_REG_ESP,stack);uc.reg_write(UC_X86_REG_ECX,BASE);uc.emu_start(0x9c5890,stop,count=3000)
  assert uc.reg_read(UC_X86_REG_EIP)==stop and uc.reg_read(UC_X86_REG_ESP)==stack+4 and uc.reg_read(UC_X86_REG_EAX)==BASE
  assert events==predicted and bytes(uc.mem_read(BASE,WIDTH))==expected,args
  rows.append(dict(args=args,initial=initial,after=bytes(expected),events=events[:]))
 return rows
if __name__=='__main__':print('FILE_INSTALLER_CTOR_RETAIL_OK cases='+str(len(collect())))
