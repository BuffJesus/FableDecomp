"""Native collection chain deletion through actual quadtree/detail parents."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-quadtree-collections-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled/00/bd'/p for p in ('CLocalDetailCacheMap_CQuadTreeElement_RemoveObjectCollections_00bdda50.cpp','CLocalDetailCacheMap_CQuadTreeElement_UnloadStaticContents_00bde020.cpp','CLocalDetailCacheMap_OnMapVisibilityChanged_00bde160.cpp')]
fixture=ROOT/'rebuild/tests/integration/NativeQuadtreeCollections_test.cpp'
remove='?RemoveObjectCollections@CQuadTreeElement@CLocalDetailCacheMap@@QAEXXZ';unload='?UnloadStaticContents@CQuadTreeElement@CLocalDetailCacheMap@@QAEXPAVCEngineStaticMapFileBlockManager@@@Z';detail='?OnMapVisibilityChanged@CLocalDetailCacheMap@@QAEX_N@Z'
release='?ReleaseRequest@CEngineStaticMapFileBlockManager@@QAEXJ@Z';finish='?DeleteUnusedBlocks@CEngineStaticMapFileBlockManager@@QAEXXZ'
targets={remove:0xbdda50,unload:0xbde020,detail:0xbde160,release:0xbe0400,finish:0xbe04f0}
aliases={'@RemoveEntry@4':remove,'@UnloadEntry@12':unload,'@DetailEntry@12':detail,release:'@Release@12',finish:'@Finish@4'}
(w/'quadtree_collections_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
code=bytearray(320)
for address,n,offset,fixups in [(0xbdda50,44,0,()),(0xbde020,87,128,((18,0xbe0400),(37,0xbdda50))),(0xbde160,39,256,((19,0xbde020),(31,0xbe04f0)))]:
 body=read_virtual_data(EXE.read_bytes(),address,n);code[offset:offset+n]=body
 for fix,target in fixups:assert body[fix-1]==0xe8 and address+fix+4+struct.unpack_from('<i',body,fix)[0]==target
(w/'quadtree_collections_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(str(b) for b in code)+'};\n')
objects=[]
for i,p in enumerate(sources+[fixture]):
 obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proofs=[check(objects[i],s,a,n,count,targets) for i,(s,a,n,count) in enumerate([(remove,0xbdda50,44,0),(unload,0xbde020,87,2),(detail,0xbde160,39,2)])]
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omitted in range(3):
 r=subprocess.run(cmd+[str(p) for i,p in enumerate(objects) if i!=omitted],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs=bytearray();cases=0;base=0x24000000
for fill,mode,owner_offset,count,flags,kind in itertools.product((0,0x55,0xff),range(4),(0x100,0x200),range(6),(0,4,8,12,0xff),range(3)):
 arena=bytearray([fill])*4096
 def put(at,value):struct.pack_into('<I',arena,at,value)
 put(owner_offset+4,base+0x400 if count else 0)
 for i in range(count):
  at=0x400+0x60*i;put(at,0x25000000);put(at+4,base+at+0x60 if i+1<count else 0)
 for i in range(4):put(owner_offset+8+4*i,base+0xa00+0x80*i if i%2 else 0)
 for at in (0xa80,0xb80):put(at,0x25000004)
 put(owner_offset-8,base+0x900);put(owner_offset+0x34,0xffffffff);arena[owner_offset+0x48]=flags
 inputs.extend(struct.pack('<3I',mode,owner_offset,kind));inputs.extend(arena);cases+=1
(w/'input.bin').write_bytes(inputs)
for kind in ('retail','compiled'):
 r=subprocess.run([str(exe),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
actual=(w/'compiled.bin').read_bytes();expected=(w/'retail.bin').read_bytes();assert actual==expected,(w,'collection cleanup');assert len(actual)%(4096+12)==0
snapshots=len(actual)//(4096+12)
headers=[ROOT/'rebuild/include/engine'/p for p in ('CLocalDetailCacheMap.h','CEngineStaticMapFileBlockManager.h')]
report=dict(cases=cases,snapshots=snapshots,omission_checks=3,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Original44byte leaf and actual87/39byte parents execute natively with four audited direct fixups. Two-call sequences,chain lengths0..5,two owners,flags,destructors poison nodes/replace owner head/trim remaining chain. Full4KiB at every virtual deletion,request release,finished-block cleanup and return. Fixed-address external vtables;no snapshot normalization. Collection and child deleting destructors,request release and finished-block cleanup remain recorded boundaries.')
(ROOT/'rebuild/compile-gate/native-quadtree-collections.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_QUADTREE_COLLECTIONS_GATE_PASS',snapshots,'snapshots')
