"""World/map unload with actual retail-empty landscape notification methods."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-game-map-unload-'));env=V.env()
paths=['00/b4/CEngineWorldMap_OnGameMapUnloaded_00b40230.cpp','00/b3/CEngineMap_OnGameMapUnloaded_00b3edc0.cpp','00/bd/CEngineLandscapeMap_GameMapNotifications_00bdbfe0.cpp']
sources=[ROOT/'rebuild/src/compiled'/p for p in paths];fixture=ROOT/'rebuild/tests/integration/NativeGameMapUnload_test.cpp'
world='?OnGameMapUnloaded@CEngineWorldMap@@QAEXJ@Z';map_symbol='?OnGameMapUnloaded@CEngineMap@@QAEXXZ';loaded='?OnGameMapLoaded@CEngineLandscapeMap@@QAEXXZ';unloaded='?OnGameMapUnloaded@CEngineLandscapeMap@@QAEXXZ'
aliases={'@World@12':world,'@Map@4':map_symbol,'@Loaded@4':loaded,'@Unloaded@4':unloaded}
(w/'game_map_unload_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();code=bytearray(113)
for a,n,offset,fix in ((0xb40230,33,0,18),(0xb3edc0,23,64,10),(0xbdbfe0,1,96,None),(0xbdbff0,1,112,None)):
 body=read_virtual_data(raw,a,n);code[offset:offset+n]=body
 if fix is not None:assert body[fix-1]==0xe8 and a+fix+4+struct.unpack_from('<i',body,fix)[0]==0xbdbff0
(w/'game_map_unload_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(str(b) for b in code)+'};\n')
objects=[]
for i,p in enumerate(sources+[fixture]):
 obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proofs=[check(objects[i],s,a,n,count,{unloaded:0xbdbff0}) for i,s,a,n,count in ((0,world,0xb40230,33,1),(1,map_symbol,0xb3edc0,23,1),(2,loaded,0xbdbfe0,1,0),(2,unloaded,0xbdbff0,1,0))]
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omit in range(3):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs=bytearray();expected=bytearray();cases=0;base=0x24000000
for fill,kind,index,landscape in itertools.product(range(256),range(4),range(2),range(2)):
 arena=bytearray([fill])*4096
 for off,value in ((0x20,base+0x200),(0x24,base+0x208),(0x28,base+0x208),(0x200,base+0x400),(0x204,base+0x500),(0x400,base+0x800 if landscape else 0),(0x500,base+0x800 if landscape else 0)):
  struct.pack_into('<I',arena,off,value)
 inputs.extend(struct.pack('<2I',kind,index));inputs.extend(arena)
 if kind<2:struct.pack_into('<I',arena,0x408+index*0x100,0)
 expected.extend(arena);expected.extend(arena);cases+=1
(w/'input.bin').write_bytes(inputs)
for kind in ('retail','compiled'):
 r=subprocess.run([str(exe),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
 assert (w/(kind+'.bin')).read_bytes()==expected,(w,kind,'independent unload-state expectation')
snapshots=cases*2
headers=[ROOT/'rebuild/include'/p for p in ('engine/CEngineMap.h','engine/CEngineWorldMap.h','candidates/landscape_background_lod_compiled.h')]
report=dict(cases=cases,snapshots=snapshots,omission_checks=3,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Four original bodies execute natively with two audited relocations. Actual empty landscape hooks compose in both world/map paths;all256fills,two map indices,null/non-null landscape and two repeated calls. Full4KiB snapshots equal independent expected arenas as well as native output. No callback substitutes or normalized bytes.')
(ROOT/'rebuild/compile-gate/native-game-map-unload.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_GAME_MAP_UNLOAD_GATE_PASS',snapshots,'snapshots')
