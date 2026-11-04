"""Concrete engine teardown with the native pool destructor and block traversal."""
import hashlib,json,subprocess,tempfile
from pathlib import Path
import check_one as V
from gate_game_component_abi import Coff
from variable_allocator_parity import check
from pe_oracle import EXE,pe_sections,va_to_off
import oracle_native_engine_dtor as oracle
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-engine-dtor-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/b2/CEngine_Destructor_00b261d0.cpp';fixture=ROOT/'rebuild/tests/integration/NativeEngineDtor_test.cpp'
specs=[('??1CEngine@@UAE@XZ',0xb261d0,238,10),('??_GCEngine@@UAEPAXI@Z',0xb2ab10,30,2),('?Release@CEngine@@UAEXXZ',0xb23990,11,0)]
targets={'??_7CEngine@@6BCIEngine@@@':0x12a0f3c,'??_7CEngine@@6BCDeviceResetCallback@@@':0x12a0f30,'?Uninitialise@CEngine@@UAEXXZ':0xb25d80,'_free':0xbfea14,'??1CSimpleMemoryPool@@UAE@XZ':0x987f10,'??_7CIEngine@@6B@':0x12a0d24,'??1CInitBaseClass@@UAE@XZ':0x99a410,'??1CEngine@@UAE@XZ':0xb261d0,'??3@YAXPAX@Z':0xbfe9bc}
aliases={'@ReleaseEngine@4':specs[2][0],'@DestroyEngine@4':specs[0][0],'@DeleteEngine@12':specs[1][0],'_EngineTable':'??_7CEngine@@6BCIEngine@@@','_EngineDeviceTable':'??_7CEngine@@6BCDeviceResetCallback@@@','_IEngineTable':'??_7CIEngine@@6B@','_PoolTable':'??_7CSimpleMemoryPool@@6B@','?Uninitialise@CEngine@@UAEXXZ':'@UninitialiseBoundary@4','??1CInitBaseClass@@UAE@XZ':'@InitBaseDtorBoundary@4','??1CBaseClassNonCopyable@@UAE@XZ':'@PoolBaseDtorBoundary@4','??0CBaseClassNonCopyable@@QAE@XZ':'_UnusedEngineDtorBoundary'}
tables=json.loads((ROOT/'tools/decomp_pipeline/native_engine_vtables.json').read_text())
for table in tables:
 for e in table['entries']:
  if e['symbol']not in [s[0]for s in specs] and e['symbol']not in aliases and e['symbol']!='__purecall'and not e['symbol'].startswith('??_E'):aliases[e['symbol']]='_UnusedEngineDtorBoundary'
(w/'engine_dtor_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")'for a,b in aliases.items())+'\n')
objects=[]
pool=ROOT/'rebuild/src/compiled/00/a5/CSimpleMemoryPool_Lifetime_00a5c570.cpp'
original=w/'original.obj';r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(original),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr
for i,path in enumerate([source,pool,fixture]):
 obj=w/(str(i)+'.obj');options=['/Dfree=EngineDtorFree']if i==0 else []
 r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(path)]+options,env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
a,b=Coff(original),Coff(objects[0])
for symbol,(sec,off)in a.symbols.items():
 if not symbol.startswith('?'):continue
 other,otheroff=b.symbols[symbol];body,rel=a.sections[sec];otherbody,otherrel=b.sections[other];assert off==otheroff and body==otherbody and {p:(t.replace('_free','_EngineDtorFree'),k)for p,(t,k)in rel.items()}==otherrel
proof=[check(original,symbol,address,size,count,targets)for symbol,address,size,count in specs]
for table in tables:Coff(original).table(table['symbol'],[e['symbol']for e in table['entries']])
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omit in (0,1):
 r=subprocess.run(cmd+[str(o)for i,o in enumerate(objects)if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs,expected,report=oracle.collect();(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
actual=(w/'actual.bin').read_bytes()
if actual!=expected:
 (w/'expected.bin').write_bytes(expected);difference=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)));raise AssertionError((w,difference,difference//4108,difference%4108,len(actual),len(expected)))
report.update(providers=proof,omission_checks=2,source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Full4KiB snapshots. Actual pool destructor and block traversal; six definition releases, shared/unique/null pointers, references0/1/2/FFFFFFFF, three vector buffers plus camera storage, four fills, mutating callbacks and scalar flags0/1/2/FFFFFFFF, plus null/live Release through the actual virtual scalar destructor. Uninitialise, heap free/delete, virtual block/definition deletes and base destructors remain boundaries. Four known vtable identities mapped; no state bytes masked. Source free renaming is verified code/relocation-identical.')
(ROOT/'rebuild/compile-gate/native-engine-dtor.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip(),report['snapshots'],'snapshots')
