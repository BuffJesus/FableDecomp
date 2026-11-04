"""Retail mesh/cache paths and writable cache shell-folder concatenation."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from pe_oracle import EXE,pe_sections
BASE=0x24000000
def collect():
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000);u.mem_map(BASE,0x10000);raw=EXE.read_bytes()
 for _,va,_,off,n in pe_sections(raw):
  if n:u.mem_write(0x400000+va,raw[off:off+n])
 inputs=bytearray();expected=bytearray();cases=snapshots=0;text=bytes(520);suffix=bytes(64)
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def put(a,v):u.mem_write(a,struct.pack('<I',v))
 def wide(a,size):
  b=bytearray()
  for i in range(size//2):
   v=bytes(u.mem_read(a+i*2,2));b.extend(v)
   if v==b'\0\0':return bytes(b)+bytes(size-len(b))
  raise AssertionError('unterminated test path')
 def event(kind,a=0,b=0,c=0):
  nonlocal snapshots
  expected.extend(struct.pack('<4I',kind,a,b,c));expected.extend(u.mem_read(BASE,256));expected.extend(text);expected.extend(suffix);snapshots+=1
 def hook(u,a,n,_):
  nonlocal text,suffix
  if a not in (BASE+0x9000,0x99b6b0,0x99bf30,0x99b510,0xbfe9f9):return
  sp=u.reg_read(UC_X86_REG_ESP);this=u.reg_read(UC_X86_REG_ECX);cleanup=0;result=this
  if a==BASE+0x9000:
   assert [get(sp+i*4)for i in range(1,5)]==[0,26,0,0];event(1,26,status);u.mem_write(get(sp+20),path);cleanup=20;result=status
   if mode==1:put(BASE+0x10,0x12345678)
  elif a==0x99b6b0:
   text=wide(get(sp+4),520);event(2,1 if this==BASE else 2);put(this,BASE+(0xd00 if this==BASE else 0xd20));cleanup=4
   if mode==1:put(BASE+0x14,0x87654321)
  elif a==0x99bf30:
   assert this==BASE;left=u.reg_read(UC_X86_REG_EDX);suffix=wide(get(sp+4),64);event(3,get(left));put(this,BASE+0xd40);cleanup=4
   if mode==2:put(left,BASE+0xd80)
  elif a==0x99b510:
   assert this!=BASE;event(4,get(this));put(this,0)
   if mode==3:put(BASE,BASE+0xd90)
  else:
   assert this==get(0x139c8a8);result=u.reg_read(UC_X86_REG_EAX)
  u.reg_write(UC_X86_REG_EAX,result);u.reg_write(UC_X86_REG_ESP,sp+4+cleanup);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook);put(0x1440280,BASE+0x9000)
 paths=['',r'C:\Users\Player\AppData\Roaming',r'C:\Users\Zoë\AppData\Roaming','C:\\使用者\\应用数据','C:\\', 'A'*259]
 rows=[]
 for fill,which,mode in itertools.product([0,0x55,0xaa,0xff],range(2),range(4)):rows.append((fill,which,mode,0,bytes(520)))
 for fill,value,status,mode in itertools.product([0,0x55,0xaa,0xff],paths,[0,1,0x80004005],range(4)):
  path=(value+'\0').encode('utf-16le');path+=bytes(520-len(path));rows.append((fill,2,mode,status,path))
 for fill,which,mode,status,path in rows:
  inputs.extend(struct.pack('<4I',fill,which,mode,status));inputs.extend(path);u.mem_write(BASE,bytes([fill])*256);text=bytes(520);suffix=bytes(64)
  sp=BASE+0xe000;stop=BASE+0xf000;put(sp,stop);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,BASE);u.emu_start([0x41a1c0,0x41a200,0x41a220][which],stop,count=10000);assert u.reg_read(UC_X86_REG_ESP)==sp+4 and u.reg_read(UC_X86_REG_EAX)==BASE;event(9);cases+=1
 return inputs,expected,dict(cases=cases,snapshots=snapshots)
