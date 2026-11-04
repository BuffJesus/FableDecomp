"""Map shutdown ownership and actual world-map closing composition."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-map-close-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/b3/CEngineMap_CloseStaticMap_00b3ef40.cpp','00/b4/CEngineWorldMap_CloseStaticMapFile_00b40000.cpp','01/43/NEngineInternal_GPWaterRenderer_01436e54.cpp')];fixture=ROOT/'rebuild/tests/integration/NativeMapClose_test.cpp'
symbol='?CloseStaticMap@CEngineMap@@QAEXXZ';world='?CloseStaticMapFile@CEngineWorldMap@@QAEXXZ';landscape='?CloseStaticMap@CEngineLandscapeMap@@QAEXXZ';detail='?CloseStaticMap@CLocalDetailCacheMap@@QAEXXZ';water='?CloseStaticMap@CEngineWaterRenderer@@QAEXXZ';glob='?GPWaterRenderer@NEngineInternal@@3PAVCEngineWaterRenderer@@A'
aliases={'@Entry@4':symbol,'@World@4':world,'_WaterGlobal':glob,landscape:'@Landscape@4',detail:'@Detail@4',water:'@WaterClose@4'}
(w/'map_close_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();code=bytearray(236);body=read_virtual_data(raw,0xb3ef40,91);parent=read_virtual_data(raw,0xb40000,108);code[:91]=body;code[128:]=parent
for data,address,pairs in ((body,0xb3ef40,((17,0xbdc4f0),(29,0xbddd50),(61,0xbfe9bc))),(parent,0xb40000,((43,0xb3ef40),(73,0xb6db80)))):
 for fix,target in pairs:assert data[fix-1]==0xe8 and address+fix+4+struct.unpack_from('<i',data,fix)[0]==target
assert struct.unpack_from('<I',parent,63)[0]==0x1436e54
(w/'map_close_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(str(b) for b in code)+'};\n')
objects=[]
for i,p in enumerate(sources+[fixture]):
 obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
targets={landscape:0xbdc4f0,detail:0xbddd50,'??3@YAXPAX@Z':0xbfe9bc,symbol:0xb3ef40,water:0xb6db80,glob:0x1436e54}
proofs=[check(objects[0],symbol,0xb3ef40,91,3,targets),check(objects[1],world,0xb40000,108,3,targets)]
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omit in range(3):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs=bytearray();cases=0;base=0x24000000
for fill,mode,owner,land,local,refs,state,kind in itertools.product((0,0x55,0xff),range(6),(0x400,0x500),range(2),range(2),(None,0,1,2,0xffffffff),(0,1,2),range(2)):
 arena=bytearray([fill])*8192
 def put(off,value):struct.pack_into('<I',arena,off,value)
 put(0x20,base+0x200);put(0x24,base+0x208);put(0x28,base+0x208);put(0x200,0x1badb002);put(0x204,base+owner);put(0x34,base+0x300);put(0x1a8,1)
 put(owner,base+0x800 if land else 0);put(owner+4,base+0x900 if local else 0);put(owner+0x20,state);put(owner+0x3c,base+0xc00);put(owner+0x40,base+0xe00 if refs is not None else 0)
 put(0xe00,refs or 0);put(0xe08,base+0xc00);put(0xe40,1);put(0xe48,base+0xd00)
 inputs.extend(struct.pack('<3I',mode,owner,kind));inputs.extend(arena);cases+=1
(w/'input.bin').write_bytes(inputs)
for kind in ('retail','compiled'):
 r=subprocess.run([str(exe),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
actual=(w/'compiled.bin').read_bytes();expected=(w/'retail.bin').read_bytes();assert actual==expected,(w,'map shutdown ownership');assert len(actual)%(8192+12)==0
snapshots=len(actual)//(8192+12)
headers=[ROOT/'rebuild/include'/p for p in ('engine/CEngineMap.h','engine/CEngineWorldMap.h','engine/CLocalDetailCacheMap.h','candidates/landscape_background_lod_compiled.h','fable_counted_pointer.h')]
report=dict(cases=cases,snapshots=snapshots,omission_checks=3,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Original91byte map and108byte world close compose with six audited fixups,actual singleton. Direct/world two-call sequences,three fills,two owners,null/nonnull landscape/detail,control null/count0/1/2/ffffffff,three map states. Landscape changes detail;detail and DeleteFunc replace ownership block;operator delete overwrites owner before Reset clears Info/Data/open state. Full8KiB arena and singleton at callbacks/returns. Landscape/detail close,DeleteFunc/operator delete,and virtual bank close remain recorded boundaries;no normalization.')
(ROOT/'rebuild/compile-gate/native-map-close.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_MAP_CLOSE_GATE_PASS',snapshots,'snapshots')
