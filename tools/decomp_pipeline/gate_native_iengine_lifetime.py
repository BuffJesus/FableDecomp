"""Engine interface lifecycle and actual scalar-release composition."""
import hashlib,json,subprocess,tempfile
from pathlib import Path
import check_one as V
from gate_game_component_abi import Coff
from variable_allocator_parity import check
from pe_oracle import EXE,pe_sections,va_to_off
import oracle_native_iengine_lifetime as oracle
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-iengine-lifetime-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/b2/CIEngine_Lifetime_00b232d0.cpp';fixture=ROOT/'rebuild/tests/integration/NativeIEngineLifetime_test.cpp'
specs=[('?Initialise@CIEngine@@UAE_NABVCEngineInit@@@Z',0xb232d0,8,1),('?Release@CIEngine@@UAEXXZ',0xb232e0,11,0),('?SetSeaBodyToRender@CIEngine@@UAEXVCCharString@@@Z',0xb23610,12,1),('??_GCIEngine@@UAEPAXI@Z',0xb23770,36,3)]
targets={'?Initialise@CInitBaseClass@@IAE_NXZ':0x99a350,'??1CCharString@@QAE@XZ':0x99eae0,'??_7CIEngine@@6B@':0x12a0d24,'??1CInitBaseClass@@UAE@XZ':0x99a410,'??3@YAXPAX@Z':0xbfe9bc}
aliases={'@InitialiseEngine@12':specs[0][0],'@ReleaseEngine@4':specs[1][0],'@SetSeaBody@12':specs[2][0],'@ScalarDeleteEngine@12':specs[3][0],'_IEngineTable':'??_7CIEngine@@6B@','?Initialise@CInitBaseClass@@IAE_NXZ':'@InitBoundary@4','??1CCharString@@QAE@XZ':'@StringDtorBoundary@4','??1CInitBaseClass@@UAE@XZ':'@BaseDtorBoundary@4'}
table=json.loads((ROOT/'tools/decomp_pipeline/native_engine_vtables.json').read_text())[-1]
for e in table['entries']:
 if e['symbol']not in [s[0]for s in specs]and e['symbol']!='__purecall'and not e['symbol'].startswith('??_E'):aliases[e['symbol']]='_UnusedIEngineLifetimeBoundary'
(w/'iengine_lifetime_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")'for a,b in aliases.items())+'\n')
objects=[]
for i,path in enumerate([source,fixture]):
 obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proof=[check(objects[0],symbol,address,size,count,targets)for symbol,address,size,count in specs]
Coff(objects[0]).table(table['symbol'],[e['symbol']for e in table['entries']])
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs,expected,report=oracle.collect();(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
actual=(w/'actual.bin').read_bytes()
if actual!=expected:
 (w/'expected.bin').write_bytes(expected);difference=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)));raise AssertionError((w,difference,difference//528,difference%528,len(actual),len(expected)))
report.update(providers=proof,omission_checks=1,source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Full512-byte snapshots, all256fills, four lifecycle operations, four parameters and eight mutation modes. Init result forwarding, null/live release, actual scalar-delete composition, flags0/1/2/FFFFFFFF, local by-value string cleanup and pointee mutation. Base init/destructor, char destructor and heap deletion are boundaries; known base vtable identity and local parameter object identity mapped. No state bytes masked.')
(ROOT/'rebuild/compile-gate/native-iengine-lifetime.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip(),report['snapshots'],'snapshots')
