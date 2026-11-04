"""Fresh canonical corrected-wrapper gate, exact bytes and bounded behavior."""
import hashlib,importlib.util,json,re,struct,subprocess,sys,tempfile,traceback
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
R=Path(__file__).resolve().parents[3];W=Path(__file__).parent;D=Path(tempfile.mkdtemp(prefix='threaded-finish-handle-'));sys.path.insert(0,str(R/'tools/decomp_pipeline'))
def preserve_failure(exc_type,exc_value,exc_tb):
 (D/'failure.log').write_text(''.join(traceback.format_exception(exc_type,exc_value,exc_tb)))
 sys.__excepthook__(exc_type,exc_value,exc_tb)
sys.excepthook=preserve_failure
import check_one as V
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from gate_game_component_abi import Coff
from near_match import compare,retail_relocations
from pe_oracle import EXE,read_virtual_data
source=R/'rebuild/src/compiled/00/98/CThreadedFile_FinishAndGetFileHandle_0098ec70.cpp';required=W/'NativeThreadedFileFinishHandle_required.cpp';unused=W/'NativeThreadedFileFinishHandle_unused_link.cpp';fixture=W/'NativeThreadedFileFinishHandle_model.py';obj=D/'root.obj';receipt=R/'rebuild/compile-gate/native-threaded-file-finish-handle.json'
sources=[source,required,unused];outputs=[obj,D/'required.obj',D/'unused_link.obj']
ROOT='?FinishAndGetFileHandle@CThreadedFile@@QAEPAXXZ';CHILD='?CloseAllStreamsForFile@CThreadedFileManager@@QAEXPAVCThreadedFile@@@Z';GLOBAL='?g_FableThreadedFileManager_013BC870@@3VCThreadedFileManager@@A'
inputs={Path(__file__),fixture,R/'tools/decomp_pipeline/gate_native_threaded_file_finish_handle.py'};seen=set()
def headers(p):
 if p in seen:return
 seen.add(p);inputs.add(p)
 for n in re.findall(r'^\s*#\s*include\s*"([^"]+)"',p.read_text(),re.M):
  q=next((q for q in (p.parent/n,R/'rebuild/include'/n) if q.is_file()),None);assert q;headers(q)
for p in sources:headers(p)
for n in ('check_one.py','gate_game_component_abi.py','pe_oracle.py','near_match.py','verify_and_land.py','variable_allocator_parity.py','crawl/rowtrim.py','crawl/purity.py'):inputs.add(R/'tools/decomp_pipeline'/n)
inputs.add(R/'tools/artifact_layout.py')
def hashes():return {str(p.relative_to(R)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sorted(inputs)}
def tools():return {str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in (V.VC/'bin'/n for n in ('cl.exe','c1xx.dll','c2.dll','link.exe','mspdb71.dll'))}
before=hashes();tc=tools();raw=EXE.read_bytes();pehash=hashlib.sha256(raw).hexdigest();flags=['/nologo','/c','/O2','/MT','/I'+str(R/'rebuild/include')]
for p,out in zip(sources,outputs):
 r=V.cl(flags+['/Fo'+str(out),str(p)],V.env(),D)
 out.with_suffix('.compile.log').write_text(r.stdout+r.stderr)
 assert r.returncode==0,r.stdout+r.stderr
object_before={str(out):hashlib.sha256(out.read_bytes()).hexdigest() for out in outputs}
c=Coff(obj);s,o=c.symbols[ROOT];assert o==0;b,rels=c.sections[s];assert rels=={11:(GLOBAL,6),16:(CHILD,20)};patched=bytearray(b)
assert len(b)==25
for site,(name,kind) in rels.items():
 assert struct.unpack_from('<I',b,site)[0]==0;target=0x13bc870 if name==GLOBAL else 0x98e7d0;struct.pack_into('<I',patched,site,(target-0x98ec70-site-4 if kind==20 else target)&0xffffffff)
native=read_virtual_data(raw,0x98ec70,25);grade=compare(b,native,tuple(rels),retail_relocations('0098ec70',25)).to_dict();(D/'first_root_parity_audit.json').write_text(json.dumps(dict(bytes=len(b),relocated_exact=bytes(patched)==native,grade=grade),indent=2)+'\n');assert bytes(patched)==native;ins=list(Cs(CS_ARCH_X86,CS_MODE_32).disasm(patched,0x98ec70));assert [(i.mnemonic,i.op_str) for i in ins]==[('push','esi'),('mov','esi, ecx'),('cmp','dword ptr [esi + 8], -1'),('je','0x98ec84'),('push','esi'),('mov','ecx, 0x13bc870'),('call','0x98e7d0'),('mov','eax, dword ptr [esi + 8]'),('pop','esi'),('ret','')]
caller=Coff(outputs[1]);section,start=caller.symbols['_main'];assert start==0
cb,cr=caller.sections[section];assert len(cr)==1;site,(name,kind)=next(iter(cr.items()));assert name==ROOT and kind==20 and struct.unpack_from('<I',cb,site)[0]==0
caller_ins=list(Cs(CS_ARCH_X86,CS_MODE_32).disasm(cb,0));assert any(x.address+1==site and x.mnemonic=='call' for x in caller_ins)
call_index=next(i for i,x in enumerate(caller_ins) if x.address+1==site)
assert not any(x.mnemonic=='push' or (x.mnemonic in ('sub','add') and x.op_str.startswith('esp,')) for x in caller_ins[:call_index]),'Zero-argument main caller must not introduce argument/storage pushes'
assert not any(x.mnemonic=='add' and x.op_str.startswith('esp,') for x in caller_ins[call_index+1:]),'No explicit root argument cleanup'
CB=0x5000000;BOUNDARY=0x5001000;CS=0x6000000;STOP=0x7000000;caller_body=bytearray(cb);struct.pack_into('<I',caller_body,site,(BOUNDARY-CB-site-4)&0xffffffff)
caller_cases=0
for receiver in (0,1,0x7fffffff,0x80000000,0xffffffff):
 for result in (0,1,0x7fffffff,0x80000000,0xffffffff):
  uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(CB,8192);uc.mem_map(CS,4096);uc.mem_map(STOP,4096);uc.mem_write(CB,bytes(caller_body));sp=CS+0x800;uc.mem_write(sp,struct.pack('<III',STOP,0x87654321,receiver));uc.reg_write(UC_X86_REG_ESP,sp);seen_call=[0];entry_regs=(UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP)
  for i,reg in enumerate(entry_regs):uc.reg_write(reg,0x11223300+i)
  def caller_hook(u,pc,n,_):
   if pc==STOP:u.emu_stop();return
   if pc!=BOUNDARY:return
   stack=u.reg_read(UC_X86_REG_ESP);ret=struct.unpack('<I',u.mem_read(stack,4))[0];assert stack==sp-4 and ret==CB+site+4 and u.reg_read(UC_X86_REG_ECX)==receiver;seen_call[0]+=1
   # Actual root has zero explicit args/RET0: pop return only. Subsequent main
   # execution/ESP/register checks reject invented argument cleanup.
   u.reg_write(UC_X86_REG_EAX,result);u.reg_write(UC_X86_REG_ECX,0xabcdef01);u.reg_write(UC_X86_REG_EDX,0xabcdef02);u.reg_write(UC_X86_REG_ESP,stack+4);u.reg_write(UC_X86_REG_EIP,ret)
  uc.hook_add(UC_HOOK_CODE,caller_hook);uc.emu_start(CB,STOP,count=100)
  assert seen_call[0]==1 and uc.reg_read(UC_X86_REG_EIP)==STOP and uc.reg_read(UC_X86_REG_ESP)==sp+4 and uc.reg_read(UC_X86_REG_EAX)==int(result!=0)
  assert [uc.reg_read(reg) for reg in entry_regs]==[0x11223300+i for i in range(4)];caller_cases+=1
calls=[(section,site)]
dependency_inventory=[]
for index,path in enumerate(outputs):
 co=Coff(path);data=path.read_bytes();machine,count,stamp,symptr,symcount,optional,flags=struct.unpack_from('<HHIIIHH',data)
 assert machine==0x14c and optional==0
 if path==outputs[2]:
  assert not any(name.startswith('??0CThreadedFileManager@') for name in co.indices.values())
 for section in range(1,count+1):
  at=20+(section-1)*40;sectionname=data[at:at+8].rstrip(b'\0').decode();virtual_size,virtual_address,raw_size,raw_pointer,rel_pointer,line_pointer,rel_count,line_count,characteristics=struct.unpack_from('<IIIIIIHHI',data,at+8)
  uninitialized=bool(characteristics&0x80);payload=data[raw_pointer:raw_pointer+raw_size] if raw_pointer else b''
  if path==outputs[2]:assert sectionname!='.CRT$XCU','Unused singleton must not introduce dynamic initialization'
  assert not raw_pointer or len(payload)==raw_size;fixups=[]
  rr=co.sections[section][1];assert co.sections[section][0]==payload
  for site,(target,kind) in rr.items():
   # Debug relocations have their own kinds/addends; inventory, don't impose
   # executable REL32 zero-addend assumptions on data/debug/BSS sections.
   addend=payload[site:site+4].hex() if site+4<=len(payload) else None
   fixups.append(dict(site=site,symbol=target,kind=kind,raw_addend_hex=addend))
  dependency_inventory.append(dict(object_index=index,section=section,section_name=sectionname,characteristics=hex(characteristics),virtual_size=virtual_size,raw_size=raw_size,raw_pointer=raw_pointer,initialized_payload_bytes=len(payload),payload_sha256=hashlib.sha256(payload).hexdigest() if payload else None,uninitialized=uninitialized,bss_extent=raw_size if uninitialized and raw_pointer==0 else 0,symbols=[dict(name=name,offset=offset) for name,(sec,offset) in co.symbols.items() if sec==section],fixups=fixups))
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(D/'required.exe'),'kernel32.lib',*[str(p) for p in outputs[1:]]]
r=subprocess.run(cmd,env=V.env(),capture_output=True,text=True);diagnostic=r.stdout+r.stderr;(D/'root-omission.link.log').write_text(diagnostic);assert r.returncode and any(ROOT in line and ('LNK2001' in line or 'LNK2019' in line) for line in diagnostic.splitlines())
r=subprocess.run(cmd+[str(obj)],env=V.env(),capture_output=True,text=True);(D/'positive-control.link.log').write_text(r.stdout+r.stderr);assert r.returncode==0,r.stdout+r.stderr
assert hashes()==before and tools()==tc and hashlib.sha256(EXE.read_bytes()).hexdigest()==pehash
spec=importlib.util.spec_from_file_location('threaded_finish_model',fixture);f=importlib.util.module_from_spec(spec);spec.loader.exec_module(f)
assert f.pehash==pehash and f.original==native
machines={'original':f.Runner(native),'compiled':f.Runner(bytes(patched))};matrix=f.run_domain(machines)
assert (matrix['cases'],matrix['snapshots_per_mode'],matrix['typed_child_callbacks_per_mode'])==(200,560,160)
assert hashes()==before and tools()==tc and hashlib.sha256(EXE.read_bytes()).hexdigest()==pehash
object_after={str(out):hashlib.sha256(out.read_bytes()).hexdigest() for out in outputs};assert object_before==object_after
report=dict(status='NATIVE_THREADED_FILE_FINISH_HANDLE_PASS',root_symbol=ROOT,bytes=len(b),instructions=len(ins),grade=grade,relocated_exact=True,fixups={str(k):(name,kind,0) for k,(name,kind) in rels.items()},caller_root_calls=calls,source_omission=dict(symbol=ROOT,diagnostic=diagnostic,missing_failed=True,provider_link_passed=True),input_sha256_before=before,input_sha256_after=hashes(),toolchain_sha256_before=tc,toolchain_sha256_after=tools(),original_exe_sha256_before=pehash,original_exe_sha256_after=hashlib.sha256(EXE.read_bytes()).hexdigest(),object_sha256_before=object_before,object_sha256_after=object_after,
 original_body_sha256=hashlib.sha256(native).hexdigest(),compiled_body_sha256=hashlib.sha256(patched).hexdigest(),matrix=matrix,
 dependency_inventory=dependency_inventory,caller_disassembly=[(i.mnemonic,i.op_str) for i in caller_ins],caller_abi_cases=caller_cases,caller_abi='Actual_main machine proof: ECXargv, zeroexplicitargs/RET0, rawDWORDpointer result translated toboolint, preservedregs/SP; linkage EXE neverrun',link_executable_executed=False,manager_link_abort_executed=False,global_link_storage_executed=False,
 executable_cache_policy='Separate immutable original/compiled Unicorn machines; executable bytes installed before any translation.',
 boundary='Existing25B root source ODR/API correction, not new recovery or child provider. ActualtypedCloseAllStreamsForFile(child) remains explicit functional boundary160callbacks; no actual managerbody/physicalownership/concurrency/lifetime/null-this-fault claim. Full32KiB arena+4KiBglobals per snapshot. Only-1 skips; nullhandle calls child, livehandle returns after callback, rootthiscall/RET0/noexplicitargs. Exacttypedglobal and childabort linkboundaries neverrun, not productionglobalstorage/layout. Existing descriptive013BC870 mapping, no duplicate static/global provider. Rawnear score not acceptance: whole relocated25B/10ins and actual2zeroaddend genuine targets exact.')
assert caller_cases==25
receipt.parent.mkdir(parents=True,exist_ok=True);receipt.write_text(json.dumps(report,indent=2)+'\n');print(report['status'],200,560,160,25,flush=True)
