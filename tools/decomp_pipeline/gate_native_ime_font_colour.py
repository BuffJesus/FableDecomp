"""Check exact native SetColor and all 256 alpha values with packed-colour cases."""
import json,subprocess,tempfile
from pathlib import Path
import check_one as V
from variable_allocator_parity import check
ROOT=Path(__file__).resolve().parents[2]
w=Path(tempfile.mkdtemp(prefix='native-ime-font-colour-'));env=V.env();objects=[]
for i,source in enumerate(['rebuild/src/compiled/00/40/CIMEFont_SetColor_00404d20.cpp',
                          'rebuild/src/compiled/01/37/IMEAlphaDefault_013750b0.cpp',
                          'rebuild/tests/integration/NativeIMEFontColour_test.cpp']):
    obj=w/('%d.obj'%i);r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/source)],env,w)
    assert not r.returncode,r.stdout+r.stderr
    objects.append(obj)
report=check(objects[0],'?SetColor@CIMEFont@NUISystem@@UAEXK@Z',0x404d20,47,1,{'?g_IMEAlphaDefault_013750B0@@3EA':0x13750b0})
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
for omit in range(2):
    r=subprocess.run(cmd+[str(o)for i,o in enumerate(objects)if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30)
assert not r.returncode and 'NATIVE_IME_FONT_COLOUR_PASS cases=2304'in r.stdout,(r.returncode,r.stdout,r.stderr,w)
report.update(cases=2304,omission_checks=2,boundary='Actual font SetColor and alpha storage. Full20-byte object checked; input alpha ignored, every replacement alpha exercised. No text rendering.')
(ROOT/'rebuild/compile-gate/native-ime-font-colour.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip())
