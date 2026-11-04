"""Native bank reader, full-body parity and packed/file callback state replay."""
import hashlib,json,subprocess,tempfile
from pathlib import Path
import check_one as V
from variable_allocator_parity import check
import oracle_native_indexed_char_append as oracle
from gate_game_component_abi import Coff
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-indexed-char-append-'));env=V.env();objects=[]
for i,name in enumerate(['rebuild/src/compiled/00/98/IndexedCharArray_PushBack_0098db20.cpp','rebuild/tests/integration/NativeIndexedCharAppend_test.cpp']):
 obj=w/('%d.obj'%i);r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/name)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
symbol='?push_back@?$CIndexedArray@VCCharString@@@@QAEXABVCCharString@@@Z'
c=Coff(objects[0]);sec,_=c.symbols[symbol];rel=c.sections[sec][1]
targets={name:0x431606 if name.startswith('?insert_unique@')else 0x99ec30 if name.startswith('??0CCharString')else 0x99eae0 if name.startswith('??1CCharString')else 0x433530 for name,kind in rel.values()}
proof=check(objects[0],symbol,0x98db20,124,5,targets)
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode,'provider required'
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs,expected,report=oracle.collect();(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
actual=(w/'actual.bin').read_bytes()
if actual!=expected:
 (w/'expected.bin').write_bytes(expected);difference=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)));raise AssertionError((w,difference,difference//4112,difference%4112))
report.update(providers=[proof],omission_checks=1,compared_bytes=len(actual),boundary='Actual full124-byte indexed string append, including overflow tail after first return. Full4KiB at every dependency and return, no masked bytes. Records cached pre-copy index, map duplicate result, append ordering, aliased values, full/spare capacity and callbacks changing vector storage/finish. String copy/destruction, unique map insertion and vector overflow are recorded ABI boundaries; map algorithm itself is not claimed.')
(ROOT/'rebuild/compile-gate/native-indexed-char-append.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip(),report['snapshots'],'snapshots')
