"""Fastcall error policy leaves, including intentional unchecked/null dispatch."""
import hashlib,itertools,re,struct,tempfile
from pathlib import Path
import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
from movable_pool_construct_oracle import compile_codes
from unicorn import Uc,UcError,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
R=Path(__file__).resolve().parents[2]
SOURCE=R/'rebuild/src/compiled/00/40/CThreadedFileManager_ErrorCallbacks_00401460.cpp'
ERROR='?m_errorCallback@CThreadedFileManager@@0P6IXXZA'
HARD='?m_harddiskErrorCallback@CThreadedFileManager@@0P6IXPBD@ZA'
DIRTY='?m_bDirtyDiscError@CThreadedFileManager@@0_NA'
ADDRESS={ERROR:0x13bc860,HARD:0x13bc864,DIRTY:0x13bc868}
ROOTS=[dict(name=name,source=SOURCE,symbol=symbol,address=address,size=size,instructions=instructions,relocations=1) for name,symbol,address,size,instructions in (
 ('set_error','?SetErrorCallback@CThreadedFileManager@@SIXP6IXXZ@Z',0x401460,7,2),
 ('set_hard','?SetHardDiskErrorCallback@CThreadedFileManager@@SIXP6IXPBD@Z@Z',0x401470,7,2),
 ('set_dirty','?SetDirtyDiscError@CThreadedFileManager@@SIX_N@Z',0x98ded0,7,2),
 ('get_hard','?GetHardDiskErrorCallback@CThreadedFileManager@@KIP6IXPBD@ZXZ',0x98dee0,6,2),
 ('call_hard','?CallHardDiskErrorCallback@CThreadedFileManager@@KIXPBD@Z',0x98def0,6,1),
 ('do_hard','?DoHardDiskErrorCallback@CThreadedFileManager@@SIXPBD@Z',0x98f9b0,12,5))]
DATA,STACK,STOP,API=0x13bc000,0x3000000,0x4000000,0x5000000
INITIAL={s:bytes((i*37+s*19)&255 for i in range(0x1000)) for s in (0,7)}

def execute(codes,root,args):
 value,callback,mutation,seed=args
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in ((0x401000,0x1000),(0x98d000,0x1000),(0x98f000,0x1000),(DATA,0x1000),(STACK,0x10000),(STOP,0x1000),(API,0x1000)):
  u.mem_map(a,n);u.mem_write(a,b'\xcc'*n)
 for a,b in codes.items():u.mem_write(a,b)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 u.mem_write(DATA,INITIAL[seed]);put(ADDRESS[HARD],callback)
 before=bytes(u.mem_read(DATA,0x1000));expected=bytearray(before);trace=[]
 for a in (API+0x100,API+0x200):u.mem_write(a,b'\xc3')
 sp=STACK+0x8000;put(sp,STOP)
 saved=(UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP)
 for i,reg in enumerate(saved):u.reg_write(reg,0x55660000+i+seed)
 u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,value);u.reg_write(UC_X86_REG_EDX,0x87654321)
 u.reg_write(UC_X86_REG_EAX,0xabcdef01)
 def hook(uc,a,size,_):
  if a not in (API+0x100,API+0x200):return
  assert a==callback and uc.reg_read(UC_X86_REG_ECX)==value
  assert uc.reg_read(UC_X86_REG_ESP)==sp and get(sp)==STOP,'Tail dispatch must retain original return address'
  trace.append((a,value))
  if mutation:
   replacement=0 if mutation==1 else API+0x200
   put(ADDRESS[HARD],replacement);struct.pack_into('<I',expected,0x864,replacement)
  if mutation==2:
   put(ADDRESS[ERROR],API+0x300);struct.pack_into('<I',expected,0x860,API+0x300)
   u.mem_write(ADDRESS[DIRTY],b'\xff');expected[0x868]=255
  uc.reg_write(UC_X86_REG_EAX,0x11223344);uc.reg_write(UC_X86_REG_ECX,0xdeadbeef);uc.reg_write(UC_X86_REG_EDX,0xaabbccdd)
 u.hook_add(UC_HOOK_CODE,hook)
 fault=False
 try:u.emu_start(root['address'],STOP,count=50)
 except UcError:
  assert root['name']=='call_hard' and callback==0 and u.reg_read(UC_X86_REG_EIP)==0
  fault=True
 name=root['name']
 if name=='set_error':struct.pack_into('<I',expected,0x860,value)
 if name=='set_hard':struct.pack_into('<I',expected,0x864,value)
 if name=='set_dirty':expected[0x868]=value&255
 if name=='get_hard':assert u.reg_read(UC_X86_REG_EAX)==callback
 called=name in ('call_hard','do_hard') and callback!=0
 assert trace==([(callback,value)] if called else [])
 assert fault==(name=='call_hard' and callback==0)
 assert u.reg_read(UC_X86_REG_ESP)==sp+(0 if fault else 4)
 assert fault or u.reg_read(UC_X86_REG_EIP)==STOP
 assert all(u.reg_read(reg)==0x55660000+i+seed for i,reg in enumerate(saved))
 state=bytes(u.mem_read(DATA,0x1000));assert state==bytes(expected)
 return (state,trace,fault),called

def compare_policy():
 inputs={Path(__file__),SOURCE,EXE}
 for name in ('check_one.py','gate_game_component_abi.py','pe_oracle.py','movable_pool_construct_oracle.py'):
  inputs.add(R/'tools/decomp_pipeline'/name)
 def closure(p):
  for name in re.findall(r'^\s*#include\s*"([^"]+)"',p.read_text(encoding='utf-8'),re.M):
   q=R/'rebuild/include'/name;assert q.is_file()
   if q not in inputs:inputs.add(q);closure(q)
 closure(SOURCE);pins={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs}
 with tempfile.TemporaryDirectory(prefix='threaded-error-policy-') as tmp:codes=compile_codes(ROOTS,ADDRESS,Path(tmp))
 raw=EXE.read_bytes();native={r['address']:read_virtual_data(raw,r['address'],r['size']) for r in ROOTS};assert codes==native
 cases=calls=faults=0;witnesses={}
 for root in ROOTS:
  if root['name'].startswith('set_'):
   matrix=itertools.product((0,1,API+0x100,API+0x200,0xffffffff,0xaabbcc80,0xaabbccff),(0,),range(1),(0,7))
  elif root['name']=='get_hard':matrix=itertools.product((0,),(0,1,API+0x100,API+0x200,0xffffffff),range(1),(0,7))
  else:matrix=itertools.product((0,DATA+0x900,DATA+0x904,ADDRESS[HARD]),(0,API+0x100,API+0x200),range(3),(0,7))
  for args in matrix:
   try:a,called=execute(codes,root,args);b,_=execute(native,root,args);assert a==b
   except Exception as e:raise AssertionError((root['name'],args)) from e
   cases+=1;calls+=called;faults+=a[2];witnesses.setdefault(root['address'],args)
 for root in ROOTS:
  detected=False
  try:execute({a:b for a,b in codes.items() if a!=root['address']},root,witnesses[root['address']])
  except Exception:detected=True
  assert detected,root['name']
 assert all(hashlib.sha256(Path(p).read_bytes()).hexdigest()==h for p,h in pins.items())
 return dict(status='PASS',cases=cases,dispatches=calls,intentional_null_faults=faults,missing_provider_checks=len(ROOTS),input_sha256=pins,
  boundaries=['Six actual fastcall roots execute against full 4KiB global storage and ABI model; error callbacks controlled.',
   'Unchecked protected dispatch intentionally jumps to null when unset; public guarded dispatch returns. No invented null safety.',
   'Raw low-byte bool ABI, opaque/null text pointers, tail return-address/stack, callee-saved registers and callback mutation of all three globals checked.'])

def audit_error_callback_data():
 with tempfile.TemporaryDirectory(prefix='threaded-error-data-') as tmp:
  obj=Path(tmp)/'root.obj';result=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(SOURCE)],V.env(),Path(tmp));assert result.returncode==0
  c=Coff(obj);sec,off=c.symbols[ERROR];body,rel=c.sections[sec];meta=c.section_metadata[sec]
  assert off==0 and not body and not rel and meta['raw_pointer']==0 and meta['raw_size']==4 and meta['characteristics']&0x80000080==0x80000080
  assert read_virtual_data(EXE.read_bytes(),0x13bc860,4)==b'\0'*4
  return dict(status='PASS',address='013bc860',symbol=ERROR,bytes=4,qualification='Four writable zero-initialized COFF bytes, no payload/relocations; original loader zero matches. Separate data credit.')
