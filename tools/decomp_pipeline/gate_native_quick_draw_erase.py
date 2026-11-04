import json,subprocess,tempfile
from pathlib import Path
import check_one as V
from variable_allocator_parity import check
import oracle_native_quick_draw_erase as oracle
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-quick-draw-erase-'));env=V.env();objects=[]
paths=['rebuild/src/compiled/00/9f/CTexture_Uninitialise_009f9f70.cpp','rebuild/src/compiled/00/9f/CTexture_Assignment_009fa1c0.cpp',
       'rebuild/src/compiled/00/9e/CQuickDrawTriInfoVector_Erase_009e15e0.cpp','rebuild/tests/integration/NativeQuickDrawErase_test.cpp']
for i,path in enumerate(paths):
 obj=w/('%d.obj'%i);r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
from gate_game_component_abi import Coff
c=Coff(objects[2]);targets={'??4CWideString@@QAEAAV0@ABV0@@Z':0x99b7d0,'??4CTexture@@QAEABV0@ABV0@@Z':0x9fa1c0,'?Uninitialise@CTexture@@QAEXXZ':0x9f9f70,'??1CWideString@@QAE@XZ':0x99b510}
spec=[]
for prefix,address,length,count in [('??4CQuick',0x9de6f0,146,2),('??$__copy@',0x9de940,71,1),('?erase@',0x9e15e0,99,3)]:
 symbol=next(s for s in c.symbols if s.startswith(prefix));targets[symbol]=address;spec.append((symbol,address,length,count))
reports=[check(objects[2],symbol,address,length,count,targets)for symbol,address,length,count in spec]
support=[check(objects[0],'?Uninitialise@CTexture@@QAEXXZ',0x9f9f70,34,0,{}),check(objects[1],'??4CTexture@@QAEABV0@ABV0@@Z',0x9fa1c0,98,0,{})]
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omit in range(3):
 r=subprocess.run(cmd+[str(o)for i,o in enumerate(objects)if i!=omit],env=env,capture_output=True,text=True);assert r.returncode,omit
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs,expected,report=oracle.collect();(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
actual=(w/'actual.bin').read_bytes()
if actual!=expected:
 (w/'expected.bin').write_bytes(expected);at=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)));raise AssertionError((w,at,at//(oracle.WIDTH+12),at%(oracle.WIDTH+12)))
report.update(providers=reports,support=support,omission_checks=3,boundary='Actual draw-record assignment, counted copy and vector erase composed with native texture assignment and cleanup. Every valid erase range of lengths0..8, empty/full/partial, zero/live/mixed textures, self assignment, mutating callbacks; full4KiB arena compared at every boundary and return. String assignment/destruction and COM AddRef/Release are recorded boundaries.')
(ROOT/'rebuild/compile-gate/native-quick-draw-erase.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip(),report['snapshots'],'full-state snapshots',flush=True)
