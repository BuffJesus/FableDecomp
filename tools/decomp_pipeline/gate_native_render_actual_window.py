import json,subprocess,tempfile
from pathlib import Path
import check_one as V
from variable_allocator_parity import check
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-render-actual-window-'));env=V.env();objects=[]
for i,path in enumerate(['rebuild/src/compiled/00/a0/CRenderManagerCore_SetAWindow_00a0aa80.cpp','rebuild/tests/integration/NativeRenderActualWindow_test.cpp']):
 obj=w/('%d.obj'%i);r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
targets={'?SetViewport@CDisplayManager@@QAEXABVC2DBoxF@@@Z':0x9bf490,'?SetIntegerViewportEndpoint@CDisplayManager@@QAEXABVC2DBoxI@@@Z':0x9bef80}
reports=[check(objects[0],'?SetAWindow@CRenderManagerCore@@QAEXABVC2DBoxF@@@Z',0xa0aa80,11,1,targets),
         check(objects[0],'?SetAWindow@CRenderManagerCore@@QAEXABVC2DBoxI@@@Z',0xa0aa90,11,1,targets)]
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'PASS scenarios=512' in r.stdout,r.stdout+r.stderr
report=dict(providers=reports,scenarios=512,omission_checks=1,boundary='PDB-identified float/integer SetAWindow overloads. Linked fixture checks exact receiver/reference forwarding and one call per invocation, including callbacks replacing the core display pointer. Display endpoints remain boundaries; complete native wrapper bytes audited against retail.')
(ROOT/'rebuild/compile-gate/native-render-actual-window.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip(),flush=True)
