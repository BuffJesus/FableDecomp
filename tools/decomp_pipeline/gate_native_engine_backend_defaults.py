"""Fourteen concrete PC engine defaults with actual typed symbols and full-state parity."""
import hashlib,json,subprocess,tempfile
from pathlib import Path
import check_one as V
from gate_game_component_abi import Coff
from variable_allocator_parity import check
from pe_oracle import EXE,pe_sections,va_to_off
import oracle_native_engine_backend_defaults as oracle
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-engine-backend-defaults-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/b2/CEngine_BackendDefaults_00b23cf0.cpp';fixture=ROOT/'rebuild/tests/integration/NativeEngineBackendDefaults_test.cpp'
specs=oracle.SPECS
targets={}
aliases={'@Default%d@%d'%(e['slot'],8+4*e['words']):e['symbol']for e in specs}
(w/'engine_backend_defaults_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")'for a,b in aliases.items())+'\n')
objects=[]
for i,path in enumerate([source,fixture]):
 obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
c=Coff(objects[0]);proof=[]
for e in specs:
 sec,off=c.symbols[e['symbol']];body,rel=c.sections[sec]
 proof.append(check(objects[0],e['symbol'],int(e['retail_address'],16),e['size'],len(rel),targets))
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs,expected,report=oracle.collect();(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
actual=(w/'actual.bin').read_bytes()
if actual!=expected:
 (w/'expected.bin').write_bytes(expected);difference=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)));raise AssertionError((w,difference,difference//1032,difference%1032,len(actual),len(expected)))
report.update(providers=proof,omission_checks=1,source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Fourteen typed PC engine defaults, all256 byte fills and four argument patterns. Full1024-byte arena, active flag write, boolean/scalar return values, untouched projection output and unrelated storage. No callbacks or normalized bytes. Empty reference-parameter methods receive opaque ABI words and never dereference them.')
(ROOT/'rebuild/compile-gate/native-engine-backend-defaults.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip(),report['snapshots'],'snapshots')
