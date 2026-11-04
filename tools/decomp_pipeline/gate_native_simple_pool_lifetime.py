import json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
import oracle_native_simple_pool_lifetime as oracle
w=Path(tempfile.mkdtemp(prefix='native-simple-pool-lifetime-'));env=V.env();objects=[]
specs=[['FreeAll', '?FreeAll@CSimpleMemoryPool@@QAEXXZ', 10864416, 101, 0], ['Allocate', '?Allocate@CSimpleMemoryPool@@QAEPAXK@Z', 10864912, 134, 1], ['SetCapacity', '?SetCapacity@CSimpleMemoryPool@@QAEXK@Z', 10864064, 19, 0], ['CalculateMemoryAllocatedByPool', '?CalculateMemoryAllocatedByPool@CSimpleMemoryPool@@QBEKXZ', 10864096, 29, 0]]
sources=['rebuild/src/compiled/00/a5/CSimpleMemoryPool_FreeAll_00a5c720.cpp','rebuild/src/compiled/00/a5/CSimpleMemoryPool_Allocate_00a5c910.cpp','rebuild/src/compiled/00/a5/CSimpleMemoryPool_SetCapacity_00a5c5c0.cpp','rebuild/src/compiled/00/a5/CSimpleMemoryPool_CalculateMemoryAllocatedByPool_00a5c5e0.cpp','rebuild/src/compiled/00/a5/CSimpleMemoryPool_BlockLifetime_00a5c890.cpp','rebuild/src/compiled/00/42/CIVCountedPointeeBase_DeleteCIVCountedPointeeBaseUBEXXZ_0042d930.cpp','rebuild/tests/integration/NativeSimplePoolLifetime_test.cpp']
for i,source in enumerate(sources):
    obj=w/('%d.obj'%i);r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/source)],env,w);assert not r.returncode,r.stdout+r.stderr
    objects.append(obj)
targets={'??_7CMemoryBlock@CSimpleMemoryPool@@6B@': 19518824, '??_7CIVCountedPointeeBase@@6B@': 19092688, '?AllocateAligned@CMemoryAllocator@@SIPAXKJ@Z': 10431456, '?DeallocateAligned@CMemoryAllocator@@SIXPAX@Z': 10431264, '??2@YAPAXI@Z': 12577306, '??3@YAXPAX@Z': 12577212, '?CreateNewMemoryBlock@CSimpleMemoryPool@@AAEXK@Z': 10864784}
reports=[check(objects[i],sym,a,n,rel,targets)for i,(name,sym,a,n,rel)in enumerate(specs)]
lifetime_specs=[['??0CMemoryBlock@CSimpleMemoryPool@@QAE@K@Z', 10864240, 53, 2], ['??1CMemoryBlock@CSimpleMemoryPool@@UAE@XZ', 10864352, 32, 3], ['??_GCMemoryBlock@CSimpleMemoryPool@@UAEPAXI@Z', 10865072, 52, 4], ['?CreateNewMemoryBlock@CSimpleMemoryPool@@AAEXK@Z', 10864784, 121, 3]]
lifetime=[check(objects[4],sym,a,n,rel,targets)for sym,a,n,rel in lifetime_specs]
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omit in (0,1,3,4,5):
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
report.update(providers=reports,omission_checks=5,lifetime=lifetime,boundary='Actual pool allocation, block creation, block constructor/destructor/deleting destructor and FreeAll. Only operator new/delete and aligned heap APIs recorded. Repeated growth/reset/retained single-block behavior, owned/borrowed direct lifetimes, destructor flags0/1 and allocator callback mutations. Complete arena with only known vtable pointer relocation normalized.')
(ROOT/'rebuild/compile-gate/native-simple-pool-lifetime.json').write_text(json.dumps(report,indent=2)+'\n')
print(r.stdout.strip(),report['snapshots'],'full-state snapshots',flush=True)
