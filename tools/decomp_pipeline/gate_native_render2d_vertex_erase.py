import json,subprocess,tempfile
from pathlib import Path
import check_one as V
from variable_allocator_parity import check
import oracle_native_render2d_vertex_erase as oracle
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-render2d-vertex-erase-'));env=V.env();objects=[]
paths=['rebuild/src/compiled/00/9e/Render2DVertexVector_Erase_009e1440.cpp','rebuild/tests/integration/NativeRender2DVertexErase_test.cpp']
for i,path in enumerate(paths):
 obj=w/('%d.obj'%i);r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
reports=[check(objects[0],'?erase@?$vector@VCTVertexRHWColSpecTex1@@V?$allocator@VCTVertexRHWColSpecTex1@@@_STL@@@_STL@@QAEPAVCTVertexRHWColSpecTex1@@PAV3@0@Z',0x9e1440,79,0,{})]
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omit in range(1):
 r=subprocess.run(cmd+[str(o)for i,o in enumerate(objects)if i!=omit],env=env,capture_output=True,text=True);assert r.returncode,omit
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs,expected,report=oracle.collect();(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
actual=(w/'actual.bin').read_bytes()
if actual!=expected:
 (w/'expected.bin').write_bytes(expected);at=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)));raise AssertionError((w,at,at//(oracle.WIDTH+4),at%(oracle.WIDTH+4)))
report.update(providers=reports,omission_checks=1,boundary='Native typed vertex-vector erase, no callback boundaries. All valid erase intervals at sizes0..16, preserving complete4KiB arena including raw floating-point payload bits, surrounding sentinels, vector finish/capacity and returned iterator.')
(ROOT/'rebuild/compile-gate/native-render2d-vertex-erase.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip(),report['snapshots'],'full-state snapshots',flush=True)
