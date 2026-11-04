"""Retail engine allocation size, failure and constructor forwarding."""
import hashlib,json,subprocess,tempfile
from pathlib import Path
import check_one as V
from gate_game_component_abi import Coff
from variable_allocator_parity import check
from pe_oracle import EXE,pe_sections,va_to_off
import oracle_native_allocate_engine as oracle
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-allocate-engine-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/b2/Global_GFAllocEngine_00b26340.cpp';fixture=ROOT/'rebuild/tests/integration/NativeAllocateEngine_test.cpp'
symbol='?GFAllocEngine@@YIPAVCIEngine@@XZ'
targets={'??2@YAPAXI@Z':0xbfea1a,'??0CEngine@@QAE@XZ':0xb260b0}
aliases={'@AllocateEngine@0':symbol,'??0CEngine@@QAE@XZ':'@ConstructEngine@8'}
(w/'allocate_engine_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")'for a,b in aliases.items())+'\n')
objects=[]
for i,path in enumerate([source,fixture]):
 obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proof=check(objects[0],symbol,0xb26340,27,2,targets);c=Coff(objects[0]);raw=EXE.read_bytes();sections=pe_sections(raw)
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs,expected,report=oracle.collect();(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
actual=(w/'actual.bin').read_bytes()
if actual!=expected:
 (w/'expected.bin').write_bytes(expected);difference=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)));raise AssertionError((w,difference,difference//1032,difference%1032,len(actual),len(expected)))
report.update(providers=[proof],omission_checks=1,source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Full1024-byte arena. All byte fills, null/success allocation at two offsets, constructor object/whole-arena mutation. Allocation and constructor boundaries only; exact requested178-byte size and returned identity; no masked bytes.')
(ROOT/'rebuild/compile-gate/native-allocate-engine.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip(),report['snapshots'],'snapshots')
