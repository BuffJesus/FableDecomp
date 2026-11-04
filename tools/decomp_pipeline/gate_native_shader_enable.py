import json, subprocess, sys, tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
import oracle_native_shader_enable as oracle
from variable_allocator_parity import check
w=Path(tempfile.mkdtemp(prefix='native-shader-enable-'))
env=V.env()
targets={'?GetPixelShaderVersion@CDisplayManager@@QBEKXZ':0x9bee00,
         '?EnableXBoxShaderConstants@CRenderManagerCore@@QAEX_N@Z':0xa0aa20}
specs=[('00/98/CShaderRenderManager_EnablePixelShaders_00988110.cpp','?EnablePixelShaders@CShaderRenderManager@@QAEXXZ',0x988110,36,1),
       ('00/9b/CDisplayManager_GetPixelShaderVersion_009bee00.cpp','?GetPixelShaderVersion@CDisplayManager@@QBEKXZ',0x9bee00,29,0),
       ('00/98/CShaderRenderManager_EnableVertexShaders_00987fe0.cpp','?EnableVertexShaders@CShaderRenderManager@@QAEXXZ',0x987fe0,61,1),
       ('00/98/CShaderRenderManager_DisableVertexShaders_009880e0.cpp','?DisableVertexShaders@CShaderRenderManager@@QAEXXZ',0x9880e0,44,1),
       ('00/a0/CRenderManagerCore_EnableXBoxShaderConstants_00a0aa20.cpp','?EnableXBoxShaderConstants@CRenderManagerCore@@QAEX_N@Z',0xa0aa20,3,0),
       ('00/98/CShaderRenderManager_DisablePixelShaders_00988190.cpp','?DisablePixelShaders@CShaderRenderManager@@QAEXXZ',0x988190,85,0)]
objects=[];reports=[]
for i,(name,symbol,address,length,count) in enumerate(specs):
    obj=w/('%d.obj'%i)
    r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/'rebuild/src/compiled'/name)],env,w)
    assert not r.returncode,r.stdout+r.stderr
    reports.append(check(obj,symbol,address,length,count,targets));objects.append(obj)
    print(reports[-1],flush=True)
obj=w/'fixture.obj'
r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/'rebuild/tests/integration/NativeShaderEnable_test.cpp')],env,w)
assert not r.returncode,r.stdout+r.stderr
objects.append(obj)
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omit in range(6):
    r=subprocess.run(cmd+[str(o)for i,o in enumerate(objects)if i!=omit],env=env,capture_output=True,text=True)
    assert r.returncode,('omission did not fail',omit)
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True)
assert not r.returncode,r.stdout+r.stderr
inputs,expected,report=oracle.collect();(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=60)
assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
actual=(w/'actual.bin').read_bytes()
if actual!=expected:
    (w/'expected.bin').write_bytes(expected)
    difference=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)))
    raise AssertionError((w,difference,difference//(oracle.WIDTH+8),difference%(oracle.WIDTH+8)))
report.update(providers=reports,omission_checks=6,boundary='Actual native shader enables/disables, capability query and PC no-op. Full arena replay; pixel COM and deleting-destructor boundaries recorded, including callback mutations and ignored HRESULT failures. No GPU rendering.')
(ROOT/'rebuild/compile-gate/native-shader-enable.json').write_text(json.dumps(report,indent=2)+'\n')
print(r.stdout.strip(),report['snapshots'],'full-state snapshots',flush=True)
