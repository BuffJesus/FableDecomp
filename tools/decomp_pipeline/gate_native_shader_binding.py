import json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
import oracle_native_shader_binding as oracle
from variable_allocator_parity import check
w=Path(tempfile.mkdtemp(prefix='native-shader-binding-'));env=V.env();objects=[]
for i,source in enumerate(['rebuild/src/compiled/00/98/CShaderRenderManager_ApplyVertexShader_00988020.cpp','rebuild/tests/integration/NativeShaderBinding_test.cpp']):
    obj=w/('%d.obj'%i);r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/source)],env,w)
    assert not r.returncode,r.stdout+r.stderr
    objects.append(obj)
parity=check(objects[0],'?ApplyVertexShader@CShaderRenderManager@@QAEXABVCVertexShader@@_N@Z',0x988020,176,0,{})
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs,expected,report=oracle.collect();(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=60)
assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
actual=(w/'actual.bin').read_bytes()
if actual!=expected:
    (w/'expected.bin').write_bytes(expected)
    difference=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)))
    raise AssertionError((w,difference,difference//(oracle.WIDTH+12),difference%(oracle.WIDTH+12)))
report.update(providers=[parity],omission_checks=1,boundary='Two repeated native binds per case; all software/fixed mode combinations, current hardware state, cache hit/miss and null shader handle, ignored force flag/HRESULT. COM callbacks mutate device, cached handles, input shader data and renderer state. Full arena and exact call arguments compared; no GPU rendering.')
(ROOT/'rebuild/compile-gate/native-shader-binding.json').write_text(json.dumps(report,indent=2)+'\n')
print(r.stdout.strip(),report['snapshots'],'full-state snapshots',parity['grade'])
