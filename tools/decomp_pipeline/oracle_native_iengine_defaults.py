"""Native engine default methods: full-state snapshots and ABI return values."""
import itertools,json,struct
from pathlib import Path
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32
from unicorn.x86_const import *
from pe_oracle import EXE,pe_sections
BASE=0x24000000
SPECS=json.loads((Path(__file__).with_name('native_iengine_defaults.json')).read_text())
def collect(specs=SPECS):
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000);u.mem_map(BASE,0x10000);raw=EXE.read_bytes()
 for _,va,_,off,n in pe_sections(raw):
  if n:u.mem_write(0x400000+va,raw[off:off+n])
 u.mem_write(BASE+0x9000,b'\xd9\x1d'+struct.pack('<I',BASE+0x8000))
 inputs=bytearray();expected=bytearray();cases=0
 for e,fill,pattern in itertools.product(specs,range(256),range(4)):
  arena=bytes([fill])*1024;args=[(BASE+0x100+i*32 if pattern==0 else 0 if pattern==1 else 0xffffffff if pattern==2 else 0x12345678+i)for i in range(6)]
  if e['kind']=='colour':args[0]=BASE+(0x80,0x100,0x180,0x200)[pattern]
  inputs.extend(struct.pack('<7I',e['slot'],*args));inputs.extend(arena);u.mem_write(BASE,arena);sp=BASE+0xe000;stop=BASE+0xf000;u.mem_write(sp,struct.pack('<7I',stop,*args));u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,BASE);u.reg_write(UC_X86_REG_EAX,0x87654321)
  u.emu_start(int(e['retail_address'],16),stop,count=100);assert u.reg_read(UC_X86_REG_ESP)==sp+4+e['words']*4
  kind=e['kind'];value=0
  if kind=='float':
   u.emu_start(BASE+0x9000,BASE+0x9006,count=2);value=struct.unpack('<I',u.mem_read(BASE+0x8000,4))[0]
  elif kind!='void':value=u.reg_read(UC_X86_REG_EAX)&(255 if kind=='byte'else 0xffffffff)
  expected.extend(struct.pack('<2I',e['slot'],value));expected.extend(u.mem_read(BASE,1024));cases+=1
 return inputs,expected,dict(cases=cases,snapshots=cases)
