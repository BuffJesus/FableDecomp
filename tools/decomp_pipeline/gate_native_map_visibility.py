"""Native visible-map list insertion/removal and child notifications."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
import check_one as V
from gate_game_component_abi import Coff
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-map-visibility-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/b4/CEngineMap_SetMapVisibility_00b41db0.cpp';fixture=ROOT/'rebuild/tests/integration/NativeMapVisibility_test.cpp'
owner_source=ROOT/'rebuild/src/compiled/00/b4/CEngineWorldMap_Visibility_00b42390.cpp'
symbol='?SetMapVisibility@CEngineMap@@QAEXAAV?$list@PAVCEngineMap@@V?$allocator@PAVCEngineMap@@@_STL@@@_STL@@_N@Z'
landscape='?OnMapVisibilityChanged@CEngineLandscapeMap@@QAEX_N@Z';detail='?OnMapVisibilityChanged@CLocalDetailCacheMap@@QAEX_N@Z'
targets={'_malloc':0xbfea0e,'_free':0xbfea14,landscape:0xbdc3c0,detail:0xbde160}
aliases={'@Entry@16':symbol,landscape:'@Landscape@12',detail:'@Detail@12'}
owners=['?SetMapVisibility@CEngineWorldMap@@QAEXJ_N@Z','?SetMapVisibilityAllMaps@CEngineWorldMap@@QAEX_N@Z']
aliases.update({'@OwnerOne@16':owners[0],'@OwnerAll@12':owners[1]})
(w/'map_visibility_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
body=read_virtual_data(EXE.read_bytes(),0xb41db0,147)
for fix,target in zip((33,95,123,136),targets.values()):assert body[fix-1]==0xe8 and 0xb41db0+fix+4+struct.unpack_from('<i',body,fix)[0]==target
code=bytearray(384);code[:147]=body
for a,n,offset,fix in [(0xb42390,27,256,20),(0xb423b0,64,320,40)]:
 b=read_virtual_data(EXE.read_bytes(),a,n);assert b[fix-1]==0xe8 and a+fix+4+struct.unpack_from('<i',b,fix)[0]==0xb41db0;code[offset:offset+n]=b
(w/'map_visibility_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(str(b) for b in code)+'};\n')
objects=[]
for i,(path,opts) in enumerate([(source,[]),(source,['/Dmalloc=MapMalloc','/Dfree=MapFree']),(fixture,[]),(owner_source,[])]):
 obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj)]+opts+[str(path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proof=check(objects[0],symbol,0xb41db0,147,4,targets)
owner_proofs=[check(objects[3],s,a,n,1,{symbol:0xb41db0}) for s,a,n in zip(owners,[0xb42390,0xb423b0],[27,64])]
original,renamed=Coff(objects[0]),Coff(objects[1]);sec,off=original.symbols[symbol];other,otheroff=renamed.symbols[symbol]
left,lr=original.sections[sec];right,rr=renamed.sections[other];rename={'_malloc':'_MapMalloc','_free':'_MapFree'}
assert left==right and off==otheroff and {p:(rename.get(t,t),k) for p,(t,k) in lr.items()}==rr
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omitted in (1,3):
 r=subprocess.run(cmd+[str(p) for i,p in enumerate(objects) if i!=0 and i!=omitted],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects[1:])),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs=bytearray();cases=0;base=0x24000000
for fill,mode,owner_offset,sequence,present,others,children,kind in itertools.product((0,0x55,0xff),range(3),(0,0x100),range(8),range(2),range(3),range(4),range(3)):
 arena=bytearray([fill])*4096
 def put(at,value):struct.pack_into('<I',arena,at,value)
 put(owner_offset,base+0xc00 if children&1 else 0);put(owner_offset+4,base+0xc80 if children&2 else 0)
 nodes=([0x500] if present else [])+[0x540+32*i for i in range(others)]
 chain=[0x400]+nodes
 for i,node in enumerate(chain):
  put(node,base+chain[(i+1)%len(chain)]);put(node+4,base+chain[(i-1)%len(chain)])
  if node!=0x400:put(node+8,base+owner_offset if node==0x500 else base+0xe00+32*i)
 put(0x300,base+0x400);put(owner_offset+0x58,base+(0x500 if present else 0x400));arena[owner_offset+0x64]=present
 put(0x2f4,base+0xf00);put(0x2f8,base+0xf0c);put(0x2fc,base+0xf10)
 put(0xf00,0);put(0xf04,base+owner_offset);put(0xf08,0)
 inputs.extend(struct.pack('<4I',mode,owner_offset,sequence,kind));inputs.extend(arena);cases+=1
(w/'input.bin').write_bytes(inputs)
for kind in ('retail','compiled'):
 r=subprocess.run([str(exe),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
actual=(w/'compiled.bin').read_bytes();expected=(w/'retail.bin').read_bytes();assert actual==expected,(w,'map visibility state');assert len(actual)%(4096+16)==0
snapshots=len(actual)//(4096+16)
headers=[ROOT/'rebuild/include'/p for p in ('engine/CEngineMap.h','candidates/landscape_background_lod_compiled.h','engine/CLocalDetailCacheMap.h','fable_stlport_list.h')]
report=dict(cases=cases,snapshots=snapshots,omission_checks=2,providers=[proof]+owner_proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in [source,owner_source]+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Retail leaf and two owner bodies execute natively after six audited call fixups. Allocator-renamed production body and relocations independently identical. Full4KiB state at allocation/free,landscape/detail callbacks and each of three returns. All eight visibility sequences,initial membership,zero/two other nodes,two map owners,child combinations and callback replacement/removal of second child. Direct leaf and actual single/all owner composition. CRT allocation/free and child notifications remain recorded boundaries.')
(ROOT/'rebuild/compile-gate/native-map-visibility.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_MAP_VISIBILITY_GATE_PASS',snapshots,'snapshots')
