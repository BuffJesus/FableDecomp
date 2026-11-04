"""Native nonsorted primitive-manager drawing dispatch."""
import hashlib,json,re,subprocess,tempfile
from pathlib import Path
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2]
w=Path(tempfile.mkdtemp(prefix='native-primitive-frame-end-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/b3/CEnginePrimitiveRenderer_FrameEnd_00b32820.cpp'
fixture=ROOT/'rebuild/tests/integration/NativePrimitiveFrameEnd_test.cpp'
symbol='?FrameEnd@CEnginePrimitiveRenderer@@UAEXABVCEngineComponentParams@@@Z'
raw=read_virtual_data(EXE.read_bytes(),0xb32820,158)
(w/'primitive_frame_end_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,raw))+'};\n#pragma comment(linker,"/alternatename:@EndFrame@12='+symbol+'")\n')
with (w/'primitive_frame_end_retail.h').open('a') as f:
 f.write('#pragma comment(linker,"/alternatename:?FrameEnd@CEngineScene@@QAEXXZ=@SceneEnd@8")\n#pragma comment(linker,"/alternatename:?ResetStats@CEngineScene@@QAEXXZ=@SceneReset@8")\n')
objects=[]
for n,p in enumerate((source,fixture)):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proof=check(objects[0],symbol,0xb32820,158,2,{'?FrameEnd@CEngineScene@@QAEXXZ':0xb94a30,'?ResetStats@CEngineScene@@QAEXXZ':0xb93df0})
link=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
r=subprocess.run(link+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode,(r.returncode,r.stdout,r.stderr)
match=re.search(r'NATIVE_PRIMITIVE_FRAME_END_PASS cases=(\d+) snapshots=(\d+)',r.stdout);assert match and int(match[1])==2304;snapshots=int(match[2]);rpt=dict(cases=2304,snapshots=snapshots,omission_checks=1,providers=[proof],source_sha256={str(source.relative_to(ROOT)):hashlib.sha256(source.read_bytes()).hexdigest()},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),headers={name:hashlib.sha256((ROOT/'rebuild/include'/name).read_bytes()).hexdigest() for name in ('engine/CEnginePrimitiveRenderer.h','fable_engine_primitive_manager.h')},boundary='Actual FrameEnd invoked through canonical CEngineComponent second base at268 and virtualslot2. Full4KiB independent model; scene FrameEnd, scene ResetStats and primitive-manager FrameEnd callbacks. Zero-to-three scene/manager counts, scene iteration truncation, manager growth tofive and storage replacement, changed scene array before stats phase, repeated receivers, preserved params and final draw-distance-invalid reset.')
(ROOT/'rebuild/compile-gate/native-primitive-frame-end.json').write_text(json.dumps(rpt,indent=2)+'\n')
print(r.stdout.strip());print('NATIVE_PRIMITIVE_FRAME_END_GATE_PASS',snapshots,'snapshots')
