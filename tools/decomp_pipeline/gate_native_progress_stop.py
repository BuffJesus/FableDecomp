"""Initial fade setup and profile completion."""
import hashlib,json,subprocess,tempfile
from pathlib import Path
import check_one as V
from gate_game_component_abi import Coff
from variable_allocator_parity import check
from pe_oracle import EXE,pe_sections,va_to_off
import oracle_native_progress_stop as oracle
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-progress-stop-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/9e/NProgressDisplay_StopProgress_009e9f20.cpp';fixture=ROOT/'rebuild/tests/integration/NativeProgressStop_test.cpp'
symbol='?StopProgress@NProgressDisplay@@YIX_N0@Z'
targets={'?PProgressDisplay@NProgressDisplay@@3V?$CCountedPointer@VCAProgressDisplay@@@@A':0x13caa38}
aliases={'@StopProgress@8':symbol,'?PProgressDisplay@NProgressDisplay@@3V?$CCountedPointer@VCAProgressDisplay@@@@A':'_ProgressOwner'}
(w/'progress_stop_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")'for a,b in aliases.items())+'\n')
objects=[]
for i,path in enumerate([source,fixture]):
 obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proof=check(objects[0],symbol,0x9e9f20,21,1,targets);c=Coff(objects[0]);raw=EXE.read_bytes();sections=pe_sections(raw)
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs,expected,report=oracle.collect();(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
actual=(w/'actual.bin').read_bytes()
if actual!=expected:
 (w/'expected.bin').write_bytes(expected);difference=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)));raise AssertionError((w,difference,difference//280,difference%280,len(actual),len(expected)))
report.update(providers=[proof],omission_checks=1,source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Full256-byte arena and8-byte owner. All byte fills, null/live display, both flags and callbacks replacing owner. Only virtual stop boundary; no normalized bytes.')
(ROOT/'rebuild/compile-gate/native-progress-stop.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip(),report['snapshots'],'snapshots')
