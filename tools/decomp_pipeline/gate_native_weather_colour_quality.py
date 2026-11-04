"""Exact weather/colour quality setters, raw flags and alias-sensitive copies."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='weather-colour-quality-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/b5/EngineWeatherColourQuality_00b50c60.cpp';fixture=ROOT/'rebuild/tests/integration/NativeWeatherColourQuality_test.cpp'
specs=[('CEngineWeatherRenderer',0xb50c60,22),('CEngineScreenEffectColourFilter',0xb5a0b0,32)]
symbols=['?SetEffectQuality@'+owner+'@@QAEXABVCEngineVideoOptionsDef@@@Z' for owner,_,_ in specs]
(w/'weather_colour_quality_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:@'+name+'@12='+symbol+'")' for name,symbol in zip(('Weather','Colour'),symbols)))
raw=EXE.read_bytes();body=read_virtual_data(raw,0xb50c60,22)+bytes(10)+read_virtual_data(raw,0xb5a0b0,32)
(w/'weather_colour_quality_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\n')
objects=[]
for n,p in enumerate((source,fixture)):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proofs=[check(objects[0],s,a,n,0,{}) for s,(_,a,n) in zip(symbols,specs)]
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0
with (w/'input.bin').open('wb') as f:
 for flag,support,bits,placement in itertools.product(range(256),(0,1,255),(0,0x3f800000,0x7f800001,0xffc12345),range(3)):
  a=bytearray([0x95])*16384;weather=(0x400,0x1000-0x69,0x1000-0x6c)[placement];colour=(0x600,0x2800-0x6a,0x2800-0x2e)[placement]
  a[weather+0x71]=flag;struct.pack_into('<I',a,weather+0x74,bits);a[colour+0x72]=(flag*73+17)%256;a[0x2844]=support
  f.write(struct.pack('<2I',weather,colour));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as f,(w/'compiled.bin').open('rb') as g:
 while True:
  a=f.read(1<<20);b=g.read(1<<20);assert a==b,w
  if not a:break
snapshots=cases*2;assert (w/'compiled.bin').stat().st_size==snapshots*16384
report=dict(cases=cases,snapshots=snapshots,omission_checks=1,providers=proofs,source_sha256={str(source.relative_to(ROOT)):hashlib.sha256(source.read_bytes()).hexdigest()},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Full16KiB arena after each actual setter;independently calculated byte changes. All256 flag bytes,three support bytes,four raw float patterns including signalling NaN,and separate/input-output/source-source aliasing. No callbacks or external boundaries.')
(ROOT/'rebuild/compile-gate/native-weather-colour-quality.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_WEATHER_COLOUR_QUALITY_GATE_PASS',snapshots,'snapshots')
