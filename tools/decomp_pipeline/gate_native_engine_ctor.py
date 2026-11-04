"""Complete typed engine constructor and its virtual table bindings."""
import hashlib,json,subprocess,tempfile
from pathlib import Path
import check_one as V
from gate_game_component_abi import Coff
from variable_allocator_parity import check
import oracle_native_engine_ctor as oracle
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-engine-ctor-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/b2/CEngine_Constructor_00b260b0.cpp';fixture=ROOT/'rebuild/tests/integration/NativeEngineCtor_test.cpp'
ctor='??0CEngine@@QAE@XZ'
targets={'??0CInitBaseClass@@QAE@XZ':0x99a310,'__real@0000000000000000':0x122ed70,'??_7CDeviceResetCallback@@6B@':0x122de50,'??_7CEngine@@6BCIEngine@@@':0x12a0f3c,'??_7CEngine@@6BCDeviceResetCallback@@@':0x12a0f30,'??0CSimpleMemoryPool@@QAE@KK@Z':0xa5c570}
aliases={'@AllocateEngine@0':'?GFAllocEngine@@YIPAVCIEngine@@XZ','@ConstructEngine@4':ctor,'_EngineTable':'??_7CEngine@@6BCIEngine@@@','_EngineDeviceTable':'??_7CEngine@@6BCDeviceResetCallback@@@','??0CInitBaseClass@@QAE@XZ':'@BaseCtorBoundary@4','??0CSimpleMemoryPool@@QAE@KK@Z':'@PoolCtorBoundary@16','??1CEngine@@UAE@XZ':'_UnusedEngineCtorBoundary','??1CIEngine@@UAE@XZ':'_UnusedEngineCtorBoundary'}
tables=json.loads((ROOT/'tools/decomp_pipeline/native_engine_vtables.json').read_text())
for table in tables:
 for entry in table['entries']:
  if not entry['symbol'].startswith('??_E') and entry['symbol']!='__purecall':aliases[entry['symbol']]='_UnusedEngineCtorBoundary'
(w/'engine_ctor_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")'for a,b in aliases.items())+'\n')
objects=[]
for i,path in enumerate([source,fixture]):
 obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proof=check(objects[0],ctor,0xb260b0,280,7,targets)
from pe_oracle import EXE,read_virtual_data
import struct
for table in tables:
 Coff(objects[0]).table(table['symbol'],[e['symbol']for e in table['entries']])
 assert read_virtual_data(EXE.read_bytes(),int(table['retail_address'],16),4*len(table['entries']))==struct.pack('<'+'I'*len(table['entries']),*[int(e['retail_address'],16)for e in table['entries']])
Coff(objects[0]).table('??_7CDeviceResetCallback@@6B@',['__purecall','__purecall'])
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs,expected,report=oracle.collect();(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
actual=(w/'actual.bin').read_bytes()
if actual!=expected:
 (w/'expected.bin').write_bytes(expected);difference=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)));raise AssertionError((w,difference,difference//4112,difference%4112,len(actual),len(expected)))
report.update(providers=[proof],omission_checks=1,source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Full4KiB snapshots with all256fills and eight base/pool mutation profiles. Inline vector, preload statistics, video defaults and six def pointers execute. Base and pool construction boundaries recorded. Two known compiled vtable identities mapped; no state bytes masked. Derived and base tables audited against173 PDB-identified donor slots and retail targets. No virtual method behavior claimed.')
allocation=ROOT/'rebuild/src/compiled/00/b2/Global_GFAllocEngine_00b26340.cpp'
allocobj=w/'allocation.obj';composedfixture=w/'composed-fixture.obj'
for path,obj,extra in [(allocation,allocobj,[]),(fixture,composedfixture,['/DCOMPOSE_ALLOCATION'])]:
 r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(path)]+extra,env,w);assert not r.returncode,r.stdout+r.stderr
composedobjects=[objects[0],allocobj,composedfixture]
for omit in (0,1):
 r=subprocess.run(cmd+[str(o)for i,o in enumerate(composedobjects)if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,composedobjects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs,expected,composition=oracle.collect(True);(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
assert (w/'actual.bin').read_bytes()==expected,w
composition['omission_checks']=2;report['allocation_composition']=composition
(ROOT/'rebuild/compile-gate/native-engine-ctor.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_ENGINE_CTOR_PASS cases=%d snapshots=%d allocation_cases=%d allocation_snapshots=%d'%(report['cases'],report['snapshots'],composition['cases'],composition['snapshots']))
