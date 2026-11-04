import json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
import oracle_native_shader_layout_selection as oracle
import vector4_parity
from gate_game_component_abi import Coff
w=Path(tempfile.mkdtemp(prefix='native-layout-vector-selection-'));env=V.env();objects=[]
sources=['rebuild/src/compiled/00/98/CShaderRenderManager_SetVertexShaderConstantLayout_0098b5e0.cpp','rebuild/src/compiled/00/98/CShaderConstantRange_NotEqual_00987d70.cpp',
         'rebuild/src/compiled/00/99/CBaseClass_CBaseClass_0099a2d0.cpp']+list(vector4_parity.sources.values())+['rebuild/tests/integration/NativeShaderLayoutSelection_test.cpp']
for i,source in enumerate(sources):
    obj=w/('%d.obj'%i);r=V.cl(['/nologo','/c','/O2','/MT','/DFABLE_SELECTION_NATIVE_VECTOR','/Fo'+str(obj),str(ROOT/source)],env,w);assert not r.returncode,r.stdout+r.stderr
    objects.append(obj)
targets={'??9CShaderConstantRange@@QBE_NABV0@@Z':0x987d70,'??_7CShaderConstantRange@@6B@':0x129a04c,
         '??1CBaseClass@@UAE@XZ':0x99a2e0,'?AllocateLights@CShaderRenderManager@@QAEXKK@Z':0x98b420,
         '?GFClear4DVectors@NVectorMath@@YAXPAVC4DVector@@KABV2@@Z':0xa5c300,
         '?resize@?$vector@VC4DVector@@V?$allocator@VC4DVector@@@_STL@@@_STL@@QAEXIABVC4DVector@@@Z':0x98da90,
         '??_7CBaseClass@@6B@':0x1231710}
reports=[check(objects[0],'?SetVertexShaderConstantLayout@CShaderRenderManager@@QAEXJ@Z',0x98b5e0,633,6,targets),
         check(objects[1],'??9CShaderConstantRange@@QBE_NABV0@@Z',0x987d70,33,0,targets)]
support=check(objects[2],'??1CBaseClass@@UAE@XZ',0x99a2e0,7,1,targets)
production=dict(zip(vector4_parity.sources,objects[3:6]))
vector_reports=[check(production[name],symbol,a,length,n,vector4_parity.targets)for name,symbol,a,length,n in vector4_parity.specs]
replay=w/'insert-replay.obj'
r=V.cl(['/nologo','/c','/O2','/MT','/Dmalloc=FableVector4TestMalloc','/Dfree=FableVector4TestFree','/Fo'+str(replay),str(ROOT/vector4_parity.sources['Insert'])],env,w)
assert not r.returncode,r.stdout+r.stderr
original,renamed=Coff(objects[5]),Coff(replay)
for name,symbol,a,length,n in vector4_parity.specs:
    if name!='Insert':continue
    sec,off=original.symbols[symbol];other,other_off=renamed.symbols[symbol]
    body,rel=original.sections[sec];other_body,other_rel=renamed.sections[other]
    assert body==other_body and off==other_off
    assert {o:({'_malloc':'_FableVector4TestMalloc','_free':'_FableVector4TestFree'}.get(t,t),k)for o,(t,k)in rel.items()}==other_rel
objects[5]=replay
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omit in range(6):
    r=subprocess.run(cmd+[str(o)for i,o in enumerate(objects)if i!=omit],env=env,capture_output=True,text=True);assert r.returncode,omit
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs,expected,report=oracle.collect(native_vector=True);(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=60)
assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
actual=(w/'actual.bin').read_bytes()
if actual!=expected:
    (w/'expected.bin').write_bytes(expected)
    difference=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)))
    raise AssertionError((w,difference,difference//(oracle.WIDTH+32),difference%(oracle.WIDTH+32)))
report.update(providers=reports,support=support,vector_providers=vector_reports,omission_checks=6,boundary='Native layout selection, range comparison, base destructor and all vector<C4DVector> operations. Light allocation and vector clear remain recorded, with callback mutations; only malloc/free are deterministic boundaries for actual vector growth. Full arena and allocator events compared across no-growth and growth capacities. No GPU rendering.')
(ROOT/'rebuild/compile-gate/native-shader-layout-vector-selection.json').write_text(json.dumps(report,indent=2)+'\n')
print(r.stdout.strip(),report['snapshots'],'full-state snapshots',flush=True)
