"""Engine-init defaults, five path lifetimes and composed native empty strings."""
import hashlib,json,subprocess,tempfile
from pathlib import Path
import check_one as V
from variable_allocator_parity import check
import oracle_native_engine_init_lifetime as oracle
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-engine-init-lifetime-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/42/CEngineInit_Lifetime_0042d830.cpp';fixture=ROOT/'rebuild/tests/integration/NativeEngineInitLifetime_test.cpp'
count_source=ROOT/'rebuild/src/compiled/01/3b/WideStringInstanceCount_013bca20.cpp'
aliases={'@ConstructInit@4':'??0CEngineInit@@QAE@XZ','@DestroyInit@4':'??1CEngineInit@@QAE@XZ','??0CWideString@@QAE@XZ':'@WideConstruct@4','??1CWideString@@QAE@XZ':'@WideDestroy@4','_WideCount':'?g_CWideStringInstanceCount_013BCA20@@3HA'}
def header():
 (w/'engine_init_lifetime_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")'for a,b in aliases.items())+'\n')
def compile(path,obj):
 r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(path)],env,w);assert not r.returncode,r.stdout+r.stderr
header();objects=[]
for i,path in enumerate([source,fixture,count_source]):
 obj=w/(str(i)+'.obj');compile(path,obj);objects.append(obj)
targets={'??0CWideString@@QAE@XZ':0x99aed0,'??1CWideString@@QAE@XZ':0x99b510}
proof=[check(objects[0],'??0CEngineInit@@QAE@XZ',0x42d830,122,5,targets),check(objects[0],'??1CEngineInit@@QAE@XZ',0x42f7c1,44,5,targets)]
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
def link(items,success=True):
 r=subprocess.run(cmd+list(map(str,items)),env=env,capture_output=True,text=True);assert (not r.returncode)==success,r.stdout+r.stderr
def replay(composed):
 inputs,expected,report=oracle.collect(composed);(w/'input.bin').write_bytes(inputs)
 r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
 actual=(w/'actual.bin').read_bytes()
 if actual!=expected:
  (w/'expected.bin').write_bytes(expected);difference=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)));raise AssertionError((w,composed,difference,difference//268,difference%268,len(actual),len(expected)))
 return report
for omit in (0,2):link([o for i,o in enumerate(objects)if i!=omit],False)
link(objects);report=replay(False)
del aliases['??0CWideString@@QAE@XZ'];del aliases['??1CWideString@@QAE@XZ'];header();compile(fixture,objects[1])
string_sources=['rebuild/src/compiled/00/99/CWideString_Constructor_0099aed0.cpp','rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp']
for i,path in enumerate(string_sources):
 obj=w/('string'+str(i)+'.obj');compile(ROOT/path,obj);objects.append(obj)
for omit in (3,4):link([o for i,o in enumerate(objects)if i!=omit],False)
link(objects);composed=replay(True)
report.update(providers=proof,omission_checks=2,composed=composed,composed_omission_checks=2,source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Full256-byte arena plus instance count. Every fill; construction/destruction/combined operations and callbacks modifying prior/future members. Composed mode executes actual five wide constructors and destructors, preserving instance count. No normalized bytes.')
(ROOT/'rebuild/compile-gate/native-engine-init-lifetime.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_ENGINE_INIT_LIFETIME_PASS cases='+str(report['cases']),report['snapshots'],'snapshots; composed',composed)
