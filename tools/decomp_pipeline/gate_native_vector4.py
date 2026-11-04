import json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from gate_game_component_abi import Coff
from variable_allocator_parity import check
import oracle_native_vector4 as oracle
import vector4_parity as parity
w=Path(tempfile.mkdtemp(prefix='native-vector4-'));env=V.env();objects=[];production={}
names=['Resize','Erase','Insert']
for name in names:
    source=ROOT/parity.sources[name];obj=w/(name+'.obj')
    r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr
    objects.append(obj);production[name]=obj
reports=[check(production[name],symbol,a,length,n,parity.targets)for name,symbol,a,length,n in parity.specs]
replay=w/'insert-replay.obj';source=ROOT/parity.sources['Insert']
r=V.cl(['/nologo','/c','/O2','/MT','/Dmalloc=FableVector4TestMalloc','/Dfree=FableVector4TestFree','/Fo'+str(replay),str(source)],env,w)
assert not r.returncode,r.stdout+r.stderr
original,renamed=Coff(objects[2]),Coff(replay)
for symbol,(section,offset) in original.symbols.items():
    if symbol not in renamed.symbols or section not in original.sections:continue
    body,rel=original.sections[section]
    if not rel:continue
    other,other_offset=renamed.symbols[symbol];other_body,other_rel=renamed.sections[other]
    assert body==other_body and offset==other_offset
    assert {o:({'_malloc':'_FableVector4TestMalloc','_free':'_FableVector4TestFree'}.get(t,t),k)for o,(t,k)in rel.items()}==other_rel
objects[2]=replay
obj=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/'rebuild/tests/integration/NativeVector4_test.cpp')],env,w)
assert not r.returncode,r.stdout+r.stderr
objects.append(obj);exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
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
    raise AssertionError((w,difference,difference//(oracle.WIDTH+8),difference%(oracle.WIDTH+8)))
report.update(providers=reports,omission_checks=3,boundary='Native C4DVector resize, erase, fill insert, overflow, uninitialized range copy/fill, backward copy and fill. Direct beginning/middle/end erasure/insertion, zero sizes, growth and aliasing; complete 24KiB arena and allocator events compared. malloc/free recorded with deterministic storage; allocator-renamed objects audited against production. No GPU rendering.')
(ROOT/'rebuild/compile-gate/native-vector4.json').write_text(json.dumps(report,indent=2)+'\n')
print(r.stdout.strip(),report['snapshots'],'full-state snapshots',flush=True)
