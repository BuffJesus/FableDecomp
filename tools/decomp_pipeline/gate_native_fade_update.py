"""Initial fade setup and profile completion."""
import hashlib,json,subprocess,tempfile
from pathlib import Path
import check_one as V
from gate_game_component_abi import Coff
from variable_allocator_parity import check
from pe_oracle import EXE,pe_sections,va_to_off
import oracle_native_fade_update as oracle
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-fade-update-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/43/CFadeInFadeOutBase_Update_00434870.cpp';fixture=ROOT/'rebuild/tests/integration/NativeFadeUpdate_test.cpp'
symbol='?Update@CFadeInFadeOutBase@@QAEXM@Z'
targets={'__real@00000000':0x122dedc}
aliases={'@UpdateFade@12':symbol}
(w/'fade_update_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")'for a,b in aliases.items())+'\n')
objects=[]
for i,path in enumerate([source,fixture]):
 obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proof=check(objects[0],symbol,0x434870,95,2,targets);c=Coff(objects[0]);raw=EXE.read_bytes();sections=pe_sections(raw)
sec,off=c.symbols['__real@00000000'];assert c.sections[sec][0][off:off+4]==bytes(4);o=va_to_off(sections,0x122dedc);assert raw[o:o+4]==bytes(4)
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs,expected,report=oracle.collect();(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
actual=(w/'actual.bin').read_bytes()
if actual!=expected:
 (w/'expected.bin').write_bytes(expected);difference=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)));raise AssertionError((w,difference,difference//28,difference%28,len(actual),len(expected)))
report.update(providers=[proof],omission_checks=1,source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Full28-byte fade state, all flags, timer/duration/elapsed Cartesian edge cases and53/64-bit precision. Actual production body without callback boundaries or normalized bytes.')
(ROOT/'rebuild/compile-gate/native-fade-update.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip(),report['snapshots'],'snapshots')
