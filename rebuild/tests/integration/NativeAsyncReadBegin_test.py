"""Fresh read-begin/RAII chains vs independent original and field model."""
import hashlib,importlib.util,json,re,struct,subprocess,sys,tempfile
from pathlib import Path
R=Path(__file__).resolve().parents[3];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import pefile
ROOT='?OnFileReadBegin@CBankFileAsyncData@@UAE_NPAPAXW4EThreadedFilePriority@@@Z'
LOCK='??0CCriticalSectionLock@@QAE@AAU_RTL_CRITICAL_SECTION@@_N@Z'
UNLOCK='??1CCriticalSectionLock@@QAE@XZ'
SETUP='?SetupBuffer@CBankFileAsyncData@@AAE_N_N@Z'
SPECS=[(R/'rebuild/src/compiled/00/9d/CBankFileAsyncData_OnFileReadBegin_009d5440.cpp',ROOT,0x9d5440,92,{22:(LOCK,20,0xa62920),39:(SETUP,20,0x9d52d0),61:(UNLOCK,20,0xa629a0),79:(UNLOCK,20,0xa629a0)}),(R/'rebuild/src/compiled/00/a6/CCriticalSectionLock_Constructor_00a62920.cpp',LOCK,0xa62920,54,{20:('__imp__TryEnterCriticalSection@4',6,0x143fd30),40:('__imp__EnterCriticalSection@4',6,0x143fe0c)}),(R/'rebuild/src/compiled/00/a6/CCriticalSectionLock_Destructor_00a629a0.cpp',UNLOCK,0xa629a0,25,{15:('__imp__LeaveCriticalSection@4',6,0x143fe10)})]
MODEL=Path(__file__).with_name('NativeAsyncReadBegin_model.py')
REQUIRED=Path(__file__).with_name('NativeAsyncReadBegin_required.cpp')
GATE=R/'tools/decomp_pipeline/gate_native_async_read_begin.py'
def main():
 inputs={Path(__file__),MODEL,REQUIRED,GATE}
 for name in ('check_one.py','gate_game_component_abi.py','pe_oracle.py','near_match.py','verify_and_land.py','variable_allocator_parity.py','crawl/rowtrim.py','crawl/purity.py'):inputs.add(R/'tools/decomp_pipeline'/name)
 inputs.add(R/'tools/artifact_layout.py');visited=set()
 def headers(p):
  if p in visited:return
  visited.add(p);inputs.add(p)
  for name in re.findall(r'^\s*#\s*include\s*"([^"]+)"',p.read_text(encoding='utf-8'),re.M):
   q=next((q for q in (p.parent/name,R/'rebuild/include'/name) if q.is_file()),None);assert q,name
   headers(q)
 for p in [REQUIRED]+[s[0] for s in SPECS]:headers(p)
 def hashes():return {str(p.relative_to(R)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sorted(inputs)}
 def toolchain():return {str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in [V.VC/'bin'/n for n in ('cl.exe','c1xx.dll','c2.dll','link.exe','mspdb71.dll')]}
 before=hashes();tc=toolchain();env=V.env();build=Path(tempfile.mkdtemp(prefix='native-async-read-begin-'));raw=EXE.read_bytes();md=Cs(CS_ARCH_X86,CS_MODE_32)
 exe_before=hashlib.sha256(raw).hexdigest()
 assert struct.unpack('<3I',read_virtual_data(raw,0x129b8b4,12))[1]==0x9d5440
 pe=pefile.PE(data=raw);actual_imports={}
 for desc in pe.DIRECTORY_ENTRY_IMPORT:
  for item in desc.imports:
   if item.address in (0x143fd30,0x143fe0c,0x143fe10):actual_imports[item.address]=(desc.dll.decode(),item.name.decode())
 assert actual_imports=={0x143fd30:('KERNEL32.dll','TryEnterCriticalSection'),0x143fe0c:('KERNEL32.dll','EnterCriticalSection'),0x143fe10:('KERNEL32.dll','LeaveCriticalSection')}
 objects=[];compiled_bodies={};providers=[]
 for index,(source,symbol,address,length,expected) in enumerate(SPECS):
  obj=build/('provider%d.obj'%index)
  r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(R/'rebuild/include'),'/Fo'+str(obj),str(source)],env,build)
  assert r.returncode==0,r.stdout+r.stderr
  c=Coff(obj);section,offset=c.symbols[symbol];assert offset==0
  body,rels=c.sections[section];assert len(body)==length and rels=={site:(name,kind) for site,(name,kind,target) in expected.items()}
  patched=bytearray(body);audit=[]
  for site,(name,kind,target) in sorted(expected.items()):
   assert struct.unpack_from('<I',body,site)[0]==0
   struct.pack_into('<I',patched,site,(target-(address+site+4) if kind==20 else target)&0xffffffff)
   audit.append(dict(site=site,symbol=name,kind=kind,addend=0,target=hex(target)))
  original=read_virtual_data(raw,address,length);assert bytes(patched)==original
  instructions=list(md.disasm(original,address));assert sum(x.size for x in instructions)==length
  if symbol==ROOT:assert len(instructions)==32
  objects.append(obj);compiled_bodies[address]=bytes(patched)
  providers.append(dict(source=str(source.relative_to(R)),symbol=symbol,address=hex(address),bytes=length,instructions=len(instructions),grade='RELOCATION_MATCH',fixups=audit,object_sha256=hashlib.sha256(obj.read_bytes()).hexdigest(),original_sha256=hashlib.sha256(original).hexdigest(),recovery_boundary='Read-begin root' if index==0 else 'Existing accepted RAII prerequisite; no new helper recovery credit'))
 ro=build/'required.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(R/'rebuild/include'),'/Fo'+str(ro),str(REQUIRED)],env,build);assert r.returncode==0,r.stdout+r.stderr
 caller=Coff(ro);assert any(name==ROOT and kind==20 and struct.unpack_from('<I',body,site)[0]==0 for body,rs in caller.sections.values() for site,(name,kind) in rs.items())
 command=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(build/'required.exe'),'kernel32.lib',str(ro)]
 omissions=[]
 for omitted,spec in enumerate(SPECS):
  symbol=spec[1]
  missing=subprocess.run(command+[str(p) for i,p in enumerate(objects) if i!=omitted],env=env,capture_output=True,text=True)
  assert missing.returncode and any(symbol in line and ('LNK2001' in line or 'LNK2019' in line) for line in (missing.stdout+missing.stderr).splitlines()),missing.stdout+missing.stderr
  control=subprocess.run(command+[str(p) for p in objects],env=env,capture_output=True,text=True);assert control.returncode==0,control.stdout+control.stderr
  omissions.append(dict(symbol=symbol,missing_failed=True,provider_link_passed=True,missing_returncode=missing.returncode,missing_stdout=missing.stdout,missing_stderr=missing.stderr,control_returncode=control.returncode,control_stdout=control.stdout,control_stderr=control.stderr))
 # Linkage executable never runs. Model executes both original and fresh compiled chains.
 spec=importlib.util.spec_from_file_location('native_async_read_begin_model',MODEL);model=importlib.util.module_from_spec(spec);spec.loader.exec_module(model)
 result=model.run_fixture(compiled_bodies)
 assert result['status']=='ORIGINAL_COMPILED_ASYNC_READ_BEGIN_MODEL_PASS'
 assert (result['cases'],result['arena_snapshots'],result['lock_snapshots'])==(10368,34560,34560)
 assert before==hashes() and tc==toolchain() and hashlib.sha256(EXE.read_bytes()).hexdigest()==exe_before
 result.update(status='NATIVE_ASYNC_READ_BEGIN_PASS',original_cases=10368,original_snapshots=34560,original_lock_snapshots=34560,compiled_cases=10368,compiled_snapshots=34560,compiled_lock_snapshots=34560,providers=providers,source_omissions=omissions,link_executable_executed=False,source_sha256=before,source_sha256_after=hashes(),toolchain_sha256_before=tc,toolchain_sha256_after=toolchain(),compiler_flags=['/nologo','/c','/O2','/MT'],original_exe_sha256_before=exe_before,original_exe_sha256_after=hashlib.sha256(EXE.read_bytes()).hexdigest(),actual_iat_imports={hex(a):dict(dll=v[0],name=v[1]) for a,v in actual_imports.items()},boundary='Actual original/fresh compiled92B readbegin+54B trylock+25B dtor, separateimmutableUC engines vs independentmodel. SetupBuffer(false), Win32Try/Leave explicitfunctional boundaries; Setup abortlinkalias ONLYunusedEXE neverrun. All3providers strictomission/control, helpers noextra credit. Full32KiB+8Blockpadding, liveBuffer outputalias/cachedsection/afterLeavepersist/ignoredpriority/RET8/ALboolean/calleesaves. No childimplementation/concurrency/allocation/nulloutputfault guarantee')
 out=R/'rebuild/compile-gate/native-async-read-begin.json';out.parent.mkdir(parents=True,exist_ok=True);out.write_text(json.dumps(result,indent=2)+'\n')
 print('NATIVE_ASYNC_READ_BEGIN_PASS 10368 34560 34560')
if __name__=='__main__':main()
