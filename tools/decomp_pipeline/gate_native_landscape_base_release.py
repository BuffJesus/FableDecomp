"""Correctly owned landscape base-request release and callback mutation order."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-landscape-base-release-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/bd/CEngineLandscapeMap_ReleaseLoadingPatchTreeBase_00bdc000.cpp';fixture=ROOT/'rebuild/tests/integration/NativeLandscapeBaseRelease_test.cpp'
symbol='?ReleaseLoadingPatchTreeBase@CEngineLandscapeMap@@AAEXXZ';release='?ReleaseRequest@CEngineStaticMapFileBlockManager@@QAEXJ@Z'
aliases={'@Entry@4':symbol,release:'@Release@12'}
(w/'landscape_base_release_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
body=read_virtual_data(EXE.read_bytes(),0xbdc000,28);assert body[17]==0xe8 and 0xbdc016+struct.unpack_from('<i',body,18)[0]==0xbe0400
(w/'landscape_base_release_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(str(b) for b in body)+'};\n')
objects=[]
for i,p in enumerate([source,fixture]):
 obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proof=check(objects[0],symbol,0xbdc000,28,1,{release:0xbe0400})
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs=bytearray();cases=0;base=0x24000000
for fill,mode,owner,flag,key,manager in itertools.product((0,0x55,0xaa,0xff),range(4),(0,0x100),range(2),(0,1,0x7fffffff,0x80000000,0xffffffff),(0,0x800,0xa00)):
 arena=bytearray([fill])*4096;struct.pack_into('<I',arena,owner+0x20,base+manager if manager else 0);struct.pack_into('<I',arena,owner+0x30,key);arena[owner+0x5e]=flag
 inputs.extend(struct.pack('<2I',mode,owner));inputs.extend(arena);cases+=1
(w/'input.bin').write_bytes(inputs)
for kind in ('retail','compiled'):
 r=subprocess.run([str(exe),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
actual=(w/'compiled.bin').read_bytes();expected=(w/'retail.bin').read_bytes();assert actual==expected,(w,'base release');assert len(actual)%(4096+12)==0
snapshots=len(actual)//(4096+12)
headers=[ROOT/'rebuild/include'/p for p in ('candidates/landscape_background_lod_compiled.h','engine/CEngineStaticMapFileBlockManager.h')]
report=dict(cases=cases,snapshots=snapshots,omission_checks=1,providers=[proof],source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in [source]+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Original28byte method natively executed with one audited request-release relocation. Two-call sequences,four fills,two owners,both flags,five signed key edges,three managers;callback reasserts flag,changes manager/key or overwrites owner. Full4KiB snapshots at release and both returns. ReleaseRequest remains recorded boundary;no normalized arena bytes.')
(ROOT/'rebuild/compile-gate/native-landscape-base-release.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_LANDSCAPE_BASE_RELEASE_GATE_PASS',snapshots,'snapshots')
