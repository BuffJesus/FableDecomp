"""Native bank reader, full-body parity and packed/file callback state replay."""
import hashlib,json,subprocess,tempfile
from pathlib import Path
import check_one as V
from variable_allocator_parity import check
import oracle_native_bank_read as oracle
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-bank-read-'));env=V.env();objects=[]
for i,name in enumerate(['rebuild/src/compiled/00/9c/CBankFile_ReadEntryData_009cc2a0.cpp','rebuild/tests/integration/NativeBankRead_test.cpp']):
 obj=w/('%d.obj'%i);r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/name)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proof=check(objects[0],'?ReadEntryData@CBankFile@@QAEXKKPAXK@Z',0x9cc2a0,95,2,{'??ACPackedUIntArray@@QBEKK@Z':0xa629f0})
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode,'provider required'
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs,expected,report=oracle.collect();(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
actual=(w/'actual.bin').read_bytes()
if actual!=expected:
 (w/'expected.bin').write_bytes(expected);difference=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)));raise AssertionError((w,difference,difference//4112,difference%4112))
report.update(providers=[proof],omission_checks=1,compared_bytes=len(actual),boundary='Actual95-byte typed bank reader. Full4KiB at every packed lookup/file seek/read and return; no masked bytes. Packed/unpacked metadata, index underflow, offset/length wraparound, and callbacks changing offset mode/runtime table/file pointer. Retains unused packed size lookup; reloads file receiver after seek. Packed decoder and file virtuals are boundaries; no filesystem I/O.')
(ROOT/'rebuild/compile-gate/native-bank-read.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip(),report['snapshots'],'snapshots')
