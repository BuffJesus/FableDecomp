"""Original CRT workers and registered destructors with explicit string callbacks."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_ECX,UC_X86_REG_EAX
from pe_oracle import EXE,pe_sections
BASE=0x24000000;GLOBALS=[0x1436ec4,0x1436ec8,0x1436ecc]
NAMES=[b'__ENGINE_SEA_STATIC_MAP_BANK_FILE__',b'__ENGINE_WATER_STATIC_MAP_BANK_FILE__',b'__STATIC_MAP_COMMON_HEADER__']
def collect():
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000);u.mem_map(BASE,0x10000);raw=EXE.read_bytes()
 for _,va,_,off,n in pe_sections(raw):
  if n:u.mem_write(0x400000+va,raw[off:off+n])
 expected=bytearray();registered=[];snapshots=0
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def put(a,v):u.mem_write(a,struct.pack('<I',v))
 def record(kind,arg):
  nonlocal snapshots
  expected.extend(struct.pack('<5I',kind,arg,*(get(a) for a in GLOBALS)));expected.extend(u.mem_read(BASE,1024));snapshots+=1
 def hook(u,a,n,_):
  if a not in (0x4012bc,0x99ebf0,0x99eae0):return
  sp=u.reg_read(UC_X86_REG_ESP);extra=0
  if a==0x4012bc:registered.append(get(sp+4));u.reg_write(UC_X86_REG_EAX,0)
  else:
   self=u.reg_read(UC_X86_REG_ECX);index=GLOBALS.index(self)
   if a==0x99ebf0:
    assert get(sp+8)==0xffffffff and bytes(u.mem_read(get(sp+4),len(NAMES[index])+1))==NAMES[index]+b'\0'
    record(1,index);put(self,BASE+0x100+32*index);put(BASE+0x100+32*index,index+1);record(2,index);extra=8;u.reg_write(UC_X86_REG_EAX,self)
   else:record(3,index);put(BASE+0x100+32*index,0);put(self,0);record(4,index)
  u.reg_write(UC_X86_REG_ESP,sp+4+extra);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook)
 def call(a):
  sp=BASE+0xe000;stop=BASE+0xf000;put(sp,stop);u.reg_write(UC_X86_REG_ESP,sp);u.emu_start(a,stop,count=1000);assert u.reg_read(UC_X86_REG_ESP)==sp+4
 for a in GLOBALS:put(a,0)
 for a in (0x1224680,0x12246a0,0x12246c0):call(a)
 assert registered==[0x122acd0,0x122ace0,0x122acf0];record(5,0)
 for a in reversed(registered):call(a)
 record(9,0);return expected,dict(snapshots=snapshots,registered=registered)
