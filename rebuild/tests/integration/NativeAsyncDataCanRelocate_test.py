"""One first ordinary compile, explicit secondary ABI audit; no retuning."""
import hashlib,importlib.util,itertools,json,re,struct,subprocess,sys,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
R=Path(__file__).resolve().parents[3];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import check_one as V
from gate_game_component_abi import Coff
from near_match import compare,retail_relocations
from pe_oracle import EXE,read_virtual_data
W=Path(__file__).parent;source=R/'rebuild/src/compiled/00/9d/CBankFileAsyncData_CanRelocateData_009d55d0.cpp'
ROOT='?CanRelocateData@CBankFileAsyncData@@UBE_NJ@Z';LOCK='??0CCriticalSectionLock@@QAE@AAU_RTL_CRITICAL_SECTION@@_N@Z';UNLOCK='??1CCriticalSectionLock@@QAE@XZ'
required=W/'NativeAsyncDataCanRelocate_required.cpp';fixture=W/'NativeAsyncDataCanRelocate_model.py';model=fixture
sources=[source,R/'rebuild/src/compiled/00/a6/CCriticalSectionLock_Constructor_00a62920.cpp',R/'rebuild/src/compiled/00/a6/CCriticalSectionLock_Destructor_00a629a0.cpp']
inputs={Path(__file__),required,fixture,model,R/'tools/decomp_pipeline/gate_native_async_data_can_relocate.py'};seen=set()
def headers(p):
 if p in seen:return
 seen.add(p);inputs.add(p)
 for name in re.findall(r'^\s*#\s*include\s*"([^"]+)"',p.read_text(),re.M):
  q=next((q for q in (p.parent/name,R/'rebuild/include'/name) if q.is_file()),None);assert q;headers(q)
for p in [*sources,required]:headers(p)
for n in ('check_one.py','gate_game_component_abi.py','pe_oracle.py','near_match.py','verify_and_land.py','variable_allocator_parity.py','crawl/rowtrim.py','crawl/purity.py'):inputs.add(R/'tools/decomp_pipeline'/n)
inputs.add(R/'tools/artifact_layout.py');inputs.add(R/'tools/pdb_class_methods.py');inputs.add(R/'debug_build/Ego_r.pdb')
def hashes():return {str(p.relative_to(R)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sorted(inputs)}
def toolhashes():return {str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in (V.VC/'bin'/n for n in ('cl.exe','c1xx.dll','c2.dll','link.exe','mspdb71.dll'))}
before=hashes();tc=toolhashes();raw=EXE.read_bytes();pehash=hashlib.sha256(raw).hexdigest();build=Path(tempfile.mkdtemp(prefix='can-relocate-first-'));md=Cs(CS_ARCH_X86,CS_MODE_32);obj=build/'root.obj';receipt=R/'rebuild/compile-gate/native-async-data-can-relocate.json'
objects=[];bodies={};audits=[];targets={LOCK:0xa62920,UNLOCK:0xa629a0,'__imp__TryEnterCriticalSection@4':0x143fd30,'__imp__EnterCriticalSection@4':0x143fe0c,'__imp__LeaveCriticalSection@4':0x143fe10}
for i,(p,symbol,address,size) in enumerate(zip(sources,(ROOT,LOCK,UNLOCK),(0x9d55d0,0xa62920,0xa629a0),(76,54,25))):
 out=obj if i==0 else build/('provider%d.obj'%i);r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(R/'rebuild/include'),'/Fo'+str(out),str(p)],V.env(),build);assert r.returncode==0,r.stdout+r.stderr;objects.append(out)
 c=Coff(out);section,offset=c.symbols[symbol];assert offset==0;b,rels=c.sections[section];patched=bytearray(b);fixups=[]
 for site,(name,kind) in rels.items():
  assert name in targets and kind in (6,20) and struct.unpack_from('<I',b,site)[0]==0
  struct.pack_into('<I',patched,site,(targets[name]-address-site-4 if kind==20 else targets[name])&0xffffffff);fixups.append(dict(site=site,symbol=name,kind=kind,addend=0,target=hex(targets[name])))
 grade=compare(b,read_virtual_data(raw,address,size),tuple(rels),retail_relocations('%08x'%address,size)).to_dict();instructions=list(md.disasm(b,address))
 audits.append(dict(symbol=symbol,source=str(p.relative_to(R)),bytes=len(b),instructions=len(instructions),grade=grade,fixups=fixups,disassembly=[dict(address=hex(x.address),mnemonic=x.mnemonic,operands=x.op_str) for x in instructions],object_sha256=hashlib.sha256(out.read_bytes()).hexdigest()))
 bodies[address]=bytes(patched)
 assert len(b)==size and bytes(patched)==read_virtual_data(raw,address,size)
 if i==0:assert rels=={22:(LOCK,20),45:(UNLOCK,20),63:(UNLOCK,20)}
 if i==1:assert rels=={20:('__imp__TryEnterCriticalSection@4',6),40:('__imp__EnterCriticalSection@4',6)}
 if i==2:assert rels=={15:('__imp__LeaveCriticalSection@4',6)}
 if i==0:
  assert {name for name,kind in rels.values()}=={LOCK,UNLOCK}
  assert any(x.mnemonic=='mov' and x.op_str=='eax, dword ptr [esi + 0x24]' for x in instructions),'Secondary Bank offset missing'
  assert any('[esi + 0x40]' in x.op_str for x in instructions) and any('[esi + 0x41]' in x.op_str for x in instructions),'Secondary flag offsets missing'
ro=build/'required.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(R/'rebuild/include'),'/Fo'+str(ro),str(required)],V.env(),build);assert r.returncode==0,r.stdout+r.stderr
caller=Coff(ro);calls=[]
for b,rels in caller.sections.values():
 for site,(name,kind) in rels.items():
  if name==ROOT:
   assert kind==20 and struct.unpack_from('<I',b,site)[0]==0;ins=list(md.disasm(b,0));assert any(x.mnemonic=='add' and x.op_str=='ecx, 4' for x in ins),'Natural secondary-this +4 missing';calls.append(dict(site=site,symbol=name,kind=kind,disassembly=[(x.mnemonic,x.op_str) for x in ins]))
assert len(calls)==1
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(build/'required.exe'),'kernel32.lib',str(ro)];omissions=[]
for i,symbol in enumerate((ROOT,LOCK,UNLOCK)):
 r=subprocess.run(cmd+[str(o) for j,o in enumerate(objects) if j!=i],env=V.env(),capture_output=True,text=True);diagnostic=r.stdout+r.stderr;assert r.returncode and any(symbol in line and ('LNK2001' in line or 'LNK2019' in line) for line in diagnostic.splitlines());omissions.append(dict(symbol=symbol,diagnostic=diagnostic))
r=subprocess.run(cmd+[str(o) for o in objects],env=V.env(),capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr

spec=importlib.util.spec_from_file_location('permanent_can_model',model);f=importlib.util.module_from_spec(spec);spec.loader.exec_module(f)
original=f.Runner(f.providers);compiled=f.Runner(bodies);cases=points=0
for finished,locked,arg,place,fill,stackfill,mutation in itertools.product(f.DOMAIN['raw_bool_bytes'],f.DOMAIN['raw_bool_bytes'],f.DOMAIN['ignored_long'],range(2),(0,0x95),(0,0x95),range(6)):
 base=f.A+place*0x3000;initial=bytearray([fill])*f.N;memory=f.Memory(initial);case=dict(data=base+0x1000,bank=base+0x200,bank2=base+0x800,argument=arg,stackfill=stackfill,mutation=mutation)
 memory.put(case['data']+40,case['bank']);memory.write(case['data']+68,bytes([finished,locked]));want=f.expected(initial,case)
 for machine in (original,compiled):assert machine.run(bytes(initial),case)==want,case
 cases+=1;points+=len(want[1])
assert (cases,points)==(6000,18000)
assert all(n==cases for machine in (original,compiled) for n in machine.hits.values())

report=dict(status='NATIVE_ASYNC_DATA_CAN_RELOCATE_PASS',cases=cases,snapshots_per_root=points,lock_snapshots_per_root=cases*3,mode_provider_hits=dict(original=original.hits,compiled=compiled.hits),DIA=f.methods,Win32_imports={hex(a):f.imports[a] for a in (0x143fd30,0x143fe0c,0x143fe10)},providers=audits,caller=calls,source_omissions=omissions,link_control_passed=True,link_executable_executed=False,input_sha256_before=before,toolchain_sha256_before=tc,original_exe_sha256=pehash,boundary='Exact secondary-this76B root; existing exact54B/25B RAII prerequisites. Only Win32Enter/Leave functional boundaries. Full32KiB arena and8B cachedlock padding, rawbools/ignoredlong, cachedpredicate acrossLeave, ALRET4/preservedregs. No physicalconcurrency/lifetime or additional helper recovery credit.')

assert hashes()==before and toolhashes()==tc and hashlib.sha256(EXE.read_bytes()).hexdigest()==pehash
report.update(input_sha256_after=hashes(),toolchain_sha256_after=toolhashes(),translation_cache_policy='Separate immutable Unicorn machines; executable bodies mapped once')
receipt.parent.mkdir(parents=True,exist_ok=True);receipt.write_text(json.dumps(report,indent=2)+'\n')
print('NATIVE_ASYNC_DATA_CAN_RELOCATE_PASS',cases,points,cases*3,flush=True)
