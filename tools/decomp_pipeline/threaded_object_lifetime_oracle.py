"""Original request construction/destruction and compiler deleting-destructor ABI."""
import hashlib,itertools,re,struct,tempfile
from pathlib import Path
import check_one as V
from pe_oracle import EXE,read_virtual_data
from movable_pool_construct_oracle import compile_codes
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
R=Path(__file__).resolve().parents[2]
CTOR_SOURCE=R/'rebuild/src/compiled/00/98/CThreadedFileObject_Constructor_0098df00.cpp'
DTOR_SOURCE=R/'rebuild/src/compiled/00/98/CThreadedFileManager_OnFinishObject_0098ebc0.cpp'
ROOTS=[dict(name=name,source=source,symbol=symbol,address=address,size=size,instructions=instructions,relocations=relocations) for name,source,symbol,address,size,instructions,relocations in (
 ('object_constructor',CTOR_SOURCE,'??0CFileObject@CThreadedFileManager@@QAE@XZ',0x98df00,30,10,0),
 ('object_destructor',DTOR_SOURCE,'??1CFileObject@CThreadedFileManager@@QAE@XZ',0x98df20,15,6,1),
 ('object_deleting_destructor',DTOR_SOURCE,'??_GCFileObject@CThreadedFileManager@@QAEPAXI@Z',0x98f500,39,15,2))]
ADDRESS={'__imp__GlobalFree@4':0x143fdf8,'??3@YAXPAX@Z':0xbfe9bc}
ARENA,LENGTH,BLOCK,STACK,STOP,API=0x2000000,0x4000,0x2002000,0x3000000,0x4000000,0x5000000
INITIAL={s:bytes((i*37+s*19)&255 for i in range(LENGTH)) for s in (0,7)}

def execute(codes,root,args):
 seed,alignment,overlap,free_fail,flags=args;obj=ARENA+0x100+alignment
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in ((0x98d000,0x1000),(0x98f000,0x1000),(0xbfe000,0x1000),(0x143f000,0x1000),(ARENA,LENGTH),(STACK,0x10000),(STOP,0x1000),(API,0x1000)):
  u.mem_map(a,n);u.mem_write(a,b'\xcc'*n)
 for a,b in codes.items():u.mem_write(a,b)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 u.mem_write(ARENA,INITIAL[seed]);put(obj+0x20,overlap);put(0x143fdf8,API)
 for a in (API,0xbfe9bc):u.mem_write(a,b'\xc3')
 expected=bytearray(u.mem_read(ARENA,LENGTH));trace=[]
 def ep(a,v):struct.pack_into('<I',expected,a-ARENA,v)
 name=root['name']
 if name=='object_constructor':
  for off in (0,4,8,12,32):ep(obj+off,0)
  expected[obj-ARENA+0x10]=1;ep(obj+0x14,3)
 else:
  if overlap and not free_fail:expected[BLOCK-ARENA:BLOCK-ARENA+20]=b'\xee'*20
  if name=='object_deleting_destructor' and flags&1:expected[obj-ARENA:obj-ARENA+64]=b'\xdd'*64
 def hook(uc,a,size,_):
  if a not in (API,0xbfe9bc):return
  sp=uc.reg_read(UC_X86_REG_ESP);pointer=get(sp+4)
  if a==API:
   assert pointer==BLOCK and overlap==BLOCK
   assert not trace;trace.append(('global_free',pointer));pop=4;result=pointer if free_fail else 0
   if not free_fail:u.mem_write(pointer,b'\xee'*20)
  else:
   assert pointer==obj and name=='object_deleting_destructor' and flags&1
   assert trace==([('global_free',BLOCK)] if overlap else [])
   trace.append(('delete',pointer));u.mem_write(pointer,b'\xdd'*64);pop=0;result=0xdead1234
  uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_ECX,0xdeadbeef);uc.reg_write(UC_X86_REG_EDX,0x87654321)
  uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
 u.hook_add(UC_HOOK_CODE,hook)
 sp=STACK+0x8000;put(sp,STOP)
 deleting=name=='object_deleting_destructor'
 if deleting:put(sp+4,flags)
 saved=(UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP)
 for i,reg in enumerate(saved):u.reg_write(reg,0x55660000+seed+i)
 u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,obj)
 u.emu_start(root['address'],STOP,count=100)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+(8 if deleting else 4)
 assert all(u.reg_read(reg)==0x55660000+seed+i for i,reg in enumerate(saved))
 if name in ('object_constructor','object_deleting_destructor'):assert u.reg_read(UC_X86_REG_EAX)==obj
 wanted=[]
 if name!='object_constructor':
  if overlap:wanted.append(('global_free',BLOCK))
  if deleting and flags&1:wanted.append(('delete',obj))
 assert trace==wanted
 state=bytes(u.mem_read(ARENA,LENGTH));assert state==bytes(expected)
 return state,trace

def compare_lifetime():
 inputs={Path(__file__),EXE,CTOR_SOURCE,DTOR_SOURCE}
 for name in ('check_one.py','gate_game_component_abi.py','pe_oracle.py','movable_pool_construct_oracle.py'):
  inputs.add(R/'tools/decomp_pipeline'/name)
 def closure(p):
  for name in re.findall(r'^\s*#include\s*"([^"]+)"',p.read_text(encoding='utf-8'),re.M):
   q=R/'rebuild/include'/name;assert q.is_file()
   if q not in inputs:inputs.add(q);closure(q)
 closure(CTOR_SOURCE);closure(DTOR_SOURCE);pins={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs}
 with tempfile.TemporaryDirectory(prefix='threaded-object-lifetime-') as tmp:codes=compile_codes(ROOTS,ADDRESS,Path(tmp))
 native={r['address']:read_virtual_data(EXE.read_bytes(),r['address'],r['size']) for r in ROOTS};assert codes==native
 cases=0;witnesses={};parts={}
 for root in ROOTS:
  if root['name']=='object_constructor':matrix=itertools.product((0,7),(0,4,8,12),(0,BLOCK,0xffffffff),(False,),(0,))
  elif root['name']=='object_destructor':matrix=itertools.product((0,7),(0,4,8,12),(0,BLOCK),(False,True),(0,))
  else:matrix=itertools.product((0,7),(0,4,8,12),(0,BLOCK),(False,True),(0,1,2,3,0x100,0x80000000,0xffffffff))
  n=0
  for args in matrix:
   a=execute(codes,root,args);b=execute(native,root,args);assert a==b
   cases+=1;n+=1;witnesses.setdefault(root['address'],args)
  parts[root['name']]=n
 for root in ROOTS:
  detected=False
  try:execute({a:b for a,b in codes.items() if a!=root['address']},root,witnesses[root['address']])
  except Exception:detected=True
  assert detected
 assert all(hashlib.sha256(Path(p).read_bytes()).hexdigest()==h for p,h in pins.items())
 return dict(status='PASS',cases=cases,partitions=parts,providers=3,missing_provider_checks=3,input_sha256=pins,
  boundaries=['Constructor writes only descriptor defaults and Overlapped zero, preserves uninitialized fields/padding. Public nonvirtual destructor frees the owned global block without clearing it.',
   'Compiler-generated deleting destructor checks only flag bit0, ignores free failure, returns original pointer and performs original ret4 cleanup. All storage and ABI guarded.',
   'GlobalFree/operator delete effects controlled; real native owned allocation/deletion exercised by the worker-owner gate.'])
