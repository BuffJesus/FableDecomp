"""Native camera copy preserves member bits and destination padding."""
import hashlib,json,subprocess,tempfile
from pathlib import Path
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2]
w=Path(tempfile.mkdtemp(prefix='native-camera-copy-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/49/CCamera_Copy_0049d380.cpp'
fixture=ROOT/'rebuild/tests/integration/NativeCameraCopy_test.cpp'
symbol='??0CCamera@@QAE@ABV0@@Z'
raw=read_virtual_data(EXE.read_bytes(),0x49d380,97)
(w/'camera_copy_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,raw))+'};\n#pragma comment(linker,"/alternatename:@CopyCamera@12='+symbol+'")\n')
objects=[]
for n,p in enumerate((source,fixture)):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proof=check(objects[0],symbol,0x49d380,97,0,{})
link=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
r=subprocess.run(link+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode,(r.returncode,r.stdout,r.stderr)
assert 'NATIVE_CAMERA_COPY_PASS cases=3744 snapshots=7488' in r.stdout;rpt=dict(cases=3744,snapshots=7488,omission_checks=1,providers=[proof],source_sha256={str(source.relative_to(ROOT)):hashlib.sha256(source.read_bytes()).hexdigest()},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),header_sha256=hashlib.sha256((ROOT/'rebuild/include/fable_camera.h').read_bytes()).hexdigest(),boundary='Actual CCamera copy constructor; no callbacks. Full512-byte state compared to original and independent member-byte model. Two distinct placements and diagnostic self-copy,13 raw bit patterns,noncanonical bool bytes,preserved padding,six x87 controls.')
(ROOT/'rebuild/compile-gate/native-camera-copy.json').write_text(json.dumps(rpt,indent=2)+'\n')
print(r.stdout.strip());print('NATIVE_CAMERA_COPY_GATE_PASS 7488 snapshots')
