"""Native hidden map-info return, cached owner and descriptor string lifetime."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
import check_one as V
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-game-map-load-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/b4/CEngineWorldMap_OnGameMapLoaded_00b401e0.cpp','00/bd/CEngineLandscapeMap_GameMapNotifications_00bdbfe0.cpp','01/43/NEngineInternal_GPEngineWorldMap_01436e8c.cpp')];fixture=ROOT/'rebuild/tests/integration/NativeGameMapLoad_test.cpp'
symbol='?OnGameMapLoaded@CEngineWorldMap@@QAEXJ@Z';loaded='?OnGameMapLoaded@CEngineLandscapeMap@@QAEXXZ';global_symbol='?GPEngineWorldMap@NEngineInternal@@3PAVCEngineWorldMap@@A';destroy='??1CCharString@@QAE@XZ'
aliases={'@Entry@12':symbol,'_MapGlobal':global_symbol,destroy:'@DestroyString@4'}
(w/'game_map_load_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();body=read_virtual_data(raw,0xb401e0,67);code=bytearray(97);code[:67]=body;code[96:]=read_virtual_data(raw,0xbdbfe0,1)
assert struct.unpack_from('<I',body,9)[0]==0x1436e8c
for fix,target in ((47,0xbdbfe0),(56,0x99eae0)):assert body[fix-1]==0xe8 and 0xb401e0+fix+4+struct.unpack_from('<i',body,fix)[0]==target
(w/'game_map_load_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(str(b) for b in code)+'};\n')
objects=[]
for i,p in enumerate(sources+[fixture]):
 obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proofs=[check(objects[0],symbol,0xb401e0,67,3,{global_symbol:0x1436e8c,loaded:0xbdbfe0,destroy:0x99eae0}),check(objects[1],loaded,0xbdbfe0,1,0,{})]
assert coff_data(Coff(objects[2]),global_symbol,4)==read_virtual_data(raw,0x1436e8c,4)==bytes(4)
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omit in range(3):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs=bytearray();cases=0;base=0x24000000
for fill,mode,index,global_owner,pmap,string in itertools.product((0,0x55,0xaa,0xff),range(6),range(3),(0,0x200),(0,base+0xc00,base+0x400),(0,base+0xf00)):
 arena=bytearray([fill])*4096
 def put(off,value):struct.pack_into('<I',arena,off,value)
 put(0x20,base+0x30c);put(0x19c,base+0xa00);put(0x39c,base+0xb00)
 for i in range(3):put(0x308+i*4,base+0x400+i*0x100);put(0x33c+i*4,base+0x700);put(0x400+i*0x100,base+0x800)
 put(0xa00,base+0xd00);put(0xb00,base+0xd00);arena[0xe00:0xe03]=bytes((fill&1,(fill>>1)&1,(fill>>2)&1));put(0xe04,pmap);put(0xe08,string)
 inputs.extend(struct.pack('<3I',mode,index,global_owner));inputs.extend(arena);cases+=1
(w/'input.bin').write_bytes(inputs)
for kind in ('retail','compiled'):
 r=subprocess.run([str(exe),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
actual=(w/'compiled.bin').read_bytes();expected=(w/'retail.bin').read_bytes();assert actual==expected,(w,'hidden map result/lifetime');assert len(actual)==cases*(3*(4096+16)+32)
headers=[ROOT/'rebuild/include'/p for p in ('engine/CEngineMap.h','engine/CEngineWorldMap.h','engine/CIDrawWorldMap.h','candidates/landscape_background_lod_compiled.h','fable_string.h')]
report=dict(cases=cases,snapshots=cases*3,omission_checks=3,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Original67byte owner and actual retail-empty landscape loaded hook,three audited fixups and real singleton data. Hidden32byte descriptor fully supplied at virtual DrawGetMapInfo boundary;string destructor records complete initialized descriptor,without its stack address. Full4KiB arenas and singleton captured at callback,destructor,return. Signed indices-1/0/1 within allocated array,two singleton owners,null/alias map and string pointers;callback replaces array/global/map slot/landscape or overwrites cached map. Virtual map-info and string destructor remain boundaries;no arena normalization.')
(ROOT/'rebuild/compile-gate/native-game-map-load.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_GAME_MAP_LOAD_GATE_PASS',cases*3,'snapshots')
