"""Cutscene insertion composed with the actual camera copy constructor."""
import hashlib,json,subprocess,tempfile
from pathlib import Path
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2]
w=Path(tempfile.mkdtemp(prefix='native-camera-copy-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/b2/CEngine_CutsceneAddCameraAngle_00b27d50.cpp'
copy_source=ROOT/'rebuild/src/compiled/00/49/CCamera_Copy_0049d380.cpp'
fixture=ROOT/'rebuild/tests/integration/NativeCutsceneCamera_test.cpp'
symbol='?CutsceneAddCameraAngle@CEngine@@UAEXABVCCamera@@@Z'
copy='??0CCamera@@QAE@ABV0@@Z'
overflow='?_M_insert_overflow@?$vector@VCCamera@@V?$allocator@VCCamera@@@_STL@@@_STL@@QAEXPAVCCamera@@ABV3@ABU__false_type@2@I_N@Z'
raw=read_virtual_data(EXE.read_bytes(),0xb27d50,62)+bytes(2)+read_virtual_data(EXE.read_bytes(),0x49d380,97)
(w/'cutscene_camera_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,raw))+'};\n#pragma comment(linker,"/alternatename:@AppendCamera@12='+symbol+'")\n#pragma comment(linker,"/alternatename:'+overflow+'=@Overflow@28")\n')
objects=[]
for n,p in enumerate((source,copy_source,fixture)):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proofs=[check(objects[0],symbol,0xb27d50,62,2,{copy:0x49d380,overflow:0xb29840}),check(objects[1],copy,0x49d380,97,0,{})]
link=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(2):
 r=subprocess.run(link+[str(o) for n,o in enumerate(objects) if n!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode,(r.returncode,r.stdout,r.stderr)
assert 'NATIVE_CUTSCENE_CAMERA_PASS cases=1344 snapshots=5376' in r.stdout;rpt=dict(cases=1344,snapshots=5376,omission_checks=2,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in (source,copy_source)},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),header_sha256=hashlib.sha256((ROOT/'rebuild/include/fable_camera.h').read_bytes()).hexdigest(),boundary='Actual cutscene append and camera copy. Two-call sequences over full2048-byte state,zero-to-three existing cameras,zero-to-two spare slots,two engine placements,source aliases,raw float/bool bits,padding. Only vector overflow remains a callback; verifies this/position/value/tag/amount/end arguments and models new storage.')
(ROOT/'rebuild/compile-gate/native-cutscene-camera.json').write_text(json.dumps(rpt,indent=2)+'\n')
print(r.stdout.strip());print('NATIVE_CUTSCENE_CAMERA_GATE_PASS 5376 snapshots')
