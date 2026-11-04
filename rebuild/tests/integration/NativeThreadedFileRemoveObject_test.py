"""Fresh wrapper provider proof; actual manager remains an original-byte boundary."""
import hashlib,importlib.util,json,re,struct,subprocess,sys,tempfile,traceback
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
import pefile
R=Path(__file__).resolve().parents[3];W=Path(__file__).parent;sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
D=Path(tempfile.mkdtemp(prefix='threaded-remove-proof-'))
def preserve_failure(exc_type,exc_value,exc_tb):
 (D/'failure.log').write_text(''.join(traceback.format_exception(exc_type,exc_value,exc_tb)))
 sys.__excepthook__(exc_type,exc_value,exc_tb)
sys.excepthook=preserve_failure
source=R/'rebuild/src/compiled/00/98/CThreadedFile_RemoveObject_0098ec90.cpp'
required=W/'NativeThreadedFileRemoveObject_required.cpp'
unused=W/'NativeThreadedFileRemoveObject_unused_link.cpp'
fixture=W/'NativeThreadedFileRemoveObject_model.py'
sources=[source,required,unused];outputs=[D/'root.obj',D/'required.obj',D/'unused.obj'];obj=outputs[0]
receipt=R/'rebuild/compile-gate/native-threaded-file-remove-object.json'
ROOT='?RemoveObject@CThreadedFile@@QAE_NK@Z';CHILD='?RemoveObject@CThreadedFileManager@@QAE_NK@Z';GLOBAL='?g_FableThreadedFileManager_013BC870@@3VCThreadedFileManager@@A'
inputs={Path(__file__),*sources,fixture,R/'tools/decomp_pipeline/gate_native_threaded_file_remove_object.py'};seen=set()
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
object_before={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in outputs}
c=Coff(obj);s,o=c.symbols[ROOT];assert o==0;b,rels=c.sections[s];assert rels=={1:(GLOBAL,6),6:(CHILD,20)};patched=bytearray(b)
assert len(b)==10
for site,(name,kind) in rels.items():
 assert struct.unpack_from('<I',b,site)[0]==0;target=0x13bc870 if name==GLOBAL else 0x98eb20;struct.pack_into('<I',patched,site,(target-0x98ec90-site-4 if kind==20 else target)&0xffffffff)
native=read_virtual_data(raw,0x98ec90,10);assert bytes(patched)==native;ins=list(Cs(CS_ARCH_X86,CS_MODE_32).disasm(patched,0x98ec90));assert [(i.mnemonic,i.op_str) for i in ins]==[('mov','ecx, 0x13bc870'),('jmp','0x98eb20')]
caller=Coff(outputs[1]);calls=[(s,site) for s,(bb,rr) in caller.sections.items() for site,(name,kind) in rr.items() if name==ROOT and kind==20 and struct.unpack_from('<I',bb,site)[0]==0];assert len(calls)==1
assert caller.symbols['_main']==(calls[0][0],0)
assert caller.sections[calls[0][0]][1]=={calls[0][1]:(ROOT,20)}
caller_ins=list(Cs(CS_ARCH_X86,CS_MODE_32).disasm(caller.sections[calls[0][0]][0],0))
call_index=next(i for i,x in enumerate(caller_ins) if x.address+1==calls[0][1] and x.mnemonic=='call')
assert any(i.mnemonic=='movzx' and i.op_str=='eax, al' for i in caller_ins[call_index+1:])
assert not any(i.mnemonic=='add' and i.op_str.startswith('esp,') for i in caller_ins[call_index+1:]),'thiscall argument must be callee-cleaned'
# Execute actual required _main bytes only, with one bounded ROOT interface;
# this proves ECX=argv and stack uid by value rather than anypush+mov heuristic.
CB=0x5000000;BOUNDARY=0x5001000;CS=0x6000000;STOP=0x7000000;caller_body=bytearray(caller.sections[calls[0][0]][0]);struct.pack_into('<I',caller_body,calls[0][1],(BOUNDARY-CB-calls[0][1]-4)&0xffffffff)
caller_cases=0
for uid in (0,1,0x7fffffff,0x80000000,0xffffffff):
 for receiver in (0,0x12345678,0xffffffff):
  for boolean in (0,1):
   uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(CB,8192);uc.mem_map(CS,4096);uc.mem_map(STOP,4096);uc.mem_write(CB,bytes(caller_body));sp=CS+0x800;uc.mem_write(sp,struct.pack('<III',STOP,uid,receiver));uc.reg_write(UC_X86_REG_ESP,sp);seen_call=[False]
   def caller_hook(u,pc,n,_):
    if pc==STOP:u.emu_stop();return
    if pc!=BOUNDARY:return
    stack=u.reg_read(UC_X86_REG_ESP);ret,arg=struct.unpack('<II',u.mem_read(stack,8));assert arg==uid and u.reg_read(UC_X86_REG_ECX)==receiver;seen_call[0]=True;u.reg_write(UC_X86_REG_EAX,0xabcdef00|boolean);u.reg_write(UC_X86_REG_ESP,stack+8);u.reg_write(UC_X86_REG_EIP,ret)
   uc.hook_add(UC_HOOK_CODE,caller_hook);uc.emu_start(CB,STOP,count=100);assert seen_call[0] and uc.reg_read(UC_X86_REG_EIP)==STOP and uc.reg_read(UC_X86_REG_ESP)==sp+4 and uc.reg_read(UC_X86_REG_EAX)==boolean;caller_cases+=1
dependency_inventory=[]
for index,path in enumerate(outputs):
 co=Coff(path);data=path.read_bytes();machine,count,stamp,symptr,symcount,optional,flags=struct.unpack_from('<HHIIIHH',data)
 assert machine==0x14c and optional==0
 for section in range(1,count+1):
  at=20+(section-1)*40;sectionname=data[at:at+8].rstrip(b'\0').decode();virtual_size,virtual_address,raw_size,raw_pointer,rel_pointer,line_pointer,rel_count,line_count,characteristics=struct.unpack_from('<IIIIIIHHI',data,at+8)
  uninitialized=bool(characteristics&0x80);payload=data[raw_pointer:raw_pointer+raw_size] if raw_pointer else b''
  assert not raw_pointer or len(payload)==raw_size;fixups=[]
  rr=co.sections[section][1]
  for site,(target,kind) in rr.items():
   # Debug relocations have their own kinds/addends; inventory, don't impose
   # executable REL32 zero-addend assumptions on data/debug/BSS sections.
   addend=payload[site:site+4].hex() if site+4<=len(payload) else None
   fixups.append(dict(site=site,symbol=target,kind=kind,raw_addend_hex=addend))
  dependency_inventory.append(dict(object_index=index,section=section,section_name=sectionname,characteristics=hex(characteristics),virtual_size=virtual_size,raw_size=raw_size,raw_pointer=raw_pointer,initialized_payload_bytes=len(payload),payload_sha256=hashlib.sha256(payload).hexdigest() if payload else None,uninitialized=uninitialized,bss_extent=raw_size if uninitialized and raw_pointer==0 else 0,symbols=[dict(name=name,offset=offset) for name,(sec,offset) in co.symbols.items() if sec==section],fixups=fixups))
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(D/'required.exe'),'kernel32.lib',*[str(p) for p in outputs[1:]]]
r=subprocess.run(cmd,env=V.env(),capture_output=True,text=True);diagnostic=r.stdout+r.stderr;assert r.returncode and any(ROOT in line and ('LNK2001' in line or 'LNK2019' in line) for line in diagnostic.splitlines())
(D/'root-omission.link.log').write_text(diagnostic)
r=subprocess.run(cmd+[str(obj)],env=V.env(),capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
(D/'positive-control.link.log').write_text(r.stdout+r.stderr)
assert hashes()==before and tools()==tc and hashlib.sha256(EXE.read_bytes()).hexdigest()==pehash

pe=pefile.PE(data=raw)
import_targets={i.address:(e.dll.decode(),i.name.decode() if i.name else None) for e in pe.DIRECTORY_ENTRY_IMPORT for i in e.imports}
for slot,name in ((0x143fe0c,'EnterCriticalSection'),(0x143fe10,'LeaveCriticalSection'),(0x143fdf8,'GlobalFree')):
 assert import_targets[slot][0].lower()=='kernel32.dll' and import_targets[slot][1]==name
spec=importlib.util.spec_from_file_location('threaded_remove_model',fixture);f=importlib.util.module_from_spec(spec);spec.loader.exec_module(f)
assert f.pehash==pehash and f.providers[f.WRAPPER]==bytes(patched)
original_wrapper=f.Runner(f.WRAPPER);original_manager=f.Runner(f.MANAGER)
compiled_wrapper=f.Runner(f.WRAPPER,bytes(patched));compiled_manager=f.Runner(f.MANAGER,bytes(patched))
original=f.run_domain(original_wrapper,original_manager);compiled=f.run_domain(compiled_wrapper,compiled_manager)
assert original==compiled
assert (original['cases'],original['snapshots_per_chain'])==(4224,25248)
assert caller_cases==30
assert hashes()==before and tools()==tc and hashlib.sha256(EXE.read_bytes()).hexdigest()==pehash
object_hashes={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in outputs}
assert object_before==object_hashes
report=dict(status='NATIVE_THREADED_FILE_REMOVE_OBJECT_PASS',root_symbol=ROOT,bytes=len(b),instructions=len(ins),relocated_exact=True,fixups={str(k):(name,kind,0) for k,(name,kind) in rels.items()},
 cases=original['cases'],snapshots_per_chain=original['snapshots_per_chain'],mutation_partitions=original['mutation_partitions'],results=original['results'],
 mode_provider_hits=dict(original=original['mode_provider_hits'],compiled=compiled['mode_provider_hits']),
 original_body_sha256={hex(a):hashlib.sha256(bb).hexdigest() for a,bb in f.providers.items()},compiled_body_sha256=hashlib.sha256(patched).hexdigest(),
 source_omission=dict(symbol=ROOT,diagnostic=diagnostic,missing_failed=True,provider_link_passed=True),
 caller_root_calls=calls,caller_disassembly=[(i.mnemonic,i.op_str) for i in caller_ins],caller_abi_cases=caller_cases,
 dependency_inventory=dependency_inventory,imports={hex(a):import_targets[a] for a in (0x143fe0c,0x143fe10,0x143fdf8)},
 input_sha256_before=before,input_sha256_after=hashes(),toolchain_sha256_before=tc,toolchain_sha256_after=tools(),original_exe_sha256_before=pehash,original_exe_sha256_after=hashlib.sha256(EXE.read_bytes()).hexdigest(),
 object_sha256_before=object_before,object_sha256_after=object_hashes,object_sha256=object_hashes,link_executable_executed=False,manager_link_abort_executed=False,global_link_storage_executed=False,
 executable_cache_policy='Four separate immutable Unicorn machines; all executable provider bytes installed before translation.',
 boundary='Only genuine10B wrapper source provider. Actual original156B manager is original-data boundary; seven typed child callbacks (Enter/Leave/find/pending/GlobalFree/scalar-delete/iterator-erase). Raw32KiB arena plus4KiB singleton snapshots, cachedsection/object/iterator and liveheader/state/handle. GlobalFree fullDWORDreturn ignored/noautomatic handle clear. Descriptive existing013BC870 global is donor-static identity mapping, not duplicate production storage/staticalias. Unused-link exacttyped global definition and manager abort are never executed, have no source/data/helper credit. No genuine managerlayout/C++source/physicalheap ownership/concurrency/lifetime proof.')
receipt.parent.mkdir(parents=True,exist_ok=True);receipt.write_text(json.dumps(report,indent=2)+'\n')
print(report['status'],4224,25248,30,flush=True)
