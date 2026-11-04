import json,subprocess,tempfile
from pathlib import Path
import check_one as V
from variable_allocator_parity import check
import oracle_native_texture_preallocated as oracle
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-texture-preallocated-'));env=V.env();objects=[]
paths=['rebuild/src/compiled/00/9f/CTexture_PreallocatedStorage_009fa230.cpp','rebuild/tests/integration/NativeTexturePreallocated_test.cpp']
for i,path in enumerate(paths):
 obj=w/('%d.obj'%i);r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
targets={'?CalcByteLength@CTexture@@AAEXXZ':0x9f9ee0,'?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z':0x9e3830,'?GetColourDepth@CPixelFormat@@QBEKXZ':0x9e3820}
reports=[check(objects[0],'?InitialiseFromPreallocatedTexture@CTexture@@QAE_NPAUIDirect3DTexture9@@@Z',0x9fa230,74,1,targets),check(objects[0],'?CalcByteLength@CTexture@@AAEXXZ',0x9f9ee0,143,2,targets)]
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omit in range(1):
 r=subprocess.run(cmd+[str(o)for i,o in enumerate(objects)if i!=omit],env=env,capture_output=True,text=True);assert r.returncode,omit
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs,expected,report=oracle.collect();(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
actual=(w/'actual.bin').read_bytes()
if actual!=expected:
 (w/'expected.bin').write_bytes(expected);at=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)));raise AssertionError((w,at,at//(oracle.WIDTH+12),at%(oracle.WIDTH+12)))
report.update(providers=reports,omission_checks=1,boundary='Actual texture adoption and byte accounting; release, mip count/description and pixel-format lookup boundaries. Complete4KiB snapshots; null/live pointers, zero/negative/multiple levels, zero/normal/overflow dimensions, callback mutations of packed flags and texture pointer, failed HRESULT with populated descriptions. Full143-byte CalcByteLength span.')
(ROOT/'rebuild/compile-gate/native-texture-preallocated.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip(),report['snapshots'],'full-state snapshots',flush=True)
