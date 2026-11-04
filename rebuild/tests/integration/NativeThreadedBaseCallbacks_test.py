"""Fresh genuine base-callback canonical provider and independent ABI gate."""
import hashlib,json,re,struct,subprocess,sys,tempfile
from pathlib import Path
R=Path(__file__).resolve().parents[3];T=Path(__file__).resolve().parent;S=Path(tempfile.mkdtemp(prefix='native_base_callbacks_'))
from NativeThreadedBaseCallbacks_model import run as run_model
sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_EBX,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP,UC_X86_REG_ESP,UC_X86_REG_EIP
sources=[R/'rebuild/src/compiled/00/9b/CThreadedFileCallback_OnFileReadBegin_009bdc00.cpp',R/'rebuild/src/compiled/00/9b/CThreadedFileCallback_OnFileReadCompletion_009bdc10.cpp',T/'NativeThreadedBaseCallbacks_required.cpp'];objects=[S/(str(i)+'.obj') for i in range(3)]
inputs={Path(__file__),T/'NativeThreadedBaseCallbacks_model.py',R/'tools/decomp_pipeline/gate_native_threaded_base_callbacks.py',EXE};resolution={}
def closure(p):
 if p in inputs:return
 inputs.add(p)
 for n in re.findall(r'^\s*#\s*include\s*"([^"]+)"',p.read_text(),re.M):
  q=next((q for q in (p.parent/n,R/'rebuild/include'/n) if q.is_file()),None);assert q,(p,n)
  resolution[str(p)+':'+n]=str(q);closure(q)
for p in sources:closure(p);assert not re.search(r'\b(__asm|naked|_emit|__forceinline)\b|#\s*define\s+(private|protected)',p.read_text())
for n in ('check_one.py','gate_game_component_abi.py','pe_oracle.py','near_match.py','crawl/rowtrim.py','crawl/purity.py','variable_allocator_parity.py','verify_and_land.py'):inputs.add(R/'tools/decomp_pipeline'/n)
inputs.add(R/'tools/artifact_layout.py');compiler={V.VC/'bin'/n for n in ('cl.exe','c1xx.dll','c2.dll','link.exe','mspdb71.dll')}
def hashes(ps):return {str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in sorted(ps)}
before=hashes(inputs);vc=hashes(compiler);raw=EXE.read_bytes();logs=[]
assert read_virtual_data(raw,0x9bdc00,5)==bytes.fromhex('b001c20800') and read_virtual_data(raw,0x9bdc10,3)==bytes.fromhex('c20400')
assert struct.unpack('<III',read_virtual_data(raw,0x129b388,12))==(0x9bdc30,0x9bdc00,0x9bdc10)
for p,o in zip(sources,objects):
 result=V.cl(['/nologo','/c','/O2','/MT','/I'+str(R/'rebuild/include'),'/Fo'+str(o),str(p)],V.env(),S);logs.append(dict(source=str(p),returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert result.returncode==0,logs[-1]
objectbefore=hashes(objects);coffs=[Coff(p) for p in objects];cc=coffs[2];md=Cs(CS_ARCH_X86,CS_MODE_32)
roots={'begin':('?OnFileReadBegin@CThreadedFileCallback@@UAE_NPAPAXW4EThreadedFilePriority@@@Z',0x9bdc00,5),'completion':('?OnFileReadCompletion@CThreadedFileCallback@@UAEX_N@Z',0x9bdc10,3)};rootbodies={}
for i,(role,(root,address,size)) in enumerate(roots.items()):
 c=coffs[i];assert root in c.symbols and all(root not in x.symbols for j,x in enumerate(coffs) if i!=j);sec,offset=c.symbols[root];b,r=c.sections[sec];assert offset==0 and not r and b==read_virtual_data(raw,address,size);rootbodies[role]=b.hex()
def inventory(coff):
 rows=[]
 for sec,(b,r) in coff.sections.items():
  h=coff.data[20+40*(sec-1):20+40*sec];flags=struct.unpack_from('<I',h,36)[0];ins=list(md.disasm(b,0)) if flags&0x20 else [];assert not flags&0x20 or sum(i.size for i in ins)==len(b)
  rows.append(dict(section=sec,header_raw=h.hex(),flags=flags,raw_pointer=struct.unpack_from('<I',h,20)[0],raw_size=struct.unpack_from('<I',h,16)[0],bytes=b.hex(),symbols={n:o for n,(s,o) in coff.symbols.items() if s==sec},relocations=[dict(offset=k,symbol=n,kind=t,addend=struct.unpack_from('<I',b,k)[0]) for k,(n,t) in r.items()],code=[dict(offset=i.address,bytes=i.bytes.hex(),op=i.mnemonic,args=i.op_str) for i in ins]))
 assert len(rows)==struct.unpack_from('<H',coff.data,2)[0];return dict(raw_coff=coff.data.hex(),sections=rows)
inventories=[inventory(c) for c in coffs];callerruns=[]
for role,(root,address,size) in roots.items():
 prefix='?RequireBaseBegin@@' if role=='begin' else '?RequireBaseCompletion@@';names=[n for n in cc.symbols if n.startswith(prefix)];assert len(names)==1;sec,offset=cc.symbols[names[0]];b,r=cc.sections[sec];assert offset==0 and len(r)==1
 k,(target,kind)=next(iter(r.items()));assert target==root and kind==20 and struct.unpack_from('<I',b,k)[0]==0 and b[k-1] in (0xe8,0xe9)
 patched=bytearray(b);entry=0x100000;child=0x100400;stop=0x100800;struct.pack_into('<I',patched,k,(child-(entry+k+4))&0xffffffff)
 count=0
 for this,arg1,arg2 in ((0,0,0),(1,0xffffffff,0x80000000),(0x87654321,1,3),(0x200300,0x200400,0xffffffff)):
  u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(entry,0x1000);u.mem_map(0x200000,0x8000);u.mem_write(entry,bytes(patched));u.mem_write(child,bytes.fromhex(rootbodies[role]));sp=0x207000;u.mem_write(sp,struct.pack('<IIII',stop,this,arg1,arg2));u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_EAX,0x12345678);callee={UC_X86_REG_EBX:0x12345678,UC_X86_REG_ESI:0x87654321,UC_X86_REG_EDI:0x81234567,UC_X86_REG_EBP:0xabcdef01}
  for reg,value in callee.items():u.reg_write(reg,value)
  hits=[]
  def hook(machine,pc,size,user):
   if pc!=child:return
   stack=machine.reg_read(UC_X86_REG_ESP);assert machine.reg_read(UC_X86_REG_ECX)==this;actual1=struct.unpack('<I',machine.mem_read(stack+4,4))[0]
   if role=='begin':assert actual1==arg1 and struct.unpack('<I',machine.mem_read(stack+8,4))[0]==arg2
   else:assert actual1&255==arg1&255
   hits.append(pc)
  u.hook_add(UC_HOOK_CODE,hook);u.emu_start(entry,stop,count=100);assert hits==[child] and u.reg_read(UC_X86_REG_EIP)==stop and u.reg_read(UC_X86_REG_ESP)==sp+4 and all(u.reg_read(reg)==v for reg,v in callee.items())
  if role=='begin':assert u.reg_read(UC_X86_REG_EAX)&255==1
  count+=1
 callerruns.append(dict(role=role,caller_symbol=names[0],actual_caller_cases=count,qualified_direct_root=True))
def link(objs,name):return subprocess.run([str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(S/name),*[str(p) for p in objs]],cwd=str(S),env=V.env(),capture_output=True,text=True)
omissions=[]
for i,(role,(root,address,size)) in enumerate(roots.items()):
 missing=link([p for j,p in enumerate(objects) if j!=i],'base_callback_missing_'+role+'.exe');diag=missing.stdout+missing.stderr;assert missing.returncode!=0 and any(root in line and re.search('LNK(?:2001|2019)',line) for line in diag.splitlines());omissions.append(dict(role=role,missing_diagnostic=diag))
control=link(objects,'base_callback_control.exe');assert control.returncode==0,control.stdout+control.stderr
linked_before=hashes(set(objects)|{S/'base_callback_control.exe'})
original_run=run_model({'begin':read_virtual_data(raw,0x9bdc00,5),'completion':read_virtual_data(raw,0x9bdc10,3)},'original')
compiled_run=run_model({k:bytes.fromhex(v) for k,v in rootbodies.items()},'compiled')
for run in (original_run,compiled_run):assert [r['cases'] for r in run['runs']]==[768,384]
assert linked_before==hashes(set(objects)|{S/'base_callback_control.exe'})
assert before==hashes(inputs) and vc==hashes(compiler) and objectbefore==hashes(objects)
report=dict(original_run=original_run,compiled_run=compiled_run,linked_before=linked_before,linked_after=hashes(set(objects)|{S/'base_callback_control.exe'}),status='NATIVE_BASE_CALLBACKS_PASS',grade='MATCH',root_bodies=rootbodies,caller_runs=callerruns,inventory=inventories,compile_logs=logs,omissions=omissions,control_diagnostic=control.stdout+control.stderr,input_before=before,input_after=hashes(inputs),compiler_before=vc,compiler_after=hashes(compiler),objects_before=objectbefore,objects_after=hashes(objects),quoted_resolution=resolution,control_exe_sha256=hashlib.sha256((S/'base_callback_control.exe').read_bytes()).hexdigest(),limits='Actual caller machines invoke actual compiled roots. Link EXEs never executed. Base-table identity correction only, no source promotion/count or actual scheduler ownership.')
out=R/'rebuild/compile-gate/native-threaded-base-callbacks.json';out.parent.mkdir(parents=True,exist_ok=True);out.write_text(json.dumps(report,indent=2)+'\n');print(report['status'],768,384)
