"""Fresh canonical Close compilation and independent differential gate."""
import hashlib,json,re,struct,subprocess,sys,tempfile
from pathlib import Path
R=Path(__file__).resolve().parents[3];T=Path(__file__).resolve().parent;S=Path(tempfile.mkdtemp(prefix='native_threaded_close_'))
from NativeThreadedClose_model import run as run_model
sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
from near_match import compare,retail_relocations
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
sources=[R/'rebuild/src/compiled/00/98/CThreadedFile_Close_0098ec40.cpp',T/'NativeThreadedClose_required.cpp',T/'NativeThreadedClose_unused_link.cpp'];objects=[S/(str(i)+'.obj') for i in range(3)]
inputs={Path(__file__),EXE,T/'NativeThreadedClose_model.py',R/'tools/decomp_pipeline/gate_native_threaded_file_close.py'};resolution={}
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
import pefile
imports={x.address:x.name.decode() for d in pefile.PE(data=raw).DIRECTORY_ENTRY_IMPORT for x in d.imports if x.name};assert imports[0x143fe30]=='CloseHandle'
for p,o in zip(sources,objects):
 result=V.cl(['/nologo','/c','/O2','/MT','/I'+str(R/'rebuild/include'),'/Fo'+str(o),str(p)],V.env(),S);logs.append(dict(source=str(p),returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert result.returncode==0,logs[-1]
objectbefore=hashes(objects);coffs=[Coff(p) for p in objects];c,cc,bc=coffs;root='?Close@CThreadedFile@@QAEXXZ';assert root in c.symbols and root not in cc.symbols and root not in bc.symbols
sec,offset=c.symbols[root];body,rels=c.sections[sec];assert offset==0
targets={'?CloseAllStreamsForFile@CThreadedFileManager@@QAEXPAVCThreadedFile@@@Z':(20,0x98e7d0),'?g_FableThreadedFileManager_013BC870@@3VCThreadedFileManager@@A':(6,0x13bc870),'__imp__CloseHandle@4':(6,0x143fe30)}
assert {n for n,t in rels.values()}==set(targets) and len(rels)==3
patched=bytearray(body);audit=[]
for k,(n,t) in rels.items():
 kind,va=targets[n];assert t==kind and struct.unpack_from('<I',body,k)[0]==0
 struct.pack_into('<I',patched,k,(va-(0x98ec40+k+4) if t==20 else va)&0xffffffff);audit.append(dict(offset=k,symbol=n,kind=t,target=hex(va),addend=0))
original=read_virtual_data(raw,0x98ec40,47);score=compare(body,original,tuple(rels),retail_relocations('0098ec40',47)).to_dict();grade='RELOCATION_MATCH' if patched==original else 'NEAR_MATCH' if score['near_match'] else 'FAIL'
md=Cs(CS_ARCH_X86,CS_MODE_32)
def inventory(coff):
 rows=[]
 for section,(b,r) in coff.sections.items():
  h=coff.data[20+40*(section-1):20+40*section];flags=struct.unpack_from('<I',h,36)[0];ins=list(md.disasm(b,0)) if flags&0x20 else [];assert not flags&0x20 or sum(i.size for i in ins)==len(b)
  rows.append(dict(section=section,header=h.hex(),flags=flags,raw=b.hex(),symbols={n:o for n,(sec,o) in coff.symbols.items() if sec==section},relocations=[dict(offset=k,symbol=n,kind=t,addend=struct.unpack_from('<I',b,k)[0]) for k,(n,t) in r.items()],code=[dict(offset=i.address,bytes=i.bytes.hex(),op=i.mnemonic,args=i.op_str) for i in ins]))
 assert len(rows)==struct.unpack_from('<H',coff.data,2)[0];return dict(raw_coff=coff.data.hex(),sections=rows)
inventories=[inventory(x) for x in coffs];requiredrels=[(k,n,t) for b,r in cc.sections.values() for k,(n,t) in r.items() if n==root];assert len(requiredrels)==1 and requiredrels[0][2]==20
caller_section,caller_offset=cc.symbols['?RequireThreadedClose@@YAXPAVCThreadedFile@@@Z'];cb,cr=cc.sections[caller_section];ci=list(md.disasm(cb,0))
assert caller_offset==0 and len(ci)==2 and ci[0].mnemonic=='mov' and ci[0].op_str=='ecx, dword ptr [esp + 4]' and ci[1].mnemonic=='jmp' and cr=={5:(root,20)},'Actual caller must forward this via ECX, zero explicit member arguments'
alias='/alternatename:?g_FableThreadedFileManager_013BC870@@3VCThreadedFileManager@@A=_UnusedManagerStorage'
def link(objs,name):return subprocess.run([str(V.VC/'bin/link.exe'),'/nologo',alias,'kernel32.lib','/out:'+str(S/name),*[str(p) for p in objs]],cwd=str(S),env=V.env(),capture_output=True,text=True)
missing=link(objects[1:],'threaded_close_missing.exe');diag=missing.stdout+missing.stderr;assert missing.returncode!=0 and any(root in line and re.search('LNK(?:2001|2019)',line) for line in diag.splitlines())
control=link(objects,'threaded_close_control.exe');assert control.returncode==0,control.stdout+control.stderr
assert grade=='RELOCATION_MATCH' and patched==original
linked_before=hashes(set(objects)|{S/'threaded_close_control.exe'})
original_run=run_model(original,'original');compiled_run=run_model(bytes(patched),'compiled');assert original_run['cases']==compiled_run['cases']==144 and original_run['snapshots']==compiled_run['snapshots']==360
assert linked_before==hashes(set(objects)|{S/'threaded_close_control.exe'})
assert before==hashes(inputs) and vc==hashes(compiler) and objectbefore==hashes(objects)
report=dict(original_run=original_run,compiled_run=compiled_run,linked_before=linked_before,linked_after=hashes(set(objects)|{S/'threaded_close_control.exe'}),status='NATIVE_THREADED_CLOSE_PASS',grade=grade,similarity=score,root_symbol=root,patched_body_hex=patched.hex(),relocations=audit,inventory=inventories,compile_logs=logs,missing_diagnostic=diag,control_diagnostic=control.stdout+control.stderr,input_before=before,input_after=hashes(inputs),compiler_before=vc,compiler_after=hashes(compiler),object_before=objectbefore,object_after=hashes(objects),quoted_resolution=resolution,control_exe_sha256=hashlib.sha256((S/'threaded_close_control.exe').read_bytes()).hexdigest(),limits='Fresh canonical unchanged ordinary source. Link EXEs never executed; functional manager/global/Win32 boundaries not recovered, no production global storage or ownership credit.')
out=R/'rebuild/compile-gate/native-threaded-file-close.json';out.parent.mkdir(parents=True,exist_ok=True);out.write_text(json.dumps(report,indent=2)+'\n');print(report['status'],grade,144,360)
