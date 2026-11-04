"""Actual engine pre-reset cleanup, virtual resource boundaries and mutable lists."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-engine-pre-reset-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/b2/CEngine_DeviceReset_00b25d00.cpp';fixture=ROOT/'rebuild/tests/integration/NativeEnginePreReset_test.cpp'
symbol='?OnPreDeviceReset@CEngine@@UAEXXZ';restore='?SmartRestoreComponentVideoMemory@CEngine@@AAE?AW4ESmartRestoreComponentVideoMemoryResult@1@XZ'
(w/'engine_pre_reset_aliases.h').write_text('#pragma comment(linker,"/alternatename:@PreReset@8='+symbol+'")\n#pragma comment(linker,"/alternatename:'+restore+'=@RestoreTrap@8")\n')
body=read_virtual_data(EXE.read_bytes(),0xb26010,151);assert body[98]==0xe8 and (0xb26010+103+struct.unpack_from('<i',body,99)[0])==0xbfeae6
(w/'engine_pre_reset_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\n')
objects=[]
for n,p in enumerate((source,fixture)):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proof=check(objects[0],symbol,0xb26010,151,1,{'_memmove':0xbfeae6})
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0;BASE=0x24000000
with (w/'input.bin').open('wb') as f:
 for fill,mode,surfaces,components,nullmask in itertools.product((0,0x95),range(6),(0,1,3),(0,1,3),range(8)):
  a=bytearray([fill])*4096;struct.pack_into('<3I',a,0x14c,BASE+0x500,BASE+0x500+surfaces*4,BASE+0x510);struct.pack_into('<3I',a,0x168,BASE+0x700,BASE+0x700+components*4,BASE+0x710)
  for j in range(4):
   struct.pack_into('<I',a,0x500+4*j,0 if nullmask&(1<<j) else BASE+0x900+64*j)
   struct.pack_into('<I',a,0x600+4*j,BASE+0xa00+64*j)
   struct.pack_into('<I',a,0x700+4*j,BASE+0xc00+32*j)
   struct.pack_into('<I',a,0x800+4*j,BASE+0xc80+32*j)
  f.write(struct.pack('<I',mode));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
x=(w/'retail.bin').read_bytes();assert x==(w/'compiled.bin').read_bytes() and len(x)%4104==0,w;snapshots=len(x)//4104
report=dict(cases=cases,snapshots=snapshots,omission_checks=1,providers=[proof],source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Exact151B/onefixup and compiler-owned12byte secondary-base adjustment. Full4KiB snapshots at every virtual deletion/resource-release callback and final state. Empty/one/three arrays,null scratch pointers,delete flag checks,callbacks replacing/shrinking scratch storage or replacing/growing/shrinking component traversal. Real STLport clear and CRT memmove. Scalar surface deletion and component resource release remain explicit boundaries; post-reset restore trap is unused.')
(ROOT/'rebuild/compile-gate/native-engine-pre-reset.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_ENGINE_PRE_RESET_GATE_PASS',snapshots,'snapshots')
