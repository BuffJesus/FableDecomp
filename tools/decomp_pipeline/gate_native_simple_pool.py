import json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
import oracle_native_simple_pool as oracle
w=Path(tempfile.mkdtemp(prefix='native-simple-pool-'));env=V.env();objects=[]
specs=[['FreeAll', '?FreeAll@CSimpleMemoryPool@@QAEXXZ', 10864416, 101, 0], ['Allocate', '?Allocate@CSimpleMemoryPool@@QAEPAXK@Z', 10864912, 134, 1], ['SetCapacity', '?SetCapacity@CSimpleMemoryPool@@QAEXK@Z', 10864064, 19, 0], ['CalculateMemoryAllocatedByPool', '?CalculateMemoryAllocatedByPool@CSimpleMemoryPool@@QBEKXZ', 10864096, 29, 0]]
sources=['rebuild/src/compiled/00/a5/CSimpleMemoryPool_FreeAll_00a5c720.cpp', 'rebuild/src/compiled/00/a5/CSimpleMemoryPool_Allocate_00a5c910.cpp', 'rebuild/src/compiled/00/a5/CSimpleMemoryPool_SetCapacity_00a5c5c0.cpp', 'rebuild/src/compiled/00/a5/CSimpleMemoryPool_CalculateMemoryAllocatedByPool_00a5c5e0.cpp', 'rebuild/tests/integration/NativeSimplePool_test.cpp']
for i,source in enumerate(sources):
    obj=w/('%d.obj'%i);r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/source)],env,w);assert not r.returncode,r.stdout+r.stderr
    objects.append(obj)
targets={'?CreateNewMemoryBlock@CSimpleMemoryPool@@AAEXK@Z':0xa5c890}
reports=[check(objects[i],sym,a,n,rel,targets)for i,(name,sym,a,n,rel)in enumerate(specs)]
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
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
    raise AssertionError((w,difference,difference//(oracle.WIDTH+12),difference%(oracle.WIDTH+12)))
report.update(providers=reports,omission_checks=4,boundary='Native allocation/free-list splitting and repeated FreeAll, capacity alignment and complete block accounting. Growth and deleting-destructor callbacks recorded with mutations. Zero/wrapping sizes, exact/split fit, first/later free area, empty/owned/borrowed block lists and unsigned accounting wrap. Full24KiB state plus returns/callbacks compared; block creation/destruction are fixture boundaries.')
(ROOT/'rebuild/compile-gate/native-simple-pool.json').write_text(json.dumps(report,indent=2)+'\n')
print(r.stdout.strip(),report['snapshots'],'full-state snapshots',flush=True)
