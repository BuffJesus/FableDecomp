"""Display teardown using typed engine-manager ABI and native member lifetimes."""
import hashlib,json,subprocess,tempfile
from pathlib import Path
import check_one as V
from gate_game_component_abi import Coff
from variable_allocator_parity import check
import oracle_native_display_engine_dtor as oracle
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-display-engine-dtor-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/43/CDisplayEngine_Destructor_004354c0.cpp';fixture=ROOT/'rebuild/tests/integration/NativeDisplayEngineDtor_test.cpp'
uninit='?UninitEngine@CDisplayEngine@@MAEXXZ';dtor='??1CDisplayEngine@@UAE@XZ';scalar='??_GCDisplayEngine@@UAEPAXI@Z';get='?Get@?$CTBaseSingleton@VCEngineManager@@@@SIAAVCEngineManager@@XZ';remove='?RemoveEngine@CEngineManager@@QAEXAAVCIEngine@@@Z';delete='?DeleteData@?$CCountedPointer@VCDisplayViewManager@@@@CIXPAX@Z'
targets={'??_7CDisplayEngine@@6B@':0x1231574,get:0x436fb0,remove:0x6404b0,'??3@YAXPAX@Z':0xbfe9bc,'??1CBaseClassNonCopyable@@UAE@XZ':0x99a300,dtor:0x4354c0}
aliases={'@UninitDisplayEngine@4':uninit,'@DestroyDisplayEngine@4':dtor,'@DeleteDisplayEngine@12':scalar,'@DeleteView@4':delete,'_DisplayEngineTable':'??_7CDisplayEngine@@6B@','_BaseTable':'??_7CBaseClass@@6B@','_NonCopyTable':'??_7CBaseClassNonCopyable@@6B@',get:'@GetManagerBoundary@0',remove:'@RemoveEngineBoundary@12','?InitEngine@CDisplayEngine@@MAE_NXZ':'_UnusedDisplayEngineDtorBoundary'}
(w/'display_engine_dtor_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")'for a,b in aliases.items())+'\n')
paths=[source,ROOT/'rebuild/src/compiled/00/43/CDisplayViewManager_Ownership_00436cc0.cpp',ROOT/'rebuild/src/compiled/00/99/CBaseClass_CBaseClass_0099a2d0.cpp',fixture];objects=[]
for i,path in enumerate(paths):
 obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proof=[check(objects[0],uninit,0x4354a0,29,2,targets),check(objects[0],dtor,0x4354c0,105,5,targets),check(objects[0],scalar,0x437070,30,2,targets)]
Coff(objects[0]).table('??_7CDisplayEngine@@6B@',['??_ECDisplayEngine@@UAEPAXI@Z','?InitEngine@CDisplayEngine@@MAE_NXZ',uninit])
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omit in range(3):
 r=subprocess.run(cmd+[str(o)for i,o in enumerate(objects)if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs,expected,report=oracle.collect();(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
actual=(w/'actual.bin').read_bytes()
if actual!=expected:
 (w/'expected.bin').write_bytes(expected);difference=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)));raise AssertionError((w,difference,difference//4108,difference%4108,len(actual),len(expected)))
report.update(providers=proof,omission_checks=3,source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual UninitEngine, destructor/scalar delete, nested view-owner deletion and base lifetime. Full4KiB snapshots with null/live ownership, wrapping refs, flags0/1/2/FFFFFFFF and mutating callbacks. Engine Release, singleton Get, manager RemoveEngine, nested view callback and operator delete are recorded boundaries. Actual vtable and callback identities map without masking other bytes.')
(ROOT/'rebuild/compile-gate/native-display-engine-dtor.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip(),report['snapshots'],'snapshots')
