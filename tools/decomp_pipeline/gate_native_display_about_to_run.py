"""Initial fade setup and profile completion."""
import hashlib,json,subprocess,tempfile
from pathlib import Path
import check_one as V
from gate_game_component_abi import Coff
from variable_allocator_parity import check
from pe_oracle import EXE,pe_sections,va_to_off
import oracle_native_display_about_to_run as oracle
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-display-about-to-run-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/43/CDisplayEngine_SetAboutToRun_00434cd0.cpp';fixture=ROOT/'rebuild/tests/integration/NativeDisplayAboutToRun_test.cpp'
symbol='?SetAboutToRun@CDisplayEngine@@QAEXXZ'
targets={'?GInitialFadeAllowedToBeActive@@3_NA':0x1375cdc,'?Update@CFadeInFadeOutBase@@QAEXM@Z':0x434870,'?EndProfile@NProfileTimer@@YIXJ@Z':0x9d8250}
aliases={'@AboutToRun@4':symbol,'?Update@CFadeInFadeOutBase@@QAEXM@Z':'@FadeUpdate@12','?EndProfile@NProfileTimer@@YIXJ@Z':'@EndProfile@4'}
(w/'display_about_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")'for a,b in aliases.items())+'\n')
objects=[]
settings=ROOT/'rebuild/src/compiled/01/37/DisplayEngine_FadeSettings_01375cdc.cpp'
for i,path in enumerate([source,fixture,settings]):
 obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proof=check(objects[0],symbol,0x434cd0,141,3,targets);c=Coff(objects[0]);raw=EXE.read_bytes();sections=pe_sections(raw)
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects[:2])),env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs,expected,report=oracle.collect();(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
actual=(w/'actual.bin').read_bytes()
if actual!=expected:
 (w/'expected.bin').write_bytes(expected);difference=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)));raise AssertionError((w,difference,difference//4112,difference%4112,len(actual),len(expected)))
report.update(providers=[proof],omission_checks=2,settings_sha256=hashlib.sha256(settings.read_bytes()).hexdigest(),source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Full4KiB and global switch, all byte fills, both fade-lock and global states, update/profile mutations. Fade Update and EndProfile are recorded boundaries. No normalized bytes.')
fade=ROOT/'rebuild/src/compiled/00/43/CFadeInFadeOutBase_Update_00434870.cpp'
fade_obj=w/'fade.obj';r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(fade_obj),str(fade)],env,w);assert not r.returncode,r.stdout+r.stderr
del aliases['?Update@CFadeInFadeOutBase@@QAEXM@Z']
(w/'display_about_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")'for a,b in aliases.items())+'\n')
r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(objects[1]),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects+[fade_obj])),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs,expected,composed=oracle.collect(actual_fade=True);(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode
assert (w/'actual.bin').read_bytes()==expected,w
report.update(composed=composed,composed_fade_sha256=hashlib.sha256(fade.read_bytes()).hexdigest(),composed_omission_checks=1)
(ROOT/'rebuild/compile-gate/native-display-about-to-run.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_DISPLAY_ABOUT_TO_RUN_PASS cases='+str(report['cases']),report['snapshots'],'snapshots; composed',composed)
