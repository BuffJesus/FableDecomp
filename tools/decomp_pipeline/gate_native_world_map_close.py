"""World-map shutdown iteration, virtual bank close, callback mutations and flags."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
import check_one as V
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-world-map-close-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/b4/CEngineWorldMap_CloseStaticMapFile_00b40000.cpp','01/43/NEngineInternal_GPWaterRenderer_01436e54.cpp')];fixture=ROOT/'rebuild/tests/integration/NativeWorldMapClose_test.cpp'
symbol='?CloseStaticMapFile@CEngineWorldMap@@QAEXXZ';mapclose='?CloseStaticMap@CEngineMap@@QAEXXZ';waterclose='?CloseStaticMap@CEngineWaterRenderer@@QAEXXZ';global_symbol='?GPWaterRenderer@NEngineInternal@@3PAVCEngineWaterRenderer@@A'
aliases={'@Entry@4':symbol,'_WaterGlobal':global_symbol,mapclose:'@MapClose@4',waterclose:'@WaterClose@4'}
(w/'world_map_close_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();body=read_virtual_data(raw,0xb40000,108)
assert struct.unpack_from('<I',body,63)[0]==0x1436e54
for fix,target in ((43,0xb3ef40),(73,0xb6db80)):assert body[fix-1]==0xe8 and 0xb40000+fix+4+struct.unpack_from('<i',body,fix)[0]==target
(w/'world_map_close_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(str(b) for b in body)+'};\n')
objects=[]
for i,p in enumerate(sources+[fixture]):
 obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proof=check(objects[0],symbol,0xb40000,108,3,{global_symbol:0x1436e54,mapclose:0xb3ef40,waterclose:0xb6db80})
assert coff_data(Coff(objects[1]),global_symbol,4)==read_virtual_data(raw,0x1436e54,4)==bytes(4)
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omit in range(2):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs=bytearray();cases=0;base=0x24000000
for fill,mode,count,mask,state,water in itertools.product((0,0x55,0xaa,0xff),range(6),range(5),(0,1,2,5,10,15),(0,1,2,0xffffffff),(0,0xd00,0xe00)):
 arena=bytearray([fill])*4096
 def put(off,value):struct.pack_into('<I',arena,off,value)
 put(0x20,base+0x600);put(0x24,base+0x600+4*count);put(0x28,base+0x610);put(0x34,base+0xf00);put(0x1a8,state)
 for i in range(4):put(0x600+i*4,base+0x800+i*0x80 if mask&(1<<i) else 0);put(0x700+i*4,base+0xa00+i*0x80 if mask&(1<<i) else 0)
 inputs.extend(struct.pack('<2I',mode,water));inputs.extend(arena);cases+=1
(w/'input.bin').write_bytes(inputs)
for kind in ('retail','compiled'):
 r=subprocess.run([str(exe),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
actual=(w/'compiled.bin').read_bytes();expected=(w/'retail.bin').read_bytes();assert actual==expected,(w,'world-map shutdown');assert len(actual)%(4096+12)==0
snapshots=len(actual)//(4096+12)
headers=[ROOT/'rebuild/include'/p for p in ('engine/CEngineMap.h','engine/CEngineWorldMap.h','engine/CStaticMapBankFile.h','engine/CEngineWaterRenderer.h','fable_engine_internal.h','fable_game_text_bank.h')]
report=dict(cases=cases,snapshots=snapshots,omission_checks=2,providers=[proof],source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Original108byte owner executes natively with three audited operands and real water-renderer singleton. Two-call sequences,four fills,0..4 map slots,six null masks,four open states,three water targets. Map callbacks shrink/grow/replace vector or clear/replace singleton;water callback replaces bank vtable;bank callbacks reassert states before final clear. Full4KiB arena and singleton snapshots;map/water/virtual bank Close remain recorded boundaries,no normalization.')
(ROOT/'rebuild/compile-gate/native-world-map-close.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_WORLD_MAP_CLOSE_GATE_PASS',snapshots,'snapshots')
