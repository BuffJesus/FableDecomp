"""Native bank reader, full-body parity and packed/file callback state replay."""
import hashlib,json,subprocess,tempfile
from pathlib import Path
import check_one as V
from variable_allocator_parity import check
import oracle_native_shader_shutdown as oracle
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-shader-shutdown-'));env=V.env();objects=[]
for i,name in enumerate(['rebuild/src/compiled/00/98/CShaderRenderManager_Uninitialise_0098a7a0.cpp','rebuild/src/compiled/00/a5/CShaderResource_ConstructionAndClose_00a5d870.cpp','rebuild/tests/integration/NativeShaderShutdown_test.cpp']):
 obj=w/('%d.obj'%i);r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/name)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
targets={'?Uninitialise@CShaderResource@@AAEXXZ':0xa5d7a0,'?CloseShaderBank@CShaderResource@@QAEXXZ':0xa5d750}
proof=[check(objects[0],'?Uninitialise@CShaderRenderManager@@QAEXXZ',0x98a7a0,79,1,targets),check(objects[1],'?Uninitialise@CShaderResource@@AAEXXZ',0xa5d7a0,17,1,targets),check(objects[1],'?CloseShaderBank@CShaderResource@@QAEXXZ',0xa5d750,69,0,targets)]
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omit in [0,1]:
 r=subprocess.run(cmd+[str(o)for i,o in enumerate(objects)if i!=omit],env=env,capture_output=True,text=True);assert r.returncode,('provider required',omit)
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs,expected,report=oracle.collect();(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
actual=(w/'actual.bin').read_bytes()
if actual!=expected:
 (w/'expected.bin').write_bytes(expected);difference=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)));raise AssertionError((w,difference,difference//(oracle.WIDTH+12),difference%(oracle.WIDTH+12)))
report.update(providers=proof,omission_checks=2,compared_bytes=len(actual),boundary='Actual79-byte shader-manager shutdown,17-byte resource uninitialization and69-byte bank close compose together. Full16KiB at every shader virtual deletion and return, no masked bytes. Empty/live/mixed shader arrays, zero to six resources, callbacks replacing/shrinking/growing manager storage and replacing resource shader storage. Final device reset occurs after all cleanup; counted owners remain untouched. Virtual shader deletion is the only recorded boundary.')
(ROOT/'rebuild/compile-gate/native-shader-shutdown.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip(),report['snapshots'],'snapshots')
