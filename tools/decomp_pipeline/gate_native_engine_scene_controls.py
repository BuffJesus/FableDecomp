"""Native scene/preload transitions and render-enable bit controls."""
import hashlib,json,subprocess,tempfile
from pathlib import Path
import check_one as V
from gate_game_component_abi import Coff
from variable_allocator_parity import check
from pe_oracle import EXE,pe_sections,va_to_off
import oracle_native_engine_scene_controls as oracle
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-engine-scene-controls-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/b2/CEngine_SceneControls_00b239a0.cpp';fixture=ROOT/'rebuild/tests/integration/NativeEngineSceneControls_test.cpp'
entries=json.loads((ROOT/'tools/decomp_pipeline/native_engine_scene_controls.json').read_text())
specs=[(e['symbol'],int(e['retail_address'],16),e['size'],0)for e in entries]
targets={}
names=['@StartScene@16','@Preload@16','@IsPreloaded@4','@Enable3D@12','@Is3D@4','@Enable2D@12','@Is2D@4']
aliases={name:e['symbol']for name,e in zip(names,entries)}
(w/'engine_scene_controls_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")'for a,b in aliases.items())+'\n')
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
 (w/'expected.bin').write_bytes(expected);difference=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)));raise AssertionError((w,difference,difference//520,difference%520,len(actual),len(expected)))
report.update(providers=proof,omission_checks=1,source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Full512-byte snapshots across ten-step sequences. New-scene and cutscene preload guards, normal/fast timeout selection, unused second preload flag, independent2D/3D masks, all other flag bits preserved, and float signed-zero/finite/subnormal/infinity/qNaN/sNaN bit copies. No external boundaries or normalized bytes.')
(ROOT/'rebuild/compile-gate/native-engine-scene-controls.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip(),report['snapshots'],'snapshots')
