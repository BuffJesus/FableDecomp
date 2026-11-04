"""ResizeFile's ignored API errors, live handle reloads and stored size contract."""
import hashlib,itertools,json,re,struct,tempfile
from pathlib import Path
import check_one as V
from gate_game_component_abi import Coff
from near_match import compare,retail_relocations
from native_exact_roots import import_names
from pe_oracle import EXE,read_virtual_data
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
R=Path(__file__).resolve().parents[2]
SOURCE=R/'rebuild/src/compiled/00/98/CThreadedFile_ResizeFile_0098e090.cpp'
ENTRY,SIZE,SYMBOL=0x98e090,58,'?ResizeFile@CThreadedFile@@QAE_NK@Z'
ADDRESS={'__imp__SetFilePointer@16':0x143fde8,'__imp__SetEndOfFile@4':0x143fdec,'__imp__GetFileSize@8':0x143fdf0}
ARENA,OBJ,STACK,STOP,API=0x2000000,0x2000103,0x3000000,0x4000000,0x5000000

def audit_body(obj):
 c=Coff(obj);section,offset=c.symbols[SYMBOL];assert offset==0
 body,rels=c.sections[section];raw=EXE.read_bytes();original=read_virtual_data(raw,ENTRY,SIZE)
 assert len(body) in (58,62) and len(rels)==3
 grade=compare(body,original,tuple(rels),retail_relocations('0098e090',SIZE)).to_dict();assert grade['near_match']
 imports=import_names(raw);patched=bytearray(body);audit=[]
 for site,(name,kind) in sorted(rels.items()):
  assert kind==6 and struct.unpack_from('<I',body,site)[0]==0
  target=ADDRESS[name];assert imports[target]==name[7:].split('@')[0]
  # Approved forms have the same three API-call operand sites.
  assert struct.unpack_from('<I',original,site)[0]==target
  struct.pack_into('<I',patched,site,target);audit.append(dict(site=site,symbol=name,kind=kind,target=hex(target)))
 exact=bytes(patched)==original;assert len(body)!=58 or exact
 return bytes(patched),dict(grade=grade,acceptance_grade='MATCH' if exact else 'NEAR_MATCH',exact_bytes=exact,bytes=len(body),original_bytes=SIZE,relocations=audit)

def execute(code,args):
 requested,size_case,pointer_fail,end_fail,mutation,seed=args
 actual=(requested,requested^1,0xffffffff)[size_case]
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in ((0x98e000,0x1000),(0x143f000,0x1000),(ARENA,0x4000),(STACK,0x10000),(STOP,0x1000),(API,0x1000)):u.mem_map(a,n)
 u.mem_write(ENTRY,code);u.mem_write(ARENA,bytes((i*37+seed*19)&255 for i in range(0x4000)))
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 handle=0x12345678;put(OBJ+8,handle);expected=bytearray(u.mem_read(ARENA,0x4000));trace=[]
 for i,target in enumerate(ADDRESS.values()):put(target,API+i*16)
 def change(off,value):
  put(OBJ+off,value);struct.pack_into('<I',expected,OBJ+off-ARENA,value)
 def hook(uc,a,size,_):
  nonlocal handle
  if a not in (API,API+16,API+32):return
  index=(a-API)//16;sp=uc.reg_read(UC_X86_REG_ESP)
  assert len(trace)==index and get(sp+4)==handle
  if index==0:
   assert (get(sp+8),get(sp+12),get(sp+16))==(requested,0,0)
   trace.append(('set_pointer',handle,requested));pop=16;result=0xffffffff if pointer_fail else requested
  elif index==1:
   trace.append(('set_end',handle));pop=4;result=0 if end_fail else 1
  else:
   assert get(sp+8)==0;trace.append(('get_size',handle));pop=8;result=actual
  if index<2 and mutation&(1<<index):
   handle^=0xffffffff;change(8,handle);change(0x10,0xdead0000+index)
  if index==2 and seed:
   change(8,0xeeeeeeee);change(0x10,0xbeefbeef)
  uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_ECX,0xdeadbeef);uc.reg_write(UC_X86_REG_EDX,0x87654321)
  uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
 u.hook_add(UC_HOOK_CODE,hook)
 sp=STACK+0x8000;put(sp,STOP);put(sp+4,requested);saved=(UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP)
 for i,reg in enumerate(saved):u.reg_write(reg,0x55660000+seed+i)
 u.reg_write(UC_X86_REG_ECX,OBJ);u.reg_write(UC_X86_REG_ESP,sp)
 u.emu_start(ENTRY,STOP,count=100)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+8
 assert all(u.reg_read(reg)==0x55660000+seed+i for i,reg in enumerate(saved))
 result=u.reg_read(UC_X86_REG_EAX)&255;assert result==int(requested==actual)
 struct.pack_into('<I',expected,OBJ+0x10-ARENA,actual)
 assert len(trace)==3 and bytes(u.mem_read(ARENA,0x4000))==bytes(expected)
 return result,trace,bytes(expected)

def compare_resize():
 inputs={Path(__file__),EXE,R/'tools/decomp_pipeline/check_one.py',R/'tools/decomp_pipeline/near_match.py',R/'tools/decomp_pipeline/native_exact_roots.py',R/'tools/decomp_pipeline/gate_game_component_abi.py',R/'tools/decomp_pipeline/pe_oracle.py'}
 def closure(p):
  if p in inputs:return
  inputs.add(p)
  for name in re.findall(r'^\s*#include\s*"([^"]+)"',p.read_text(encoding='utf-8'),re.M):closure(next(q for q in (p.parent/name,R/'rebuild/include'/name) if q.is_file()))
 closure(SOURCE);pins={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs}
 with tempfile.TemporaryDirectory(prefix='threaded-file-resize-') as tmp:
  obj=Path(tmp)/'root.obj';result=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(SOURCE)],V.env(),Path(tmp))
  assert result.returncode==0,result.stdout+result.stderr
  built,grade=audit_body(obj)
 original=read_virtual_data(EXE.read_bytes(),ENTRY,SIZE);cases=0
 for args in itertools.product((0,1,0x7fffffff,0x80000000,0xffffffff),range(3),range(2),range(2),range(4),(0,7)):
  assert execute(built,args)==execute(original,args);cases+=1
 for bad in (b'\xcc',b'\x32\xc0\xc2\x04\x00'):
  detected=False
  try:execute(bad,(0,0,0,0,0,0))
  except Exception:detected=True
  assert detected
 assert all(hashlib.sha256(Path(p).read_bytes()).hexdigest()==h for p,h in pins.items())
 return dict(status='PASS',cases=cases,root_controls=2,missing_provider_checks=1,input_sha256=pins,root=grade,
  boundaries=['All three Win32 APIs controlled, not real file resizing; unsigned size and signed seek-bit patterns, ignored seek/end failures, final INVALID_FILE_SIZE comparison preserved.',
  'Live handle/length mutation at API boundaries, full16KiB memory guards and thiscall ABI; no file construction, opening or manager execution.'])
