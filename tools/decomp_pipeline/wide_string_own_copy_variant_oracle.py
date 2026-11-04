"""Validate the operator+ TU's legitimate near copy-on-write COMDAT variant."""
import hashlib,itertools,struct
from pathlib import Path
from gate_game_component_abi import Coff
from near_match import compare,retail_relocations
from pe_oracle import EXE,read_virtual_data
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ENTRY=0x99b560
SYMBOL='?MakeOwnCopyOfData@CWideString@@AAEXXZ'
ARENA,OBJ,OLD,TEXT,NEW,NEW_TEXT=0x2000000,0x2000100,0x2000200,0x2001000,0x2000300,0x2002000
STACK,STOP=0x3000000,0x4000000
ADDRESS={'??2@YAPAXI@Z':0xbfea1a,'?UnassignString@CWideString@@AAEXXZ':0x99b4d0,
 '??$_M_range_initialize@PBG@?$basic_string@GU?$char_traits@G@_STL@@V?$allocator@G@2@@_STL@@QAEXPBG0ABUforward_iterator_tag@1@@Z':0x99c670}

def execute(codes,args):
 present,owners,length,seed=args;copy=present and 1<owners<0x80000000
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in ((0x99b000,0x2000),(0xbfe000,0x1000),(ARENA,0x4000),(STACK,0x10000),(STOP,0x1000)):u.mem_map(a,n)
 for a,b in codes.items():u.mem_write(a,b)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 u.mem_write(ARENA,bytes((i*37+seed*19)&255 for i in range(0x4000)))
 text=b''.join(struct.pack('<H',0x400+i*17+seed) for i in range(length))+b'\0\0'
 u.mem_write(TEXT,text);put(OBJ,OLD if present else 0)
 for offset,value in ((0,TEXT),(4,TEXT+length*2),(8,TEXT+(length+1)*2),(12,owners)):put(OLD+offset,value)
 expected=bytearray(u.mem_read(ARENA,0x4000));trace=[]
 if copy:
  for a,value in ((OBJ,NEW),(OLD+12,owners-1),(NEW,NEW_TEXT),(NEW+4,NEW_TEXT+length*2),(NEW+8,NEW_TEXT+(length+1)*2),(NEW+12,1)):
   struct.pack_into('<I',expected,a-ARENA,value)
  expected[NEW_TEXT-ARENA:NEW_TEXT-ARENA+len(text)]=text
 def hook(uc,a,size,_):
  if a not in (0xbfea1a,0xbfea0e,0xbfeae6,0xbfea14,0xbfe9bc):return
  sp=uc.reg_read(UC_X86_REG_ESP)
  if a==0xbfea1a:
   assert copy and not trace and get(sp+4)==16;trace.append(('new_data',16));result=NEW
  elif a==0xbfea0e:
   assert trace==[('new_data',16)] and get(sp+4)==len(text);trace.append(('malloc',len(text)));result=NEW_TEXT
  elif a==0xbfeae6:
   assert trace==[('new_data',16),('malloc',len(text))] and tuple(get(sp+4+4*i) for i in range(3))==(NEW_TEXT,TEXT,length*2)
   trace.append(('memmove',length*2));u.mem_write(NEW_TEXT,bytes(u.mem_read(TEXT,length*2)));result=NEW_TEXT
  else:raise AssertionError('A valid shared buffer remains owned; no old-data deallocation is allowed')
  uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_ECX,0xdeadbeef);uc.reg_write(UC_X86_REG_EDX,0x87654321)
  uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+4)
 u.hook_add(UC_HOOK_CODE,hook)
 sp=STACK+0x8000;put(sp,STOP);saved=(UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP)
 for i,reg in enumerate(saved):u.reg_write(reg,0x55660000+seed+i)
 u.reg_write(UC_X86_REG_ECX,OBJ);u.reg_write(UC_X86_REG_ESP,sp)
 u.emu_start(ENTRY,STOP,count=1000)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+4
 assert all(u.reg_read(reg)==0x55660000+seed+i for i,reg in enumerate(saved))
 assert bytes(u.mem_read(ARENA,0x4000))==bytes(expected)
 assert trace==([('new_data',16),('malloc',len(text))]+([('memmove',length*2)] if length else []) if copy else [])
 return trace,bytes(expected)

def compare_variant(obj):
 c=Coff(obj);sec,off=c.symbols[SYMBOL];body,rels=c.sections[sec];assert off==0 and len(body)==124
 original=read_virtual_data(EXE.read_bytes(),ENTRY,128)
 grade=compare(body,original,tuple(rels),retail_relocations('0099b560',128)).to_dict();assert grade['near_match'] and grade['opcode_similarity']>=.90
 patched=bytearray(body)
 for site,(name,kind) in rels.items():
  assert kind==20 and struct.unpack_from('<i',body,site)[0]==0
  struct.pack_into('<I',patched,site,(ADDRESS[name]-ENTRY-site-4)&0xffffffff)
 common={0x99b4d0:read_virtual_data(EXE.read_bytes(),0x99b4d0,62),0x99c670:read_virtual_data(EXE.read_bytes(),0x99c670,98)}
 native={**common,ENTRY:original};built={**common,ENTRY:bytes(patched)};cases=0
 for args in itertools.product((False,True),(0,1,2,3,0x7fffffff,0x80000000,0xffffffff),(0,1,7,16),(0,7)):
  assert execute(built,args)==execute(native,args);cases+=1
 for address in (ENTRY,0x99b4d0):
  detected=False
  try:execute({**built,address:b'\xc3'},(True,2,1,0))
  except Exception:detected=True
  assert detected
 return dict(status='PASS',cases=cases,grade=grade,acceptance_grade='NEAR_MATCH',bytes=124,original_bytes=128,controls=2,
  object_sha256=hashlib.sha256(Path(obj).read_bytes()).hexdigest(),
  boundaries=['Valid old-data ownership and nonmutating successful CRT allocations; original range initialization/UnassignString machine code executes on both sides.',
  'Full16KiB independent memory prediction, null/unique/shared/signed refcount patterns, lengths0/1/7/16, stack/register ABI; allocation failure and arbitrary allocator mutation excluded.'])
