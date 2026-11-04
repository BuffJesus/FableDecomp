"""Fresh three-provider exact relocation, typed linkage, independent immutable UC proof."""
import hashlib,importlib.util,itertools,json,re,struct,subprocess,sys,tempfile
from pathlib import Path
R=Path(__file__).resolve().parents[3];sys.path[:0]=[str(R/'tools'),str(R/'tools/decomp_pipeline')]
import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
w=Path(__file__).parent;SOURCE=R/'rebuild/src/compiled/00/9d/CBankFileAsyncData_RelocateData_009d5670.cpp'
MODEL=w/'NativeAsyncDataRelocate_model.py';REQUIRED=w/'NativeAsyncDataRelocate_required.cpp'
ROOT='?RelocateData@CBankFileAsyncData@@UAEXJPAX0J@Z'
LOCK='??0CCriticalSectionLock@@QAE@AAU_RTL_CRITICAL_SECTION@@_N@Z';UNLOCK='??1CCriticalSectionLock@@QAE@XZ'
LS=R/'rebuild/src/compiled/00/a6/CCriticalSectionLock_Constructor_00a62920.cpp';US=R/'rebuild/src/compiled/00/a6/CCriticalSectionLock_Destructor_00a629a0.cpp'
def main():
 inputs={Path(__file__),MODEL,REQUIRED,SOURCE,LS,US,R/'tools/decomp_pipeline/gate_native_async_data_relocate.py',R/'tools/pdb_class_methods.py',R/'debug_build/Ego_r.pdb'}
 for n in ('check_one.py','gate_game_component_abi.py','pe_oracle.py','near_match.py','verify_and_land.py','variable_allocator_parity.py','crawl/rowtrim.py','crawl/purity.py'):inputs.add(R/'tools/decomp_pipeline'/n)
 inputs.add(R/'tools/artifact_layout.py');visited=set()
 def headers(p):
  if p in visited:return
  visited.add(p);inputs.add(p)
  for name in re.findall(r'^\s*#\s*include\s*"([^"]+)"',p.read_text(encoding='utf-8'),re.M):
   q=next((q for q in (p.parent/name,R/'rebuild/include'/name) if q.is_file()),None);assert q,name;headers(q)
 for p in (SOURCE,REQUIRED,LS,US):headers(p)
 def hashes():return {str(p.relative_to(R)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sorted(inputs)}
 def toolchain():return {str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in [V.VC/'bin'/n for n in ('cl.exe','c1xx.dll','c2.dll','link.exe','mspdb71.dll')]}
 before=hashes();tc=toolchain();env=V.env();raw=EXE.read_bytes();exe_before=hashlib.sha256(raw).hexdigest()
 build=Path(tempfile.mkdtemp(prefix='async-relocate-fresh-'));obj=build/'root.obj';ro=build/'required.obj';lo=build/'lock.obj';uo=build/'unlock.obj'
 for source,target in ((SOURCE,obj),(REQUIRED,ro),(LS,lo),(US,uo)):
  result=V.cl(['/nologo','/c','/O2','/MT','/I'+str(R/'rebuild/include'),'/Fo'+str(target),str(source)],env,build);assert result.returncode==0,result.stdout+result.stderr
 # Fresh retail IAT and final secondary-table identity; not stale work packets.
 import pefile
 from pdb_class_methods import source as diasource,children,publics
 pe=pefile.PE(data=raw);imports={item.address:(entry.dll.decode(),item.name.decode() if item.name else None) for entry in pe.DIRECTORY_ENTRY_IMPORT for item in entry.imports}
 for address,name in ((0x143fe0c,'EnterCriticalSection'),(0x143fe10,'LeaveCriticalSection'),(0x143fd30,'TryEnterCriticalSection')):
  assert imports[address][0].lower()=='kernel32.dll' and imports[address][1]==name
 assert struct.unpack('<IIII',read_virtual_data(raw,0x129b8a0,16))==(0x9d80a0,0x9d5670,0x9d55d0,0x9d5100)
 dia,src=diasource();src.loadDataFromPdb(str(R/'debug_build/Ego_r.pdb'));g=src.openSession().globalScope;udt=g.findChildren(11,'CBankFileAsyncData',0).Item(0)
 methods=[dict(name=x.name,signature=x.undecoratedName,access=x.access,calling_convention=x.type.callingConvention,virtual=bool(x.virtual),parameters=[a.type.length for a in children(dia,x.type,20)]) for x in children(dia,udt,5) if x.name=='RelocateData']
 assert len(methods)==1 and methods[0]['access']==3 and methods[0]['calling_convention']==11 and methods[0]['virtual'] and methods[0]['parameters']==[4,4,4,4]
 assert publics('?RelocateData@CBankFileAsyncData',str(R/'debug_build/Ego_r.pdb'))
 objects=[obj,lo,uo];symbols=[ROOT,LOCK,UNLOCK]
 specs=[(ROOT,0x9d5670,78,{22:(LOCK,20,0xa62920),67:(UNLOCK,20,0xa629a0)}),(LOCK,0xa62920,54,{20:('__imp__TryEnterCriticalSection@4',6,0x143fd30),40:('__imp__EnterCriticalSection@4',6,0x143fe0c)}),(UNLOCK,0xa629a0,25,{15:('__imp__LeaveCriticalSection@4',6,0x143fe10)})]
 compiled={};original={};audits=[]
 for path,(symbol,address,length,expected) in zip(objects,specs):
  co=Coff(path);s,o=co.symbols[symbol];assert o==0
  body,rels=co.sections[s];assert len(body)==length and rels=={site:(n,k) for site,(n,k,target) in expected.items()}
  instructions=list(Cs(CS_ARCH_X86,CS_MODE_32).disasm(body,address));assert sum(i.size for i in instructions)==length
  if address==0x9d5670:
   assert len(instructions)==28
   assert any(i.op_str=='eax, dword ptr [esi + 0x24]' for i in instructions)
   assert any('[esi + 8]' in i.op_str for i in instructions) and any('[esi + 4]' in i.op_str for i in instructions)
   assert instructions[-1].mnemonic=='ret' and instructions[-1].op_str=='0x10'
  patched=bytearray(body);fix=[]
  for site,(name,kind,target) in sorted(expected.items()):
   assert struct.unpack_from('<I',body,site)[0]==0
   struct.pack_into('<I',patched,site,(target-(address+site+4) if kind==20 else target)&0xffffffff);fix.append(dict(site=site,symbol=name,kind=kind,addend=0,target=hex(target)))
  original[address]=read_virtual_data(raw,address,length);assert bytes(patched)==original[address]
  if address==0x9d5670:assert hashlib.sha256(original[address]).hexdigest()=='bdd1cf882ef0c36eb02d13f1031a927de81ff6f11c8975cfd270d080f8253f01'
  compiled[address]=bytes(patched);audits.append(dict(symbol=symbol,address=hex(address),bytes=length,instructions=len(instructions),grade='RELOCATION_MATCH',fixups=fix,disassembly=[dict(address=hex(i.address),mnemonic=i.mnemonic,operands=i.op_str) for i in instructions],object_sha256=hashlib.sha256(path.read_bytes()).hexdigest(),extra_helper_credit=False))
 caller=Coff(ro)
 mainsec,mainoff=caller.symbols['_main'];mainbody,mainrels=caller.sections[mainsec];assert mainoff==0
 mainins=list(Cs(CS_ARCH_X86,CS_MODE_32).disasm(mainbody,0));assert sum(i.size for i in mainins)==len(mainbody)
 assert mainins[0].mnemonic=='mov' and mainins[0].op_str=='edx, dword ptr [esp + 4]'
 assert mainins[1].mnemonic=='mov' and mainins[1].op_str=='eax, dword ptr [esp + 8]'
 assert any(i.mnemonic=='lea' and i.op_str=='ecx, [eax + 4]' for i in mainins),'Natural qualifiedcaller must derive secondary-this from original argv'
 callsites=[site for site,(name,kind) in mainrels.items() if name==ROOT and kind==20];assert len(callsites)==1
 rootcall=next(i for i in mainins if i.address<callsites[0]<i.address+i.size);assert rootcall.mnemonic=='call'
 assert [i.op_str for i in mainins if i.address<rootcall.address and i.mnemonic=='push'][-4:]==['esi','ecx','eax','edx']
 assert any(n==ROOT and k==20 and struct.unpack_from('<I',b,site)[0]==0 for b,rs in caller.sections.values() for site,(n,k) in rs.items())
 command=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(build/'required.exe'),'kernel32.lib',str(ro)];omissions=[]
 for omitted,symbol in enumerate(symbols):
  missing=subprocess.run(command+[str(p) for i,p in enumerate(objects) if i!=omitted],env=env,capture_output=True,text=True)
  assert missing.returncode and any(symbol in line and ('LNK2001' in line or 'LNK2019' in line) for line in (missing.stdout+missing.stderr).splitlines()),missing.stdout+missing.stderr
  control=subprocess.run(command+[str(p) for p in objects],env=env,capture_output=True,text=True);assert control.returncode==0,control.stdout+control.stderr
  omissions.append(dict(symbol=symbol,missing_failed=True,provider_link_passed=True,missing_returncode=missing.returncode,missing_stdout=missing.stdout,missing_stderr=missing.stderr,control_returncode=control.returncode,control_stdout=control.stdout,control_stderr=control.stderr))
 # Link EXE NEVER executes. All three providers genuine, no abort aliases.
 spec=importlib.util.spec_from_file_location('permanent_relocate_model',MODEL);m=importlib.util.module_from_spec(spec);spec.loader.exec_module(m)
 originalrun=m.build_runner(original);compiledrun=m.build_runner(compiled);cases=ap=lp=0
 for buffer,data,pair,ignored,mutation,place,fill,stackfill in itertools.product((0,1,0x7fffffff,0x80000000,0xffffffff),(0,1,0x7fffffff,0x80000000,0xffffffff),range(7),range(3),range(6),range(2),(0,0x95),(0,0x95)):
  origin=m.A+place*0x3000;d=origin+0x1000;old,new=((0,0),(0,0xffffffff),(0xffffffff,0),(0x80000000,0x7fffffff),(0x12345678,0x12345678),(d+12,d+8),(d+8,d+12))[pair]
  i1,i4=((0,0xffffffff),(0xffffffff,0),(0x80000000,0x80000000))[ignored]
  initial=bytearray([fill])*m.N;memory=m.Memory(initial);c=dict(data=d,bank=origin+0x200,bank2=origin+0x800,old=old,new=new,ignored1=i1,ignored4=i4,mutation=mutation,stackfill=stackfill)
  memory.put(d+40,c['bank']);memory.put(d+12,buffer);memory.put(d+8,data)
  expected=m.model(initial,c);actualoriginal=originalrun(bytes(initial),c);actualcompiled=compiledrun(bytes(initial),c)
  assert actualoriginal==expected,(c,'original');assert actualcompiled==expected,(c,'compiled')
  cases+=1;ap+=len(expected[1]);lp+=len(expected[2])
 assert all(set(run.hits)=={'0x9d5670','0xa62920','0xa629a0'} and all(n==cases for n in run.hits.values()) and all(n==cases for n in run.phases.values()) for run in (originalrun,compiledrun))
 assert (cases,ap,lp)==(25200,126000,75600) and before==hashes() and tc==toolchain() and hashlib.sha256(EXE.read_bytes()).hexdigest()==exe_before
 report=dict(status='NATIVE_ASYNC_DATA_RELOCATE_PASS',cases=cases,arena_snapshots=ap,lock_snapshots=lp,original_cases=cases,original_arena_snapshots=ap,original_lock_snapshots=lp,compiled_cases=cases,compiled_arena_snapshots=ap,compiled_lock_snapshots=lp,providers=audits,source_omissions=omissions,link_executable_executed=False,root_object_recompiled=True,separate_immutable_uc_instances=True,qualified_caller_disassembly=[dict(address=hex(i.address),bytes=i.bytes.hex(),mnemonic=i.mnemonic,operands=i.op_str) for i in mainins],input_sha256_before=before,input_sha256_after=hashes(),toolchain_sha256_before=tc,toolchain_sha256_after=toolchain(),original_exe_sha256=hashlib.sha256(raw).hexdigest(),boundary='Fresh exact78B secondaryreceiver root plus freshlycompiled genuine54B/25B blockinglocks, separateimmutableUCchains vs independentmodel. Only Win32Enter/Leave interfaces, noaliases/EXEexecution. Defineduint32wrap/nullpreservation/livefields/BufferthenData phase snapshots safe because complete78B exactbyte equality; oldnew rawaliases/ignoredlongs1+4/RET16/calleesaves/cachedsection/afterLeavepersist/padding. No C++shim/allocation/concurrency guarantee; existing lockhelpers noextra credit.')
 report.update(original_exe_sha256_after=hashlib.sha256(EXE.read_bytes()).hexdigest(),verified_snapshot_phase_counts={name:cases for name in ('enter','buffer_phase','data_phase','leave','final')},body_chain_invocations_each=cases,mode_provider_hits=dict(original=originalrun.hits,compiled=compiledrun.hits),mode_phase_counts=dict(original=originalrun.phases,compiled=compiledrun.phases),DIA=methods,Win32_imports={hex(a):imports[a] for a in (0x143fd30,0x143fe0c,0x143fe10)},source_objects_freshly_compiled=True)
 receipt=R/'rebuild/compile-gate/native-async-data-relocate.json';receipt.parent.mkdir(parents=True,exist_ok=True);receipt.write_text(json.dumps(report,indent=2)+'\n');print(report['status'],cases,ap,lp)
if __name__=='__main__':main()
