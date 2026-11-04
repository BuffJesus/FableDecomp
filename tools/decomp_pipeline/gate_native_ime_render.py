"""Native IME rendering coordinator parity and complete-state retail comparison."""
import json,subprocess,tempfile
from pathlib import Path
import check_one as V
import oracle_native_ime_render as oracle
from variable_allocator_parity import check
ROOT=Path(__file__).resolve().parents[2]
w=Path(tempfile.mkdtemp(prefix='native-ime-render-'));env=V.env();objects=[]
for i,source in enumerate(['rebuild/src/compiled/00/40/CIME_Render_00404c00.cpp',
                          'rebuild/src/compiled/01/37/IMEAlphaDefault_013750b0.cpp',
                          'rebuild/tests/integration/NativeIMERender_test.cpp']):
    obj=w/('%d.obj'%i);r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/source)],env,w)
    assert not r.returncode,r.stdout+r.stderr
    objects.append(obj)
report=check(objects[0],'?Render@CIME@NUISystem@@QAEXXZ',0x404c00,69,5,
    {'?g_IMEAlphaDefault_013750B0@@3EA':0x13750b0,'?ImeUi_RenderUI@@YIX_N0@Z':0xcb38e0,
     '?GetInstance@CManager@NUISystem@@SIPAV12@XZ':0x41e5f2,'?DrawWithRenderManager@CSprite@NUISystem@@QAEXXZ':0x41a980})
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omit in range(2):
    r=subprocess.run(cmd+[str(o)for i,o in enumerate(objects)if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs,expected,counts=oracle.collect();(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=30)
assert not r.returncode,(r.returncode,r.stdout,r.stderr,w)
assert (w/'actual.bin').read_bytes()==expected,w
report.update(**counts,omission_checks=2,boundary='Actual native CIME rendering coordinator and alpha storage. Edit colour, IME UI and cursor draw recorded; callback replacement/nullability verified. No pixels rendered.')
(ROOT/'rebuild/compile-gate/native-ime-render.json').write_text(json.dumps(report,indent=2)+'\n')
print(r.stdout.strip(),counts['snapshots'],'full-state snapshots')
