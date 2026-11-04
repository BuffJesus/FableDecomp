import json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from gate_game_component_abi import Coff
from variable_allocator_parity import check
import oracle_native_shader_layout_vector as oracle
import shader_layout_vector_parity as parity
w=Path(tempfile.mkdtemp(prefix='native-layout-vector-'));env=V.env();objects=[];production={}
names=['RegisterVertexShaderConstantLayout','UnregisterVertexShaderConstantLayouts','layout_resize','layout_insert']
for name in names:
    source=ROOT/'rebuild/src/compiled'/parity.sources[name];obj=w/(name+'.obj')
    r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr
    objects.append(obj);production[name]=obj
reports=[check(production[name],symbol,a,length,n,parity.targets)for name,symbol,a,length,n in parity.specs]
replay=w/'insert-replay.obj';source=ROOT/'rebuild/src/compiled'/parity.sources['layout_insert']
r=V.cl(['/nologo','/c','/O2','/MT','/Dmalloc=FableShaderLayoutTestMalloc','/Dfree=FableShaderLayoutTestFree','/Fo'+str(replay),str(source)],env,w)
assert not r.returncode,r.stdout+r.stderr
original,renamed=Coff(objects[3]),Coff(replay)
for symbol,(section,offset) in original.symbols.items():
    if symbol not in renamed.symbols or section not in original.sections:continue
    body,rel=original.sections[section]
    if not rel:continue
    other,other_offset=renamed.symbols[symbol];other_body,other_rel=renamed.sections[other]
    assert body==other_body and offset==other_offset
    assert {o:({'_malloc':'_FableShaderLayoutTestMalloc','_free':'_FableShaderLayoutTestFree'}.get(t,t),k)for o,(t,k)in rel.items()}==other_rel
objects[3]=replay
obj=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/'rebuild/tests/integration/NativeShaderLayoutVector_test.cpp')],env,w)
assert not r.returncode,r.stdout+r.stderr
objects.append(obj);exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omit in range(4):
    r=subprocess.run(cmd+[str(o)for i,o in enumerate(objects)if i!=omit],env=env,capture_output=True,text=True);assert r.returncode,omit
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs,expected,report=oracle.collect();(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=60)
assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
actual=(w/'actual.bin').read_bytes()
if actual!=expected:
    (w/'expected.bin').write_bytes(expected)
    difference=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)))
    raise AssertionError((w,difference,difference//(oracle.WIDTH+8),difference%(oracle.WIDTH+8)))
report.update(providers=reports,omission_checks=4,boundary='Native registration/unregistration, resize, fill-insert, overflow and fastcall fill. Full arena including old/new storage; direct middle/begin/end insertion, shrink/zero counts, aliasing source, and repeated parent operations. malloc/free recorded with deterministic fixture storage; allocator-renamed object code/relocations checked against production. memmove executes native CRT and reference copy in the retail oracle. No GPU rendering.')
(ROOT/'rebuild/compile-gate/native-shader-layout-vector.json').write_text(json.dumps(report,indent=2)+'\n')
print(r.stdout.strip(),report['snapshots'],'full-state snapshots',flush=True)
