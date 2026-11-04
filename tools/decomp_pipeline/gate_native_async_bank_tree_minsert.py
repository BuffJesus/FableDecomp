"""Fresh exact pointer-tree _M_insert + independent bounded behavior."""
import hashlib,json,re,struct,subprocess,sys,tempfile
from pathlib import Path
import pefile
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
R=Path(__file__).resolve().parents[2]
source=R/'rebuild/src/compiled/00/9d/STLAsyncBankTree_Minsert_009d7990.cpp'
fixture=R/'rebuild/tests/integration/NativeAsyncBankTreeMinsert_test.py'
required=fixture.with_name('NativeAsyncBankTreeMinsert_required.cpp')
work=Path(tempfile.mkdtemp(prefix='native-async-tree-minsert-'));obj=work/'root.obj'
inputs={source,fixture,required,Path(__file__)};seen=set()
def headers(path):
 if path in seen:return
 seen.add(path);inputs.add(path)
 for name in re.findall(r'^\s*#\s*include\s*"([^"]+)"',path.read_text(errors='replace'),re.M):
  q=next((q for q in (path.parent/name,R/'rebuild/include'/name) if q.is_file()),None);assert q,(path,name);headers(q)
headers(source);headers(required)
for n in ('check_one.py','gate_game_component_abi.py','pe_oracle.py','variable_allocator_parity.py','near_match.py','verify_and_land.py','crawl/rowtrim.py','crawl/purity.py'):inputs.add(R/'tools/decomp_pipeline'/n)
inputs.add(R/'tools/artifact_layout.py')
before={str(p.relative_to(R)):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs}
toolchain_before={str(V.VC/'bin'/n):hashlib.sha256((V.VC/'bin'/n).read_bytes()).hexdigest() for n in ('cl.exe','c1xx.dll','c2.dll','link.exe','mspdb71.dll')}
result=V.cl(['/nologo','/c','/O2','/MT','/I'+str(R/'rebuild/include'),'/Fo'+str(obj),str(source)],V.env(),work)
assert result.returncode==0,result.stdout+result.stderr
raw=EXE.read_bytes();pe=pefile.PE(data=raw)
imports=[(entry.dll.decode(),imp.name.decode() if imp.name else None) for entry in pe.DIRECTORY_ENTRY_IMPORT for imp in entry.imports if imp.address==0x1440158]
assert imports==[('MSVCR71.dll','malloc')],imports
thunk=read_virtual_data(raw,0xbfea0e,6);assert thunk[:2]==b'\xff\x25' and struct.unpack_from('<I',thunk,2)[0]==0x1440158
ins=list(Cs(CS_ARCH_X86,CS_MODE_32).disasm(thunk,0xbfea0e));assert len(ins)==1 and ins[0].mnemonic=='jmp'
report_path=work/'behavior.json'
result=subprocess.run([sys.executable,str(fixture),str(obj),str(report_path)],capture_output=True,text=True)
assert result.returncode==0,result.stdout+result.stderr;print(result.stdout.strip(),flush=True)
report=json.loads(report_path.read_text());assert report['cases']==4800 and report['snapshots']==12000
assert report['executable_cache_invalidated'] is True and report['execution_hits']=={'original':4800,'compiled':4800}
assert report['source_omission']['missing_failed'] and report['source_omission']['provider_link_passed']
assert report['compiled_root_sha256']==report['body_sha256'] and len(report['relocation_audit'])==3
assert all(hashlib.sha256((R/p).read_bytes()).hexdigest()==h for p,h in before.items())
assert all(hashlib.sha256(Path(p).read_bytes()).hexdigest()==h for p,h in toolchain_before.items())
report.update(compile_flags=['/nologo','/c','/O2','/MT'],original_malloc_import={'dll':'MSVCR71.dll','symbol':'malloc','iat':'01440158','thunk':'00BFEA0E','thunk_sha256':hashlib.sha256(thunk).hexdigest()},toolchain_sha256=toolchain_before,fresh_compile_input_sha256=before,gate_marker='NATIVE_ASYNC_BANK_TREE_MINSERT_GATE_PASS')
out=R/'rebuild/compile-gate/native-async-bank-tree-minsert.json';out.parent.mkdir(parents=True,exist_ok=True);out.write_text(json.dumps(report,indent=2)+'\n')
print('NATIVE_ASYNC_BANK_TREE_MINSERT_GATE_PASS',4800,12000,flush=True)
