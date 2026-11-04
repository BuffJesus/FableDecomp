import json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
import oracle_native_render_capture_draw as oracle
w=Path(tempfile.mkdtemp(prefix='native-render-capture-draw-'));env=V.env();objects=[]
sources=['rebuild/src/compiled/00/a0/CRenderStateManager_RestoreCaptureBlock_00a05840.cpp',
         'rebuild/src/compiled/00/a0/CRenderManagerCore_DrawAsIndexedTris_00a0af40.cpp',
         'rebuild/src/compiled/00/a0/CRenderStateManager_RealiseRenderState_00a058c0.cpp',
         'rebuild/tests/integration/NativeRenderCaptureDraw_test.cpp']
for i,source in enumerate(sources):
    obj=w/('%d.obj'%i);r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/source)],env,w);assert not r.returncode,r.stdout+r.stderr
    objects.append(obj)
targets={'?RealiseRenderState@CRenderStateManager@@QAEXXZ':0xa058c0}
reports=[check(objects[0],'?RestoreCaptureBlock@CRenderStateManager@@QAEXXZ',0xa05840,121,0,targets),
         check(objects[1],'?DrawAsIndexedTris@CRenderManagerCore@@QAEXPBXJJPAXJ@Z',0xa0af40,58,1,targets)]
support=check(objects[2],'?RealiseRenderState@CRenderStateManager@@QAEXXZ',0xa058c0,167,0,targets)
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omit in range(3):
    r=subprocess.run(cmd+[str(o)for i,o in enumerate(objects)if i!=omit],env=env,capture_output=True,text=True);assert r.returncode,omit
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs,expected,report=oracle.collect();(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=60)
assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
actual=(w/'actual.bin').read_bytes()
if actual!=expected:
    (w/'expected.bin').write_bytes(expected)
    difference=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)))
    raise AssertionError((w,difference,difference//(oracle.WIDTH+40),difference%(oracle.WIDTH+40)))
report.update(providers=reports,support=support,omission_checks=3,boundary='Two nested capture restores, each followed by actual native realization and indexed UP draw. Zero to eight saved states, prior dirty entries, equal current values, all state kinds and bookmark depths; COM failures and callbacks mutate desired state and device pointers. Exact call arguments and complete 24KiB arena compared. No GPU rendering.')
(ROOT/'rebuild/compile-gate/native-render-capture-draw.json').write_text(json.dumps(report,indent=2)+'\n')
print(r.stdout.strip(),report['snapshots'],'full-state snapshots',flush=True)
