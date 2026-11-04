"""Retail read/write publication blocks use distinct physical-sort counters."""
import itertools,struct
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
from threaded_file_open_write_oracle import ADDRESS as WRITE_ADDRESS
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32
from unicorn.x86_const import *

ROOTS=[('?OpenForWrite@CThreadedFile@@QAE_NABVCWideString@@_N@Z',0x98e000,143,103,132),
 ('?Open@CThreadedFile@@QAE_NABVCWideString@@_N@Z',0x98e1e0,310,261,286)]
ADDRESS={**WRITE_ADDRESS,'_s_NextReadPhysicalSortKey':0x13bc9ec,
 '??0CWideString@@QAE@XZ':0x99aed0,'?SplitPath@CAFile@@SIXABVCWideString@@PAV2@111@Z':0x997620,
 '?ToCharString@CWideString@@QBE?AVCCharString@@XZ':0x99b2c0,'?GetAsUpperCase@CCharString@@QBE?AV1@XZ':0x99ee20,
 '??_C@_00CNPNBAHC@?$AA@':0x122d70e,'??_C@_02FCIENNII@D?3?$AA@':0x129a15c,
 '??8?$CBasicString@D@@QBE_NPBD@Z':0x4115a0,'??1CCharString@@QAE@XZ':0x99eae0,
 '??4CWideString@@QAEAAV0@ABV0@@Z':0x99b7d0}
ARENA,GLOBAL,STACK=0x2000000,0x13bc000,0x3000000

def relocated(obj,require_exact=True):
 c=Coff(obj);raw=EXE.read_bytes();codes={};reports=[]
 for symbol,entry,size,start,end in ROOTS:
  sec,off=c.symbols[symbol];body,rels=c.sections[sec];assert off==0 and len(body)==size
  original=read_virtual_data(raw,entry,size);patched=bytearray(body);audit=[]
  for site,(name,kind) in sorted(rels.items()):
   assert kind in (6,20) and struct.unpack_from('<i',body,site)[0]==0;target=ADDRESS[name]
   operand=struct.unpack_from('<I',original,site)[0];original_target=(entry+site+4+operand)&0xffffffff if kind==20 else operand
   if require_exact:assert original_target==target,(symbol,site,name,hex(original_target),hex(target))
   struct.pack_into('<I',patched,site,(target-(entry+site+4 if kind==20 else 0))&0xffffffff)
   audit.append(dict(site=site,symbol=name,kind=kind,target=hex(target),retail_target=hex(original_target)))
  if require_exact:assert bytes(patched)==original
  codes[entry]=bytes(patched);reports.append(dict(symbol=symbol,address=hex(entry),bytes=size,grade='MATCH' if bytes(patched)==original else 'MISMATCH',relocations=audit))
 if require_exact:
  places=[]
  for name,address in (('_s_NextPhysicalSortKey',0x13bc9e8),('_s_NextReadPhysicalSortKey',0x13bc9ec)):
   sec,off=c.symbols[name];body,rels=c.sections[sec];meta=c.section_metadata[sec]
   assert body==b'' and not rels and meta['raw_pointer']==0 and meta['characteristics']&0x80 and off+4<=meta['raw_size']
   assert read_virtual_data(raw,address,4)==b'\0'*4;places.append((sec,off))
  assert places[0]!=places[1]
 return codes,reports

def execute(codes,args):
 sequence,initial,length,seed=args;u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in ((0x98e000,0x1000),(ARENA,0x1000),(GLOBAL,0x1000),(STACK,0x1000)):u.mem_map(a,n)
 for a,body in codes.items():u.mem_write(a,body)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 u.mem_write(ARENA,bytes((i*37+seed*19)&255 for i in range(0x1000)))
 u.mem_write(GLOBAL,bytes((i*13+seed*7)&255 for i in range(0x1000)))
 put(GLOBAL+0x9e8,initial[0]);put(GLOBAL+0x9ec,initial[1])
 expected={a:bytearray(u.mem_read(a,0x1000)) for a in (ARENA,GLOBAL)};counters=list(initial);states=[]
 for step,kind in enumerate(sequence):
  symbol,entry,size,start,end=ROOTS[kind];obj=ARENA+64+step*32
  counters[kind]=(counters[kind]+1)&0xffffffff
  struct.pack_into('<I',expected[GLOBAL],0x9e8+kind*4,counters[kind])
  struct.pack_into('<II',expected[ARENA],obj-ARENA+0x10,length,counters[kind])
  expected[ARENA][obj-ARENA+0x19]=1
  if kind==0:expected[ARENA][obj-ARENA+0x18]=1
  for reg,value in ((UC_X86_REG_EAX,length),(UC_X86_REG_EBX,0x12345678),(UC_X86_REG_ESI,obj),(UC_X86_REG_EDI,0x87654321),(UC_X86_REG_EBP,obj),(UC_X86_REG_ESP,STACK+0x800)):u.reg_write(reg,value)
  u.emu_start(entry+start,entry+end,count=20)
  assert u.reg_read(UC_X86_REG_EIP)==entry+end and u.reg_read(UC_X86_REG_ESP)==STACK+0x800
  assert u.reg_read(UC_X86_REG_EBX)==0x12345678 and u.reg_read(UC_X86_REG_EDI)==0x87654321
  assert u.reg_read(UC_X86_REG_ESI)==obj and u.reg_read(UC_X86_REG_EBP)==obj
  assert all(bytes(u.mem_read(a,0x1000))==bytes(b) for a,b in expected.items()),(sequence,step,initial)
  states.append(tuple(counters))
 return states,tuple(bytes(expected[a]) for a in (ARENA,GLOBAL))

def compare_counters(obj):
 built,roots=relocated(obj);raw=EXE.read_bytes();original={entry:read_virtual_data(raw,entry,size) for _,entry,size,_,_ in ROOTS}
 cases=0
 for args in itertools.product(itertools.product(range(2),repeat=6),((0,0),(3,17),(0xffffffff,1),(0xfffffffe,0xffffffff)),(0,0xffffffff),(0,7)):
  assert execute(built,args)==execute(original,args);cases+=1
 broken={**built};read=bytearray(broken[0x98e1e0])
 for site in (265,275):struct.pack_into('<I',read,site,0x13bc9e8)
 broken[0x98e1e0]=bytes(read);detected=False
 try:execute(broken,((0,1,0,1,1,0),(0,0),17,0))
 except AssertionError:detected=True
 assert detected
 return dict(status='PASS',cases=cases,publication_blocks=cases*6,roots=roots,controls=1,
  boundary='Whole read/write roots and distinct BSS identities audited against retail; bounded six-operation success-publication sequences execute original/rebuilt instruction blocks, with full8KiB memory and preserved-register checks. Helpers, failure branches, full entry/return ABI and OS opens are outside this block gate.')
