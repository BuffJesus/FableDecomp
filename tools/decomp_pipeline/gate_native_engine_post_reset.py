"""Native secondary-base engine reset callback and complete-object adjustment."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-engine-post-reset-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/b2/CEngine_DeviceReset_00b25d00.cpp';fixture=ROOT/'rebuild/tests/integration/NativeEnginePostReset_test.cpp'
symbol='?OnPostDeviceReset@CEngine@@UAE_NXZ';restore='?SmartRestoreComponentVideoMemory@CEngine@@AAE?AW4ESmartRestoreComponentVideoMemoryResult@1@XZ'
(w/'engine_post_reset_aliases.h').write_text('#pragma comment(linker,"/alternatename:@Reset@8='+symbol+'")\n#pragma comment(linker,"/alternatename:'+restore+'=@Restore@8")\n')
body=read_virtual_data(EXE.read_bytes(),0xb25d00,32);assert body[13]==0xe8 and (0xb25d00+18+struct.unpack_from('<i',body,14)[0])==0xb24500
(w/'engine_post_reset_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\n')
objects=[]
for n,p in enumerate((source,fixture)):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proof=check(objects[0],symbol,0xb25d00,32,1,{restore:0xb24500})
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=snapshots=0
with (w/'input.bin').open('wb') as f:
 for receiver,flag,value,mode in itertools.product((0,0x180),range(256),(0,1,2,3,0xffffffff,0x80000000),range(3)):
  a=bytearray([0x95])*1024;a[receiver+0xbd]=flag;f.write(struct.pack('<4I',receiver,value,mode,flag));f.write(a);cases+=1
  snapshots+=2+(0 if not flag else 1 if mode==1 else 2)
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
x=(w/'retail.bin').read_bytes();assert x==(w/'compiled.bin').read_bytes() and len(x)==snapshots*1032,w
report=dict(cases=cases,snapshots=snapshots,omission_checks=1,providers=[proof],source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Exact32B/onefixup, compiler-owned secondary CDeviceResetCallback adjustment12bytes. Full1KiB snapshots and callback/return events. All256 flag bytes, two object placements,six restore results,two calls with callback clearing/forcing reset flag. Callback independently checks complete engine receiver, result and call counts. SmartRestoreComponentVideoMemory remains explicit boundary.')
(ROOT/'rebuild/compile-gate/native-engine-post-reset.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_ENGINE_POST_RESET_GATE_PASS',snapshots,'snapshots')
