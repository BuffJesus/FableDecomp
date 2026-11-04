"""Guarded first ordinary compile proposal. FULL root review before execution."""
import hashlib,importlib.util,itertools,json,re,struct,subprocess,sys,tempfile,traceback
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
R=Path(__file__).resolve().parents[3];T=Path(__file__).parent;W=Path(tempfile.mkdtemp(prefix='native-threaded-file-wait-'))
REPORT=R/'rebuild/compile-gate/native-threaded-file-wait.json';AUDIT={}
REPORT.parent.mkdir(parents=True,exist_ok=True)
sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
from near_match import compare,retail_relocations
ROOT='?WaitForObjectToFinish@CThreadedFile@@QAEXK@Z'
CHILD='?WaitForObjectToFinish@CThreadedFileManager@@QAEXK@Z'
GLOBAL='?g_FableThreadedFileManager_013BC870@@3VCThreadedFileManager@@A'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def load(p):
 spec=importlib.util.spec_from_file_location('independent_threaded_wait_behavior',p);m=importlib.util.module_from_spec(spec);spec.loader.exec_module(m);return m
def inventory(path):
 data=path.read_bytes();c=Coff(path);machine,count,stamp,symptr,symcount,optional,flags=struct.unpack_from('<HHIIIHH',data);assert machine==0x14c and optional==0;rows=[]
 for sec in range(1,count+1):
  at=20+(sec-1)*40;name=data[at:at+8].rstrip(b'\0').decode();vs,va,size,ptr,rp,lp,nr,nl,fl=struct.unpack_from('<IIIIIIHHI',data,at+8)
  payload=data[ptr:ptr+size] if ptr else b'';assert c.sections[sec][0]==payload
  decoded=list(Cs(CS_ARCH_X86,CS_MODE_32).disasm(payload,0)) if fl&0x20 else []
  assert not fl&0x20 or sum(i.size for i in decoded)==len(payload)
  rows.append(dict(section=sec,name=name,virtual_size=vs,raw_size=size,raw_pointer=ptr,flags=hex(fl),initialized_bytes=len(payload),payload_sha256=hashlib.sha256(payload).hexdigest(),bss_extent=size if not ptr and fl&0x80 else 0,symbols=[(n,o) for n,(s,o) in c.symbols.items() if s==sec],fixups=[dict(site=k,target=n,kind=t,raw_addend_hex=payload[k:k+4].hex() if k+4<=len(payload) else None) for k,(n,t) in c.sections[sec][1].items()]))
 for row in rows:
  section=row['section'];payload=c.sections[section][0]
  row['payload_hex']=payload.hex()
  row['code']=[(i.address,i.bytes.hex(),i.mnemonic,i.op_str) for i in Cs(CS_ARCH_X86,CS_MODE_32).disasm(payload,0)] if int(row['flags'],16)&0x20 else []
 return rows
def main():
 sources=[R/'rebuild/src/compiled/00/98/CThreadedFile_WaitForObjectToFinish_0098f370.cpp',T/'NativeThreadedFileWait_required.cpp',T/'NativeThreadedFileWait_unused_link.cpp'];objects=[W/'root.obj',W/'required.obj',W/'unused_link.obj']
 assert not any(p.exists() for p in objects),'First compile only; never overwrite first objects'
 modelp=T/'NativeThreadedFileWait_model.py'
 inputs={Path(__file__),modelp,EXE,*sources,R/'tools/decomp_pipeline/gate_native_threaded_file_wait.py'}
 seen=set()
 def headers(p):
  if p in seen:return
  seen.add(p);inputs.add(p)
  for n in re.findall(r'^\s*#\s*include\s*"([^"]+)"',p.read_text(),re.M):
   q=next((q for q in (p.parent/n,R/'rebuild/include'/n) if q.is_file()),None);assert q;headers(q)
 for p in sources:headers(p)
 for p in sources:
  assert not re.search(r'\b(__asm|naked|_emit|__forceinline)\b|#\s*define\s+(private|protected)',p.read_text())
 for n in ('check_one.py','gate_game_component_abi.py','pe_oracle.py','near_match.py','verify_and_land.py','variable_allocator_parity.py','crawl/rowtrim.py','crawl/purity.py'):inputs.add(R/'tools/decomp_pipeline'/n)
 inputs.add(R/'tools/artifact_layout.py')
 include_roots=[Path(n) for n in V.env()['INCLUDE'].split(';') if n]
 assert V.VC/'include' in include_roots and all(p.is_dir() for p in include_roots)
 for sdk in include_roots:inputs.update(p for p in sdk.rglob('*') if p.is_file())
 library_roots=[Path(n) for n in V.env()['LIB'].split(';') if n]
 assert library_roots and all(p.is_dir() for p in library_roots)
 linked_libraries={}
 for name in ('kernel32.lib','libcmt.lib','oldnames.lib'):
  candidates=[p/name for p in library_roots if (p/name).is_file()]
  assert candidates,name
  linked_libraries[name]=candidates[0];inputs.add(candidates[0])
 def pins():return {str(p):sha(p) for p in sorted(inputs)}
 def tc():return {str(p):sha(p) for p in (V.VC/'bin'/n for n in ('cl.exe','c1xx.dll','c2.dll','link.exe','mspdb71.dll'))}
 before=pins();toolbefore=tc();AUDIT.update(inputs=inputs,pins=pins,tc=tc,input_before=before,toolchain_before=toolbefore,objects=objects)
 (W/'before.json').write_text(json.dumps(dict(kind='FRESH_CANONICAL_BEFORE_COMPILE',input_sha256=before,toolchain_sha256=toolbefore),indent=2)+'\n')
 (REPORT.with_suffix('.before.json')).write_bytes((W/'before.json').read_bytes())
 raw=EXE.read_bytes();native=read_virtual_data(raw,0x98f370,10);assert native==bytes.fromhex('b970c83b01e966feffff')
 assert read_virtual_data(raw,0x98f37a,6)==b'\xcc'*6
 compile_commands=[]
 for src,out in zip(sources,objects):
  command=['/nologo','/c','/O2','/MT','/I'+str(R/'rebuild/include'),'/Fo'+str(out),str(src)]
  compile_commands.append([str(V.VC/'bin/cl.exe'),*command])
  r=V.cl(command,V.env(),W);out.with_suffix('.compile.log').write_text(r.stdout+r.stderr);assert r.returncode==0,r.stdout+r.stderr
 objectbefore={str(p):sha(p) for p in objects};AUDIT['object_before']=objectbefore
 (W/'objects_after_compile.json').write_text(json.dumps(objectbefore,indent=2)+'\n');inventories={str(p):inventory(p) for p in objects}
 c=Coff(objects[0]);sec,offset=c.symbols[ROOT];assert offset==0;b,rels=c.sections[sec]
 assert rels=={1:(GLOBAL,6),6:(CHILD,20)};patched=bytearray(b)
 for site,(target,kind) in rels.items():
  assert struct.unpack_from('<I',b,site)[0]==0
  address=0x13bc870 if target==GLOBAL else 0x98f1e0
  struct.pack_into('<I',patched,site,(address if kind==6 else address-0x98f370-site-4)&0xffffffff)
 grade=compare(b,native,tuple(rels),retail_relocations('0098f370',10)).to_dict()
 (W/'first_root_parity.json').write_text(json.dumps(dict(bytes=len(b),grade=grade,relocated_exact=bytes(patched)==native,inventory=inventories),indent=2)+'\n')
 assert bytes(patched)==native,'First candidate parity failure retained; no tuning'
 ins=list(Cs(CS_ARCH_X86,CS_MODE_32).disasm(patched,0x98f370));assert len(ins)==2 and ins[0].mnemonic=='mov' and ins[0].op_str=='ecx, 0x13bc870' and ins[1].mnemonic=='jmp' and ins[1].op_str=='0x98f1e0'
 caller=Coff(objects[1]);s,o=caller.symbols['_main'];assert o==0;cb,cr=caller.sections[s];assert len(cr)==1
 site,(target,kind)=next(iter(cr.items()));assert (target,kind)==(ROOT,20) and struct.unpack_from('<I',cb,site)[0]==0
 callerins=list(Cs(CS_ARCH_X86,CS_MODE_32).disasm(cb,0));assert any(i.mnemonic=='call' and i.address+1==site for i in callerins)
 callindex=next(i for i,x in enumerate(callerins) if x.address+1==site)
 assert not any(x.mnemonic=='add' and x.op_str.startswith('esp,') for x in callerins[callindex+1:]),'Root callee RET4; no caller argument cleanup'
 # Actual qualified typed caller proves ECXargv/raw UIDargc, calleeRET4; defined main returns zero, not child EAX.
 BASE=0x5000000;BOUND=BASE+0x1000;STACK=0x6000000;STOP=0x7000000;callerbody=bytearray(cb);struct.pack_into('<I',callerbody,site,(BOUND-BASE-site-4)&0xffffffff);caller_cases=0
 for receiver,uid,result in itertools.product((0,1,0x80000000,0xffffffff),(0,1,0x7fffffff,0x80000000,0xfffffffe,0xffffffff),(0,0xfedcba98)):
  u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(BASE,8192);u.mem_map(STACK,4096);u.mem_map(STOP,4096);u.mem_write(BASE,bytes(callerbody));sp=STACK+0x800;u.mem_write(sp,struct.pack('<III',STOP,uid,receiver));u.reg_write(UC_X86_REG_ESP,sp)
  regs=(UC_X86_REG_EBX,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI)
  for i,r in enumerate(regs):u.reg_write(r,0x12340000+i)
  hit=[0]
  def hook(uc,pc,size,data):
   if pc==STOP:uc.emu_stop();return
   if pc!=BOUND:return
   st=uc.reg_read(UC_X86_REG_ESP);ret,actual_uid=struct.unpack('<II',uc.mem_read(st,8));assert uc.reg_read(UC_X86_REG_ECX)==receiver and actual_uid==uid and ret==BASE+site+4;hit[0]+=1
   uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_ECX,0xabcdef01);uc.reg_write(UC_X86_REG_EDX,0xabcdef02);uc.reg_write(UC_X86_REG_ESP,st+8);uc.reg_write(UC_X86_REG_EIP,ret)
  u.hook_add(UC_HOOK_CODE,hook);u.emu_start(BASE,STOP,count=200)
  assert hit==[1] and u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+4 and u.reg_read(UC_X86_REG_EAX)==0
  assert [u.reg_read(r) for r in regs]==[0x12340000+i for i in range(4)];caller_cases+=1
 # Test-only singleton remains trivial BSS, no synthetic dynamic constructor.
 unused=Coff(objects[2]);assert not any(n.startswith('??0CThreadedFileManager@') for n in unused.indices.values())
 assert not any(s['name']=='.CRT$XCU' for s in inventories[str(objects[2])])
 command=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(W/'never_run.exe'),'kernel32.lib',*[str(p) for p in objects[1:]]]
 r=subprocess.run(command,env=V.env(),capture_output=True,text=True);missing=r.stdout+r.stderr;(W/'root-omission.link.log').write_text(missing)
 assert r.returncode!=0 and any(ROOT in line and ('LNK2001' in line or 'LNK2019' in line) for line in missing.splitlines())
 r=subprocess.run(command+[str(objects[0])],env=V.env(),capture_output=True,text=True);(W/'positive-control.link.log').write_text(r.stdout+r.stderr);assert r.returncode==0,r.stdout+r.stderr
 linked_before={str(p):sha(p) for p in objects+[W/'never_run.exe']};AUDIT['linked_before']=linked_before
 (W/'linked_before.json').write_text(json.dumps(linked_before,indent=2)+'\n')
 f=load(modelp);modes={'original':f.check(native),'compiled':f.check(bytes(patched))}
 assert modes['original']==modes['compiled']==dict(cases=144,checkpoints=288,root_hits=144,child_hits=144)
 assert linked_before=={str(p):sha(p) for p in objects+[W/'never_run.exe']}
 assert pins()==before,'Fresh canonical input drift; full after diagnostics persisted in finally'
 assert tc()==toolbefore,'Five-toolchain drift; diagnostics in finally'
 assert {str(p):sha(p) for p in objects}==objectbefore,'Object drift; diagnostics in finally'
 report=dict(status='NATIVE_THREADED_FILE_WAIT_PASS',root_symbol=ROOT,original_body_sha256=hashlib.sha256(native).hexdigest(),compiled_body_sha256=hashlib.sha256(patched).hexdigest(),executable_cache_policy='Separate immutable UCs with code installed before translation.',bytes=len(b),instructions=len(ins),grade=grade,relocated_exact=True,fixups={str(k):(n,t,0) for k,(n,t) in rels.items()},caller_abi_cases=caller_cases,caller_disassembly=[(i.mnemonic,i.op_str) for i in callerins],cases=144,checkpoints_per_mode=288,mode_execution=modes,source_omission_diagnostic=missing,provider_link_control_passed=True,inventory=inventories,input_sha256_before=before,input_sha256_after=pins(),toolchain_sha256_before=toolbefore,toolchain_sha256_after=tc(),object_sha256_before=objectbefore,object_sha256_after={str(p):sha(p) for p in objects},link_executable_executed=False,boundary='First ordinary genuine Wait wrapper: separate immutable original/compiled engines, fullarena/global typedmanager callback proof only. Actual181B manager not executed; no scheduling/queue/concurrency/ownership/source credit. Raw child EAX behavior is not defined void return. Link-only typedabort/existingglobal BSS neverrun, no ctor/aliases/new physical production singleton. No historical first-probe provenance reconstructed. Fresh canonical compile and behavior only; identity replacement is not a new recovery.',sdk_include_roots=[str(p) for p in include_roots])
 report.update(raw_coff={str(p):p.read_bytes().hex() for p in objects},linked_before=linked_before,linked_after={str(p):sha(p) for p in objects+[W/'never_run.exe']},compile_commands=compile_commands,omission_link_command=command,positive_link_command=command+[str(objects[0])],resolved_link_libraries={n:str(p) for n,p in linked_libraries.items()})
 REPORT.write_text(json.dumps(report,indent=2)+'\n');print(report['status'],10,2,144,288,caller_cases)
def finish_audit(error):
 if not AUDIT:return
 def current(table):return {n:sha(Path(n)) if Path(n).is_file() else None for n in table}
 after=current(AUDIT['input_before']);tools=current(AUDIT['toolchain_before']);objects=current(AUDIT.get('object_before',{}));linked=current(AUDIT.get('linked_before',{}))
 def drift(before,after):return [dict(path=n,before=h,after=after.get(n)) for n,h in before.items() if after.get(n)!=h]
 diagnostic=dict(status='FAILED' if error else 'TERMINAL',error=error,input_before=AUDIT['input_before'],input_after=after,toolchain_before=AUDIT['toolchain_before'],toolchain_after=tools,object_before=AUDIT.get('object_before',{}),object_after=objects,linked_before=AUDIT.get('linked_before',{}),linked_after=linked,input_drift=drift(AUDIT['input_before'],after),toolchain_drift=drift(AUDIT['toolchain_before'],tools),object_drift=drift(AUDIT.get('object_before',{}),objects),linked_drift=drift(AUDIT.get('linked_before',{}),linked),preserved_artifact_directory=str(W))
 diagnostic['existing_objects_after_even_partial_compile']={str(p):sha(p) for p in AUDIT['objects'] if p.is_file()}
 diagnostic['compile_and_link_logs']={str(p):p.read_text() for p in W.glob('*.log')}
 (W/'after.json').write_text(json.dumps(diagnostic,indent=2)+'\n');REPORT.with_suffix('.after.json').write_bytes((W/'after.json').read_bytes())
if __name__=='__main__':
 error=None
 try:main()
 except BaseException:
  error=traceback.format_exc();(W/'failure.log').write_text(error)
  REPORT.write_text(json.dumps(dict(status='NATIVE_THREADED_FILE_WAIT_FAILED',error=error,preserved_artifact_directory=str(W)),indent=2)+'\n')
  raise
 finally:finish_audit(error)
