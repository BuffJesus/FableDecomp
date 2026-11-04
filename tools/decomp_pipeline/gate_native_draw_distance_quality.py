"""Draw-distance settings, scene invalidation and callback-mutable arrays."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='draw-distance-quality-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/b3/CEnginePrimitiveRenderer_SetDrawDistanceQuality_00b32010.cpp';fixture=ROOT/'rebuild/tests/integration/NativeDrawDistanceQuality_test.cpp'
symbol='?SetDrawDistanceQuality@CEnginePrimitiveRenderer@@QAEXW4EEngineVideoOptionQuality@@ABVCEngineVideoOptionsDef@@@Z';child='?InvalidateBoundingBoxesAndDrawDistances@CEngineScene@@QAEXXZ'
(w/'draw_distance_quality_aliases.h').write_text('#pragma comment(linker,"/alternatename:@SetQuality@16='+symbol+'")\n#pragma comment(linker,"/alternatename:'+child+'=@Invalidate@8")\n')
body=read_virtual_data(EXE.read_bytes(),0xb32010,134);assert body[98]==0xe8 and 0xb32010+103+struct.unpack_from('<i',body,99)[0]==0xb93500
(w/'draw_distance_quality_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\n')
objects=[]
for n,p in enumerate((source,fixture)):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proof=check(objects[0],symbol,0xb32010,134,1,{child:0xb93500})
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0;BASE=0x24000000
with (w/'input.bin').open('wb') as f:
 for flag,count,mode,placement,quality,pattern in itertools.product((0,1,127,255),range(4),range(6),(0x400,0x34,0x30),(0,1,3,0xffffffff),range(4)):
  a=bytearray([0x95])*4096
  for j in range(7):struct.pack_into('<I',a,placement+0x48+4*j,(0x3f800000+j,0x80000000+j,0x7f800001+j,0xffc00001+j)[pattern])
  a[placement+0x73]=flag;struct.pack_into('<I',a,0x290,BASE+0xb00);struct.pack_into('<3I',a,0x338,BASE+0x800,BASE+0x800+4*count,BASE+0x80c)
  for j in range(3):struct.pack_into('<I',a,0x800+4*j,BASE+0x900+64*j)
  f.write(struct.pack('<4I',quality,count,mode,placement));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as f,(w/'compiled.bin').open('rb') as g:
 while True:
  a=f.read(1<<20);b=g.read(1<<20);assert a==b,w
  if not a:break
size=(w/'compiled.bin').stat().st_size;assert size%4104==0;snapshots=size//4104
report=dict(cases=cases,snapshots=snapshots,omission_checks=1,providers=[proof],source_sha256={str(source.relative_to(ROOT)):hashlib.sha256(source.read_bytes()).hexdigest()},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Full4KiB arena after every scene callback and final return. Zero-to-three scenes,end truncation/extension,begin replacement,next entry replacement,repeated-mesh manager replacement,source flag/float mutation. Raw booleans/floats and overlapping settings copies,ignored enum values. Callback receiver/count and final flags independently asserted. Scene invalidation is an explicit boundary.')
(ROOT/'rebuild/compile-gate/native-draw-distance-quality.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_DRAW_DISTANCE_QUALITY_GATE_PASS',snapshots,'snapshots')
