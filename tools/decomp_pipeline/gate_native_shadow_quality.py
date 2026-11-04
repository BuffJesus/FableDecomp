"""Native shadow quality store/cleanup/recreation sequence with raw float bits."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-shadow-quality-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled/00/b4/CEngineShadowRenderer_ChangeVideoOptionShadowQuality_00b4fc10.cpp'];fixture=ROOT/'rebuild/tests/integration/NativeShadowQuality_test.cpp'
symbol='?ChangeVideoOptionShadowQuality@CEngineShadowRenderer@@QAE_NJM@Z';uninit='?UninitialiseTextures@CEngineShadowRenderer@@AAEXXZ';init='?InitialiseTextures@CEngineShadowRenderer@@AAE_NXZ'
(w/'shadow_quality_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in (('@Quality@16',symbol),(uninit,'@Uninitialise@8'),(init,'@Initialise@8')))+'\n')
body=read_virtual_data(EXE.read_bytes(),0xb4fc10,41)
for off,target in ((26,0xb4b830),(33,0xb4f8e0)):assert body[off-1]==0xe8 and 0xb4fc10+off+4+struct.unpack_from('<i',body,off)[0]==target
(w/'shadow_quality_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\n')
objects=[]
for n,p in enumerate(sources+[fixture]):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proofs=[check(objects[0],symbol,0xb4fc10,41,2,{uninit:0xb4b830,init:0xb4f8e0})]
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0
with (w/'input.bin').open('wb') as f:
 for base,size,bits,mode,result,precision in itertools.product((0,0x800),(0,1,512,0x7fffffff,0x80000000,0xffffffff),(0,0x80000000,0x3f800000,0xbf800000,1,0x7f7fffff,0xff7fffff,0x7f800000,0xff800000,0x7fc12345,0x7f800001,0xff812345),range(4),range(2),(0,0x10000,0x20000)):
  a=bytearray([0x95])*4096;a[base+0x2af]=cases%256
  f.write(struct.pack('<6I',base,size,bits,mode,result,precision));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as f,(w/'compiled.bin').open('rb') as g:
 while True:
  a=f.read(1<<20);b=g.read(1<<20);assert a==b,w
  if not a:break
snapshots=cases*3;assert (w/'compiled.bin').stat().st_size==snapshots*4104
report=dict(cases=cases,snapshots=snapshots,omission_checks=1,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Full4KiB and x87-status/result snapshots. Independent exact stores before cleanup,cleanup-before-init order,callback mutations retained,success/failure propagation. Twelve raw float values including signaling NaNs,three x87 precisions,six signed sizes,two object placements,four mutation modes. Texture cleanup and allocation remain explicit callbacks.')
(ROOT/'rebuild/compile-gate/native-shadow-quality.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_SHADOW_QUALITY_GATE_PASS',snapshots,'snapshots')
