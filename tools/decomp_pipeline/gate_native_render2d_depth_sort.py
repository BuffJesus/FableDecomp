import json,subprocess,tempfile
from pathlib import Path
import check_one as V
from variable_allocator_parity import check
import oracle_native_render2d_depth_sort as oracle
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-render2d-depth-sort-'));env=V.env();objects=[]
for i,path in enumerate(['rebuild/src/compiled/00/9d/Render2D_DepthComparators_009d8c30.cpp','rebuild/tests/integration/NativeRender2DDepthSort_test.cpp']):
 obj=w/('%d.obj'%i);r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
reports=[check(objects[0],'_FableCompareQuickDrawInfoDepth_009d8c30',0x9d8c30,32,0,{}),
         check(objects[0],'_FableCompareQuickDrawVerticesDepth_009d8cb0',0x9d8cb0,58,2,{'__real@3eaaaaab':0x125a4dc})]
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs,expected,report=oracle.collect();(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
actual=(w/'actual.bin').read_bytes()
if actual!=expected:
 (w/'expected.bin').write_bytes(expected);at=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)));raise AssertionError((w,at,at//200,at%200))
report.update(providers=reports,omission_checks=1,boundary='Two actual native cdecl callbacks; all four x87 rounding modes and24/53/64-bit precision, signed zero, infinities, quiet/signaling NaNs, denormals, extreme and adjacent finite values. Results and complete192-byte input records compared. Equal and unordered comparisons return1. Does not claim qsort composition or rendering.')
(ROOT/'rebuild/compile-gate/native-render2d-depth-sort.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip(),report['comparisons'],'comparisons',flush=True)
