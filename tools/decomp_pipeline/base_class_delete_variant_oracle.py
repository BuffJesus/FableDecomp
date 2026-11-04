"""Bound the cold, inlined CBaseClass deleting COMDAT; no reconstruction credit."""
import hashlib,itertools,struct
from pathlib import Path
from gate_game_component_abi import Coff
from near_match import compare,retail_relocations
from pe_oracle import EXE,read_virtual_data
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *

ENTRY,DTOR,DELETE,VTABLE=0x4371f0,0x99a2e0,0xbfe9bc,0x1231710
ARENA,STACK,STOP=0x2000000,0x3000000,0x4000000
SYMBOL='??_GCBaseClass@@UAEPAXI@Z'

def execute(body,destructor,args):
 flags,alignment,seed=args;obj=ARENA+64+alignment
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for start,size in ((0x437000,0x1000),(0x99a000,0x1000),(0xbfe000,0x1000),(ARENA,0x1000),(STACK,0x10000),(STOP,0x1000)):u.mem_map(start,size)
 u.mem_write(ENTRY,body);u.mem_write(DTOR,destructor)
 def put(a,value):u.mem_write(a,struct.pack('<I',value&0xffffffff))
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 u.mem_write(ARENA,bytes((i*37+seed*19)&255 for i in range(0x1000)))
 expected=bytearray(u.mem_read(ARENA,0x1000));struct.pack_into('<I',expected,obj-ARENA,VTABLE)
 if flags&1:struct.pack_into('<I',expected,obj-ARENA+4,0x5a5a0000+seed)
 trace=[]
 def hook(uc,address,size,_):
  if address!=DELETE:return
  sp=uc.reg_read(UC_X86_REG_ESP)
  assert flags&1 and not trace and get(sp+4)==obj and get(obj)==VTABLE
  trace.append(('delete',obj));put(obj+4,0x5a5a0000+seed)
  for reg,value in ((UC_X86_REG_EAX,0xdeadbeef),(UC_X86_REG_ECX,0x87654321),(UC_X86_REG_EDX,0xaabbccdd)):uc.reg_write(reg,value)
  uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+4)
 u.hook_add(UC_HOOK_CODE,hook)
 sp=STACK+0x8000;put(sp,STOP);put(sp+4,flags)
 saved=(UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP)
 for i,reg in enumerate(saved):u.reg_write(reg,0x55660000+seed+i)
 u.reg_write(UC_X86_REG_ECX,obj);u.reg_write(UC_X86_REG_ESP,sp)
 u.emu_start(ENTRY,STOP,count=100)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+8
 assert u.reg_read(UC_X86_REG_EAX)==obj
 assert all(u.reg_read(reg)==0x55660000+seed+i for i,reg in enumerate(saved))
 assert bytes(u.mem_read(ARENA,0x1000))==bytes(expected)
 assert trace==([('delete',obj)] if flags&1 else [])
 return bytes(expected),trace

def compare_variant(obj):
 c=Coff(obj);sec,off=c.symbols[SYMBOL];body,rels=c.sections[sec]
 assert off==0 and len(body)==31 and rels=={10:('??_7CBaseClass@@6B@',6),18:('??3@YAXPAX@Z',20)}
 raw=EXE.read_bytes();original=read_virtual_data(raw,ENTRY,30);destructor=read_virtual_data(raw,DTOR,7)
 assert destructor==b'\xc7\x01'+struct.pack('<I',VTABLE)+b'\xc3'
 assert original[-3:]==b'\xc2\x04\x00' and read_virtual_data(raw,ENTRY+30,1)==b'\xcc'
 assert ENTRY+8+struct.unpack_from('<i',original,4)[0]==DTOR
 assert ENTRY+21+struct.unpack_from('<i',original,17)[0]==DELETE
 patched=bytearray(body)
 for site,(name,kind) in rels.items():
  assert struct.unpack_from('<I',body,site)[0]==0
  target=VTABLE if kind==6 else DELETE
  struct.pack_into('<I',patched,site,(target-(ENTRY+site+4 if kind==20 else 0))&0xffffffff)
 flags=tuple(range(16))+(0x100,0x101,0x102,0x103,0xff00,0xff01,0x10000,0x10001,0x7ffffffe,0x7fffffff,0x80000000,0x80000001,0xfffffffc,0xfffffffd,0xfffffffe,0xffffffff)
 cases=0
 for args in itertools.product(flags,range(4),(0,7)):
  assert execute(bytes(patched),destructor,args)==execute(original,destructor,args);cases+=1
 for site,size in ((8,6),(17,5)):
  broken=bytearray(patched);broken[site:site+size]=b'\x90'*size;detected=False
  try:execute(bytes(broken),destructor,(1,1,7))
  except AssertionError:detected=True
  assert detected
 grade=compare(body,original,tuple(rels),retail_relocations('004371f0',30)).to_dict()
 return dict(status='PASS',cases=cases,controls=2,grade=grade,acceptance_grade='FUNCTIONAL_COLD_VARIANT',bytes=31,original_bytes=30,
  object_sha256=hashlib.sha256(Path(obj).read_bytes()).hexdigest(),
  boundaries=['The base deletion helper is cold in the owned file composition; no root promotion or near-match claim.',
   'Original root destructor instructions, full4KiB memory prediction, low-bit deletion flags, unaligned objects, return-this and stack/callee-save ABI.',
   'Checked deletion callback mutates owned memory and poisons volatile registers; no actual heap free or exception-unwind claim.'])
