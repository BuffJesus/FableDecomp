"""Complete typed display-engine constructor and its virtual table bindings."""
import hashlib,json,subprocess,tempfile
from pathlib import Path
import check_one as V
from gate_game_component_abi import Coff
from variable_allocator_parity import check
import oracle_native_display_engine_ctor as oracle
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-display-engine-ctor-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/43/CDisplayEngine_Constructor_00434e10.cpp';fixture=ROOT/'rebuild/tests/integration/NativeDisplayEngineCtor_test.cpp'
ctor='??0CDisplayEngine@@QAE@ABVCDisplayEngineInit@@@Z'
targets={'??0CBaseClassNonCopyable@@QAE@XZ':0x99a2f0,'??_7CDisplayEngine@@6B@':0x1231574,'??0CCamera@@QAE@ABVC3DVector@@0M_N@Z':0xa0bf20}
aliases={'@ConstructDisplayEngine@12':ctor,'_DisplayEngineTable':'??_7CDisplayEngine@@6B@','??0CBaseClassNonCopyable@@QAE@XZ':'@BaseCtorBoundary@4','??0CCamera@@QAE@ABVC3DVector@@0M_N@Z':'@CameraCtorBoundary@24'}
for symbol in ['??1CDisplayEngine@@UAE@XZ','?InitEngine@CDisplayEngine@@MAE_NXZ','?UninitEngine@CDisplayEngine@@MAEXXZ']:aliases[symbol]='_UnusedDisplayEngineCtorBoundary'
(w/'display_engine_ctor_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")'for a,b in aliases.items())+'\n')
objects=[]
for i,path in enumerate([source,fixture]):
 obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proof=check(objects[0],ctor,0x434e10,334,3,targets)
Coff(objects[0]).table('??_7CDisplayEngine@@6B@',['??_ECDisplayEngine@@UAEPAXI@Z','?InitEngine@CDisplayEngine@@MAE_NXZ','?UninitEngine@CDisplayEngine@@MAEXXZ'])
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs,expected,report=oracle.collect();(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
actual=(w/'actual.bin').read_bytes()
if actual!=expected:
 (w/'expected.bin').write_bytes(expected);difference=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)));raise AssertionError((w,difference,difference//4136,difference%4136,len(actual),len(expected)))
report.update(providers=[proof],omission_checks=1,source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Full256-byte typed display engine, full4KiB snapshots with all byte fills, four input patterns and base/camera mutations. Camera arguments and receiver recorded; inline fade, colour and counted-owner construction executes. Only compiled vtable identity mapped. No engine allocation, initialization or rendered-frame claim.')
(ROOT/'rebuild/compile-gate/native-display-engine-ctor.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip(),report['snapshots'],'snapshots')
