"""File construction, real virtual deleting entry and ordinary destructor contract."""
import hashlib,itertools,re,struct,tempfile
from pathlib import Path
import check_one as V
from gate_game_component_abi import Coff
from near_match import compare,retail_relocations
from native_exact_roots import import_names
from pe_oracle import EXE,read_virtual_data
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
R=Path(__file__).resolve().parents[2]
CTOR=R/'rebuild/src/compiled/00/98/CThreadedFile_Constructor_0098dfd0.cpp'
SOURCE=R/'rebuild/src/compiled/00/98/CThreadedFile_Close_0098ec40.cpp'
CTORSYM='??0CThreadedFile@@QAE@XZ';DELETESYM='??_GCThreadedFile@@UAEPAXI@Z';DTORSYM='??1CThreadedFile@@UAE@XZ'
ADDRESS={'??0CBaseClassNonCopyable@@QAE@XZ':0x99a2f0,'??0CWideString@@QAE@XZ':0x99aed0,'??_7CThreadedFile@@6B@':0x129a158,
 '?g_FableThreadedFileManager_013BC870@@3VCThreadedFileManager@@A':0x13bc870,'?CloseAllStreamsForFile@CThreadedFileManager@@QAEXPAVCThreadedFile@@@Z':0x98e7d0,
 '__imp__CloseHandle@4':0x143fe30,'??1CWideString@@QAE@XZ':0x99b510,'??1CBaseClassNonCopyable@@UAE@XZ':0x99a300,'??3@YAXPAX@Z':0xbfe9bc}
ARENA,OBJ,GLOBAL,STACK,STOP,API,PLAIN=0x2000000,0x2000103,0x13bc000,0x3000000,0x4000000,0x5000000,0x98f090

def bodies(directory,source=SOURCE):
 objects={};reports=[];codes={};raw=EXE.read_bytes();imports=import_names(raw)
 for path,symbol,entry,size,expected_grade in ((CTOR,CTORSYM,0x98dfd0,41,'MATCH'),(source,DELETESYM,0x991450,88,'MATCH'),(source,DTORSYM,PLAIN,67,'MATCH')):
  if path not in objects:
   obj=directory/('provider%d.obj'%len(objects));result=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(path)],V.env(),directory)
   assert result.returncode==0,result.stdout+result.stderr
   objects[path]=Coff(obj)
  c=objects[path];sec,off=c.symbols[symbol];assert off==0;body,rels=c.sections[sec];patched=bytearray(body)
  if size:
   original=read_virtual_data(raw,entry,size);grade=compare(body,original,tuple(rels),retail_relocations('%08x'%entry,size)).to_dict();assert grade['near_match']
  else:grade=None
  audit=[]
  for site,(name,kind) in sorted(rels.items()):
   assert kind in (6,20) and struct.unpack_from('<i',body,site)[0]==0
   target=ADDRESS[name]
   if name.startswith('__imp__'):assert imports[target]==name[7:].split('@')[0]
   struct.pack_into('<I',patched,site,(target-(entry+site+4 if kind==20 else 0))&0xffffffff)
   audit.append(dict(site=site,symbol=name,kind=kind,target=hex(target)))
  if expected_grade=='MATCH':assert bytes(patched)==original
  codes[entry]=bytes(patched);reports.append(dict(symbol=symbol,address=hex(entry) if size else None,bytes=len(body),acceptance_grade=expected_grade,grade=grade,relocations=audit))
 assert len(codes[0x991450])==88 and len(codes[PLAIN])==67
 # Constructor's actual one-slot vtable names this deleting symbol, not its ctor.
 c=objects[CTOR];sec,off=c.symbols['??_7CThreadedFile@@6B@'];body,rels=c.sections[sec]
 vector='??_ECThreadedFile@@UAEPAXI@Z'
 assert off==0 and len(body)==4 and body==b'\0'*4 and rels=={0:(vector,6)}
 # VC71 emits a weak vector-name fallback to the actual scalar helper. Audit the
 # raw COFF auxiliary record, rather than inventing another table function.
 ptr,count=struct.unpack_from('<II',c.data,8);aliases=[];i=0
 while i<count:
  at=ptr+i*18;storage,aux=struct.unpack_from('<BB',c.data,at+16)
  if c.indices[i]==vector:
   assert storage==105 and aux==1
   fallback,search=struct.unpack_from('<II',c.data,at+18)
   assert search==2 and c.indices[fallback]==DELETESYM;aliases.append(vector)
  i+=1+aux
 assert aliases==[vector]
 assert read_virtual_data(raw,0x129a158,4)==struct.pack('<I',0x991450)
 return codes,reports

def execute(codes,entry,args):
 handle,flags,close_fail,mutation,child_mutation,seed=args;construct=entry==0x98dfd0;plain=entry==PLAIN
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in ((0x98d000,0x3000),(0x991000,0x1000),(0x99a000,0x1000),(0x99b000,0x1000),(0xbfe000,0x1000),(0x143f000,0x1000),(ARENA,0x4000),(GLOBAL,0x1000),(STACK,0x10000),(STOP,0x1000),(API,0x1000)):u.mem_map(a,n)
 for a,b in codes.items():u.mem_write(a,b)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 for a,n in ((ARENA,0x4000),(GLOBAL,0x1000)):u.mem_write(a,bytes((i*37+seed*19)&255 for i in range(n)))
 put(OBJ+8,handle);put(0x143fe30,API)
 expected={a:bytearray(u.mem_read(a,n)) for a,n in ((ARENA,0x4000),(GLOBAL,0x1000))};trace=[]
 def model(a,v):
  base=ARENA if ARENA<=a<ARENA+0x4000 else GLOBAL;struct.pack_into('<I',expected[base],a-base,v&0xffffffff)
 def change(a,v):put(a,v);model(a,v)
 def flags_zero():expected[ARENA][OBJ+0x18-ARENA:OBJ+0x1a-ARENA]=b'\0\0'
 def verify():assert all(bytes(u.mem_read(a,len(b)))==bytes(b) for a,b in expected.items())
 if not construct:model(OBJ,0x129a158)
 def hook(uc,a,size,_):
  if a not in (0x99a2f0,0x99aed0,0x98e7d0,API,0x99b510,0x99a300,0xbfe9bc):return
  esp=uc.reg_read(UC_X86_REG_ESP);ecx=uc.reg_read(UC_X86_REG_ECX);pop=0;result=0xbad00000
  if a==0x99a2f0:
   assert construct and not trace and ecx==OBJ;verify();trace.append(('base_construct',));change(OBJ,0x129a7c4)
   if mutation:change(OBJ+4,0x13572468);change(OBJ+0x10,0x87654321)
   model(OBJ,0x129a158);result=OBJ
  elif a==0x99aed0:
   assert construct and trace==[('base_construct',)] and ecx==OBJ+0xc;verify();trace.append(('wide_construct',));change(OBJ+0xc,0)
   if child_mutation:change(OBJ+8,0xcccccccc);change(OBJ+0x14,0x55555555)
   model(OBJ+8,0xffffffff);flags_zero();result=OBJ+0xc
  elif a==0x98e7d0:
   assert not construct and handle!=0xffffffff and not trace and ecx==0x13bc870 and get(esp+4)==OBJ
   verify();trace.append(('close_streams',));pop=4
   if mutation:change(OBJ+8,(0x76543210,0xffffffff)[mutation-1]);change(OBJ+0x10,0xabcdef10)
  elif a==API:
   assert trace==[('close_streams',)] and get(esp+4)==get(OBJ+8);verify();trace.append(('close_handle',get(esp+4)));pop=4;result=int(not close_fail)
   if child_mutation:change(OBJ+8,0xeeeeeeee);change(OBJ+0x14,0xabcdef14)
   model(OBJ+8,0xffffffff);flags_zero()
  elif a==0x99b510:
   assert not construct and ecx==OBJ+0xc and len(trace)==(0 if handle==0xffffffff else 2);verify();trace.append(('wide_destroy',));change(OBJ+0xc,0)
  elif a==0x99a300:
   assert not construct and ecx==OBJ and trace[-1]==('wide_destroy',);verify();trace.append(('base_destroy',));change(OBJ,0x129a7c4)
   if child_mutation:change(OBJ+4,0xabcdef04)
  else:
   assert not construct and not plain and flags&1 and get(esp+4)==OBJ and trace[-1]==('base_destroy',);verify();trace.append(('delete',));pop=0
   u.mem_write(OBJ,b'\xdd'*28);expected[ARENA][OBJ-ARENA:OBJ-ARENA+28]=b'\xdd'*28
  uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_ECX,0xdeadbeef);uc.reg_write(UC_X86_REG_EDX,0x87654321)
  uc.reg_write(UC_X86_REG_EIP,get(esp));uc.reg_write(UC_X86_REG_ESP,esp+4+pop)
 u.hook_add(UC_HOOK_CODE,hook)
 sp=STACK+0x8000;put(sp,STOP);put(sp+4,flags);saved=(UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP)
 for i,reg in enumerate(saved):u.reg_write(reg,0x55660000+seed+i)
 u.reg_write(UC_X86_REG_ECX,OBJ);u.reg_write(UC_X86_REG_ESP,sp)
 u.emu_start(entry,STOP,count=200)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+(4 if construct or plain else 8)
 assert all(u.reg_read(reg)==0x55660000+seed+i for i,reg in enumerate(saved));verify()
 if not plain:assert u.reg_read(UC_X86_REG_EAX)==OBJ
 wanted=[('base_construct',),('wide_construct',)] if construct else ([('close_streams',),trace[1]] if handle!=0xffffffff else [])+[('wide_destroy',),('base_destroy',)]+([('delete',)] if not plain and flags&1 else [])
 assert trace==wanted
 return trace,{a:bytes(b) for a,b in expected.items()}

def compare_lifetime(source=SOURCE):
 inputs={Path(__file__),EXE,R/'tools/decomp_pipeline/check_one.py',R/'tools/decomp_pipeline/near_match.py',R/'tools/decomp_pipeline/native_exact_roots.py',R/'tools/decomp_pipeline/gate_game_component_abi.py',R/'tools/decomp_pipeline/pe_oracle.py'}
 def closure(p):
  if p in inputs:return
  inputs.add(p)
  for n in re.findall(r'^\s*#include\s*"([^"]+)"',p.read_text(encoding='utf-8'),re.M):closure(next(q for q in (p.parent/n,R/'rebuild/include'/n) if q.is_file()))
 for p in (CTOR,source):closure(p)
 pins={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs}
 with tempfile.TemporaryDirectory(prefix='threaded-file-lifetime-') as tmp:built,grades=bodies(Path(tmp),source)
 original={a:read_virtual_data(EXE.read_bytes(),a,n) for a,n in ((0x98dfd0,41),(0x991450,88),(PLAIN,67))};cases=0;plain_cases=0
 for mutation,child,seed in itertools.product(range(2),range(2),(0,7)):
  args=(0,0,0,mutation,child,seed);assert execute(built,0x98dfd0,args)==execute(original,0x98dfd0,args);cases+=1
 for args in itertools.product((0xffffffff,0,0x12345678),(0,1,2,3,0x100,0xffffffff),range(2),range(3),range(2),(0,7)):
  assert execute(built,0x991450,args)==execute(original,0x991450,args);cases+=1
  if args[1]==0:
   assert execute(built,PLAIN,args)==execute(original,PLAIN,args)==execute(original,0x991450,args);plain_cases+=1;cases+=1
 for entry in original:
  for bad in (b'\xcc',b'\x8b\xc1\xc3' if entry==0x98dfd0 else b'\x8b\xc1\xc2\x04\x00'):
   detected=False
   try:execute({**built,entry:bad},entry,(0,1,0,0,0,0))
   except Exception:detected=True
   assert detected
 assert all(hashlib.sha256(Path(p).read_bytes()).hexdigest()==h for p,h in pins.items())
 return dict(status='PASS',cases=cases,ordinary_destructor_cases=plain_cases,roots=grades,missing_provider_checks=3,root_controls=6,input_sha256=pins,
  boundaries=['Existing constructor and both new destructor entries exact; ordinary destructor also compared to original deleting flag-zero entry.',
  'Manager CloseAllStreams, CloseHandle, wide/base lifetime and CRT delete controlled; full20KiB storage, call order, live handle reload and thiscall flags/ABI; no actual file or production owner.'])
