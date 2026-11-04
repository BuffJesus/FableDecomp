"""Local-detail visibility cleanup with callback-replaced block manager."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-detail-map-visibility-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/bd/CLocalDetailCacheMap_OnMapVisibilityChanged_00bde160.cpp';fixture=ROOT/'rebuild/tests/integration/NativeDetailMapVisibility_test.cpp'
symbol='?OnMapVisibilityChanged@CLocalDetailCacheMap@@QAEX_N@Z'
unload='?UnloadStaticContents@CQuadTreeElement@CLocalDetailCacheMap@@QAEXPAVCEngineStaticMapFileBlockManager@@@Z';finish='?DeleteUnusedBlocks@CEngineStaticMapFileBlockManager@@QAEXXZ'
targets={unload:0xbde020,finish:0xbe04f0};aliases={'@Entry@12':symbol,unload:'@Unload@12',finish:'@Finish@4'}
(w/'detail_map_visibility_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
body=read_virtual_data(EXE.read_bytes(),0xbde160,39)
for fix,target in zip((19,31),targets.values()):assert body[fix-1]==0xe8 and 0xbde160+fix+4+struct.unpack_from('<i',body,fix)[0]==target
(w/'detail_map_visibility_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(str(b) for b in body)+'};\n')
objects=[]
for i,p in enumerate([source,fixture]):
 obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proof=check(objects[0],symbol,0xbde160,39,2,targets)
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs=bytearray();cases=0;base=0x24000000
for fill,mode,owner_offset,visible,manager in itertools.product(range(256),range(3),(0,0x100),range(2),(0,0x800)):
 arena=bytearray([fill])*4096;struct.pack_into('<I',arena,owner_offset+4,base+manager if manager else 0)
 inputs.extend(struct.pack('<3I',mode,owner_offset,visible));inputs.extend(arena);cases+=1
(w/'input.bin').write_bytes(inputs)
for kind in ('retail','compiled'):
 r=subprocess.run([str(exe),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
actual=(w/'compiled.bin').read_bytes();expected=(w/'retail.bin').read_bytes();assert actual==expected,(w,'detail visibility state');assert len(actual)%(4096+12)==0
snapshots=len(actual)//(4096+12)
headers=[ROOT/'rebuild/include/engine'/p for p in ('CLocalDetailCacheMap.h','CEngineStaticMapFileBlockManager.h')]
report=dict(cases=cases,snapshots=snapshots,omission_checks=1,providers=[proof],source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in [source]+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Original39byte body executes natively with two audited call relocations. All256fill bytes,two owners,null/non-null manager,both booleans,unload callback replaces or clears manager. Full4KiB snapshots at both callbacks and return. UnloadStaticContents and DeleteUnusedBlocks remain recorded boundaries.')
(ROOT/'rebuild/compile-gate/native-detail-map-visibility.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_DETAIL_MAP_VISIBILITY_GATE_PASS',snapshots,'snapshots')
