"""Graphic-bank scene counters and delay settings with actual resource-list increments."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='graphic-bank-scene-delay-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/9f/CGraphicDataBank_SceneDelay_009fd0c0.cpp','00/9f/CResourceList_IncrementFrameCounter_009fc160.cpp')];fixture=ROOT/'rebuild/tests/integration/NativeGraphicBankSceneDelay_test.cpp'
scene='?ChangeScene@CGraphicDataBank@@QAEXXZ';delay='?SetReduceSizeFrameDelay@CGraphicDataBank@@QAEXK@Z';increment='?IncrementFrameCounter@CResourceList@@QAEXK@Z'
(w/'graphic_bank_scene_delay_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in (('@ChangeScene@8',scene),('@SetDelay@12',delay),('@Increment@12',increment)))+'\n')
raw=EXE.read_bytes();body=read_virtual_data(raw,0x9fd0c0,41)+bytes(7)+read_virtual_data(raw,0x9fd100,19)+bytes(13)+read_virtual_data(raw,0x9fc160,15)
for off in (17,35):assert body[off-1]==0xe8 and 0x9fd0c0+off+4+struct.unpack_from('<i',body,off)[0]==0x9fc160
(w/'graphic_bank_scene_delay_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\n')
objects=[]
for n,p in enumerate(sources+[fixture]):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proofs=[check(objects[0],scene,0x9fd0c0,41,2,{increment:0x9fc160}),check(objects[0],delay,0x9fd100,19,0,{}),check(objects[1],increment,0x9fc160,15,0,{})]
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in (0,1):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0
with (w/'input.bin').open('wb') as f:
 for base,delay_value,unload,counter,frames in itertools.product((0,0x400),(0,1,0x80000000,0xffffffff),(0,1,7,0x80000000,0xffffffff),(0,1,31,0x7fffffff,0x80000000,0xffffffff),(0,1,3,0x7fffffff,0x80000000,0xffffffff)):
  a=bytearray([0x95])*4096;struct.pack_into('<I',a,base+0x268,unload);struct.pack_into('<I',a,base+0x194,counter);struct.pack_into('<I',a,base+0x1d0,(counter+19)&0xffffffff)
  f.write(struct.pack('<3I',base,delay_value,frames));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as f,(w/'compiled.bin').open('rb') as g:
 while True:
  a=f.read(1<<20);b=g.read(1<<20);assert a==b,w
  if not a:break
snapshots=cases*4;assert (w/'compiled.bin').stat().st_size==snapshots*4096
report=dict(cases=cases,snapshots=snapshots,omission_checks=2,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Full4KiB arena and independently expected modular counters/delay fields after scene/delay/direct increment/scene sequence. Two object positions,four delay values,five unload delays,six counter seeds,six increments. Three original spans/twofixups;actual parent calls actual child,no external boundaries.')
(ROOT/'rebuild/compile-gate/native-graphic-bank-scene-delay.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_GRAPHIC_BANK_SCENE_DELAY_GATE_PASS',snapshots,'snapshots')
