import json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
import oracle_native_shader_updates as oracle
from variable_allocator_parity import check
w=Path(tempfile.mkdtemp(prefix='native-shader-updates-'));env=V.env();objects=[];reports=[]
specs=[('SetVSConstants',0x98b930,37,'QAEXJPBXK@Z'),('UpdatePixelShader',0x988a20,41,'QAEXXZ'),
       ('UpdateAmbient',0x989760,85,'QAEXXZ'),('UpdateFogColour',0x9897c0,99,'QAEXXZ'),
       ('UpdateLightGlobals',0x98a760,54,'QAEXXZ'),('UpdatePixelShaderConstants',0x989bf0,186,'QAEXXZ'),
       ('ResetWorldTransform',0x988290,131,'QAEXXZ')]
targets={'?$S1@?4??UpdatePixelShaderConstants@CShaderRenderManager@@QAEXXZ@4IA':0x13bc430,
         '?ps_constants@?4??UpdatePixelShaderConstants@CShaderRenderManager@@QAEXXZ@4PAVC4DVector@@A':0x13bc3b0,
         '??0C4DVector@@QAE@XZ':0x987670,'??_H@YGXPAXIHP6EPAX0@Z@Z':0x4038c0}
for name,address,length,mangling in specs:
    obj=w/(name+'.obj');source=ROOT/'rebuild/src/compiled/00/98'/('CShaderRenderManager_'+name+'_%08x.cpp'%address)
    r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr
    reports.append(check(obj,'?'+name+'@CShaderRenderManager@@'+mangling,address,length,8 if address==0x989bf0 else 0,targets));objects.append(obj)
    print(reports[-1],flush=True)
support=[check(objects[5],'??0C4DVector@@QAE@XZ',0x987670,3,0,{}),
         check(objects[5],'??_H@YGXPAXIHP6EPAX0@Z@Z',0x4038c0,48,0,{})]
obj=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/'rebuild/tests/integration/NativeShaderUpdates_test.cpp')],env,w)
assert not r.returncode,r.stdout+r.stderr
objects.append(obj);exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omit in range(7):
    r=subprocess.run(cmd+[str(o)for i,o in enumerate(objects)if i!=omit],env=env,capture_output=True,text=True);assert r.returncode,omit
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs,expected,report=oracle.collect();(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=60)
assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
actual=(w/'actual.bin').read_bytes()
if actual!=expected:
    (w/'expected.bin').write_bytes(expected)
    difference=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)))
    raise AssertionError((w,difference,difference//(oracle.WIDTH+144),difference%(oracle.WIDTH+144)))
report.update(providers=reports,support=support,omission_checks=7,boundary='Full arena and upload payload comparison, callback pointer/flag/colour/world-matrix mutations, ignored HRESULT failures, four x87 rounding modes, signed zero/infinities/quiet NaN. Pixel constant table reordered through native static scratch; genuine compiler vector constructor and empty C4DVector constructor separately exact. ResetWorldTransform executes genuine C++. COM calls recorded; no GPU rendering.')
(ROOT/'rebuild/compile-gate/native-shader-updates.json').write_text(json.dumps(report,indent=2)+'\n')
print(r.stdout.strip(),report['snapshots'],'full-state snapshots',flush=True)
