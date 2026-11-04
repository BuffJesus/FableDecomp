"""Native engine frame dispatch and x87 world-time accumulation."""
import hashlib,json,subprocess,tempfile
from pathlib import Path
import check_one as V
from gate_game_component_abi import Coff
from variable_allocator_parity import check
from pe_oracle import EXE,pe_sections,va_to_off
import oracle_native_engine_world_frames as oracle
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-engine-world-frames-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/b2/CEngine_WorldFrames_00b24030.cpp';fixture=ROOT/'rebuild/tests/integration/NativeEngineWorldFrames_test.cpp'
specs=[('?WorldFrameUpdate@CEngine@@UAEX_N@Z',0xb24030,143,2),('?PausedWorldFrameUpdate@CEngine@@UAEXXZ',0xb240c0,140,2)]
targets={'__real@4f800000':0x122dcb4,'__real@3f800000':0x122ded8}
aliases={'@WorldFrame@12':specs[0][0],'@PausedFrame@4':specs[1][0]}
(w/'engine_world_frames_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")'for a,b in aliases.items())+'\n')
objects=[]
for i,path in enumerate([source,fixture]):
 obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proof=[check(objects[0],symbol,address,size,count,targets)for symbol,address,size,count in specs];c=Coff(objects[0]);raw=EXE.read_bytes();sections=pe_sections(raw)
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs,expected,report=oracle.collect();(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
actual=(w/'actual.bin').read_bytes()
if actual!=expected:
 (w/'expected.bin').write_bytes(expected);difference=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)));raise AssertionError((w,difference,difference//2060,difference%2060,len(actual),len(expected)))
report.update(providers=proof,omission_checks=1,source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Full2KiB arena, initialization/render guards,0..3 components, live scene masks, replacing component pointers during PeekSceneFilterFlags, mutating frame/time/rate fields and list ends, both update modes and first-frame flags. Counter wrapping and nine unsigned rates throughFFFFFFFF, double signed zero/finite/subnormal/infinity/qNaN/sNaN at53/64-bit x87 precision. Only component virtuals are boundaries; no normalized or masked bytes.')
(ROOT/'rebuild/compile-gate/native-engine-world-frames.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip(),report['snapshots'],'snapshots')
