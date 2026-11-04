"""Native world-map visibility dispatch, including callback-mutated vectors."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-world-map-visibility-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/b4/CEngineWorldMap_Visibility_00b42390.cpp'
fixture=ROOT/'rebuild/tests/integration/NativeWorldMapVisibility_test.cpp'
boundary='?SetMapVisibility@CEngineMap@@QAEXAAV?$list@PAVCEngineMap@@V?$allocator@PAVCEngineMap@@@_STL@@@_STL@@_N@Z'
symbols=['?SetMapVisibility@CEngineWorldMap@@QAEXJ_N@Z','?SetMapVisibilityAllMaps@CEngineWorldMap@@QAEX_N@Z']
aliases={'@EntryOne@16':symbols[0],'@EntryAll@12':symbols[1],boundary:'@Boundary@16'}
(w/'world_map_visibility_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();code=bytearray(128)
for a,n,offset,fix in [(0xb42390,27,0,20),(0xb423b0,64,64,40)]:
 body=read_virtual_data(raw,a,n);assert body[fix-1]==0xe8 and a+fix+4+struct.unpack_from('<i',body,fix)[0]==0xb41db0
 code[offset:offset+n]=body
(w/'world_map_visibility_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(str(b) for b in code)+'};\n')
objects=[]
for i,p in enumerate([source,fixture]):
 obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proofs=[check(objects[0],s,a,n,1,{boundary:0xb41db0}) for s,a,n in zip(symbols,[0xb42390,0xb423b0],[27,64])]
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs=bytearray();cases=0;base=0x24000000
for fill,kind,mode,owner_offset,visible,pattern in itertools.product((0,0x55,0xa5,0xff),range(2),range(4),(0,0x100),range(2),range(8)):
 arena=bytearray([fill])*4096
 def put(at,value):struct.pack_into('<I',arena,at,value)
 count=pattern if kind else 6
 start=base+0x400
 put(owner_offset+0x20,start);put(owner_offset+0x24,start+4*count);put(owner_offset+0x28,start+32)
 for i in range(8):
  put(0x400+4*i,0 if kind and (i+pattern)%3==0 else base+0x800+32*i)
  put(0x600+4*i,0 if i%3==0 else base+0xa00+32*i)
 # A negative index is valid here because the vector start is deliberately advanced.
 index=pattern%6
 if not kind and pattern==7:
  put(owner_offset+0x20,start+8);put(owner_offset+0x24,start+24);index=0xffffffff
 inputs.extend(struct.pack('<5I',kind,mode,owner_offset,index,visible));inputs.extend(arena);cases+=1
(w/'input.bin').write_bytes(inputs)
for kind in ('retail','compiled'):
 r=subprocess.run([str(exe),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
actual=(w/'compiled.bin').read_bytes();expected=(w/'retail.bin').read_bytes();assert actual==expected,(w,'visibility callback state');assert len(actual)%(4096+16)==0
snapshots=len(actual)//(4096+16)
headers=[ROOT/'rebuild/include/engine/CEngineWorldMap.h',ROOT/'rebuild/include/engine/CEngineMap.h']
report=dict(cases=cases,snapshots=snapshots,omission_checks=1,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in [source]+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Retail bodies natively executed with audited call fixups. Full4KiB snapshots at every map callback and return. Empty/sparse/dense vectors,two owners,booleans,valid negative index,callbacks shrink/grow/replace vector. Actual map leaf remains recorded boundary.')
(ROOT/'rebuild/compile-gate/native-world-map-visibility.json').write_text(json.dumps(report,indent=2)+'\n')
print('NATIVE_WORLD_MAP_VISIBILITY_GATE_PASS',snapshots,'snapshots')
