"""Local-detail fade-distance option combinations, aliasing and x87 comparisons."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='local-distance-options-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/b5/CEngineLocalDetailGenerator_UpdateLoadingDistanceVideoOptions_00b59870.cpp';fixture=ROOT/'rebuild/tests/integration/NativeLocalDistanceOptions_test.cpp'
symbol='?UpdateLoadingDistanceVideoOptions@CEngineLocalDetailGenerator@@QAEXABVCEngineVideoOptionsDef@@@Z'
(w/'local_distance_options_aliases.h').write_text('#pragma comment(linker,"/alternatename:@UpdateOptions@12='+symbol+'")\n')
body=read_virtual_data(EXE.read_bytes(),0xb59870,321)
(w/'local_distance_options_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\n')
objects=[]
for n,p in enumerate((source,fixture)):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proof=check(objects[0],symbol,0xb59870,321,0,{})
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
def float_value(bits):return struct.unpack('<f',struct.pack('<I',bits))[0]
def word(a,offset):return struct.unpack_from('<I',a,offset)[0]
def put(a,offset,value):struct.pack_into('<I',a,offset,value)
patterns=(0,0x80000000,0x3f800000,0x40000000,0xbf800000,0x7f800000,0xff800000,0x7fc12345,0x7f812345,0xffc12345,1,0x7f7fffff);cases=0
with (w/'input.bin').open('wb') as f:
 for minimum,multiplier,repeated,base,alias,precision in itertools.product(patterns,patterns,patterns,(0x100,0x500),range(3),(0,0x10000,0x20000)):
  definition=(0x400,base,base-12)[alias];a=bytearray([0x95])*2048;put(a,definition+0x48,multiplier);put(a,definition+0x4c,minimum);put(a,definition+0x54,repeated);expected=bytearray(a);table=base+0x3c
  for j in range(8):put(expected,table+8*j,0);put(expected,table+8*j+4,0x3f800000)
  # Inputs may overlap the initialized table; preserve the native read/store order.
  put(expected,table+8,word(expected,definition+0x4c));put(expected,table+12,word(expected,definition+0x48));put(expected,table+16,0);put(expected,table+20,word(expected,definition+0x54));put(expected,table+32,0);put(expected,table+36,0x3f800000)
  # Single-channel entries retain raw bits. Combined entries are ordered maxima
  # above their 0/1 defaults; unordered operands never replace a value.
  channels={j:(word(expected,table+8*j),word(expected,table+8*j+4)) for j in (1,2,4)}
  for group in (3,5,6,7):
   for field,default in ((0,0),(1,0x3f800000)):
    best=default
    for channel in (4,2,1):
     if group&channel:
      value=channels[channel][field]
      if float_value(best)<float_value(value):best=value
    put(expected,table+8*group+4*field,best)
  f.write(struct.pack('<3I',base,definition,precision));f.write(a);f.write(expected);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as f,(w/'compiled.bin').open('rb') as g:
 while True:
  a=f.read(1<<20);b=g.read(1<<20);assert a==b,w
  if not a:break
assert (w/'compiled.bin').stat().st_size==cases*2052
report=dict(cases=cases,snapshots=cases,omission_checks=1,providers=[proof],source_sha256={str(source.relative_to(ROOT)):hashlib.sha256(source.read_bytes()).hexdigest()},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Full2KiB arena and x87 status word;independent expected combined-channel maxima. Twelve raw float patterns in each of three inputs including signed zero,infinities,quiet/signalling NaNs,denormal and maxfinite;two object locations,three input alias placements,24/53/64bit x87 precision. Actual bitmask aggregation executes;raw single-channel values retained,other memory unchanged. No callbacks.')
(ROOT/'rebuild/compile-gate/native-local-distance-options.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_LOCAL_DISTANCE_OPTIONS_GATE_PASS',cases,'snapshots')
