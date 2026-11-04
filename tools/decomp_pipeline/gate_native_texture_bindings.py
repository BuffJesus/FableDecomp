import json,subprocess,tempfile
from pathlib import Path
import check_one as V
from variable_allocator_parity import check
import oracle_native_texture_bindings as oracle
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-texture-bindings-'));env=V.env();objects=[]
paths=['rebuild/src/compiled/00/9f/CTexture_Uninitialise_009f9f70.cpp','rebuild/src/compiled/00/9f/CTexture_Assignment_009fa1c0.cpp',
       'rebuild/src/compiled/00/9a/CRenderManagerCore_AttachTextureToStage_009a0cf0.cpp','rebuild/tests/integration/NativeTextureBindings_test.cpp']
for i,path in enumerate(paths):
 obj=w/('%d.obj'%i);r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
reports=[check(objects[0],'?Uninitialise@CTexture@@QAEXXZ',0x9f9f70,34,0,{}),
         check(objects[1],'??4CTexture@@QAEABV0@ABV0@@Z',0x9fa1c0,98,0,{}),
         check(objects[2],'?AttachTextureToStage@CRenderManagerCore@@QAEXJPBVCTexture@@@Z',0x9a0cf0,79,0,{})]
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omit in range(3):
 r=subprocess.run(cmd+[str(o)for i,o in enumerate(objects)if i!=omit],env=env,capture_output=True,text=True);assert r.returncode,omit
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs,expected,report=oracle.collect();(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
actual=(w/'actual.bin').read_bytes()
if actual!=expected:
 (w/'expected.bin').write_bytes(expected);at=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)));raise AssertionError((w,at,at//(oracle.WIDTH+20),at%(oracle.WIDTH+20)))
report.update(providers=reports,omission_checks=3,boundary='Actual native assignment, repeated cached binding, Uninitialise, null rebinding and source cleanup. Full20KiB arena at every COM callback and return. Null/live/self assignment; cache hits/misses; stage0/1/3/7; prior stage counts; mutating callbacks and COM failure. Only AddRef/Release/SetTexture are recorded boundaries.')
(ROOT/'rebuild/compile-gate/native-texture-bindings.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip(),report['snapshots'],'full-state snapshots',flush=True)
