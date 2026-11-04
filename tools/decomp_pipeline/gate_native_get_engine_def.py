"""Compile and compare the actual engine-definition lookup with retail."""
import hashlib,json,subprocess,tempfile
from pathlib import Path
import check_one as V
from variable_allocator_parity import check
import oracle_native_get_engine_def as oracle
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-get-engine-def-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/43/CDefinitionManager_GetDefVCEngineDef_00430096.cpp'
fixture=ROOT/'rebuild/tests/integration/NativeGetEngineDef_test.cpp'
aliases={'@GetEngineDef@20':oracle.SYMBOL,oracle.LOOKUP:'@LookupBoundary@20'}
(w/'get_engine_def_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")'for a,b in aliases.items())+'\n')
objects=[]
for i,path in enumerate([source,fixture]):
 obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proof=check(objects[0],oracle.SYMBOL,0x430096,101,1,{oracle.LOOKUP:0x9ade10})
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs,expected,report=oracle.collect();(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
actual=(w/'actual.bin').read_bytes()
if actual!=expected:
 (w/'expected.bin').write_bytes(expected);difference=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)));raise AssertionError((w,difference,difference//2064,difference%2064,len(actual),len(expected)))
report.update(providers=[proof],omission_checks=1,source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Full2048-byte snapshots; no mapped or masked arena bytes. Class-index lookup and virtual pointee deletion are recorded boundaries. Hidden result contents captured during callbacks. Signed indices, null lookup/output, all256fills, shared and distinct owners, count wraparound, output/temporary/count mutation during deletion. Callee-saved registers and stack cleanup checked in retail execution.')
(ROOT/'rebuild/compile-gate/native-get-engine-def.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip(),report['snapshots'],'snapshots')
