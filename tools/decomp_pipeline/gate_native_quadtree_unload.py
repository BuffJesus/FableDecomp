"""Native quadtree request release and virtual child destruction."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-quadtree-unload-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/bd/CLocalDetailCacheMap_CQuadTreeElement_UnloadStaticContents_00bde020.cpp';fixture=ROOT/'rebuild/tests/integration/NativeQuadtreeUnload_test.cpp'
parent_source=ROOT/'rebuild/src/compiled/00/bd/CLocalDetailCacheMap_OnMapVisibilityChanged_00bde160.cpp'
parent='?OnMapVisibilityChanged@CLocalDetailCacheMap@@QAEX_N@Z';finish='?DeleteUnusedBlocks@CEngineStaticMapFileBlockManager@@QAEXXZ'
symbol='?UnloadStaticContents@CQuadTreeElement@CLocalDetailCacheMap@@QAEXPAVCEngineStaticMapFileBlockManager@@@Z'
release='?ReleaseRequest@CEngineStaticMapFileBlockManager@@QAEXJ@Z';remove='?RemoveObjectCollections@CQuadTreeElement@CLocalDetailCacheMap@@QAEXXZ'
targets={release:0xbe0400,remove:0xbdda50};aliases={'@Entry@12':symbol,release:'@Release@12',remove:'@Remove@4'}
aliases.update({'@DetailEntry@12':parent,finish:'@Finish@4'})
(w/'quadtree_unload_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
body=read_virtual_data(EXE.read_bytes(),0xbde020,87)
for fix,target in zip((18,37),targets.values()):assert body[fix-1]==0xe8 and 0xbde020+fix+4+struct.unpack_from('<i',body,fix)[0]==target
code=bytearray(192);code[:87]=body;body=read_virtual_data(EXE.read_bytes(),0xbde160,39);code[128:167]=body
for fix,target in ((19,0xbde020),(31,0xbe04f0)):assert body[fix-1]==0xe8 and 0xbde160+fix+4+struct.unpack_from('<i',body,fix)[0]==target
(w/'quadtree_unload_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(str(b) for b in code)+'};\n')
objects=[]
for i,p in enumerate([source,fixture,parent_source]):
 obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proof=check(objects[0],symbol,0xbde020,87,2,targets)
parent_proof=check(objects[2],parent,0xbde160,39,2,{symbol:0xbde020,finish:0xbe04f0})
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omitted in (0,2):
 r=subprocess.run(cmd+[str(p) for i,p in enumerate(objects) if i!=omitted],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs=bytearray();cases=0;base=0x24000000
for fill,mode,owner_offset,flags,mask,kind in itertools.product((0,0x55,0xff),range(6),(0x100,0x200),(0,4,8,12,0x83,0xff),range(16),range(3)):
 arena=bytearray([fill])*4096
 def put(at,value):struct.pack_into('<I',arena,at,value)
 for i in range(4):put(owner_offset+8+4*i,base+0x400+0x80*i if mask&(1<<i) else 0)
 for at in (0x400,0x480,0x500,0x580,0x700):put(at,0x25000000)
 arena[owner_offset+0x48]=flags;put(owner_offset+0x34,(0,1,0xffffffff,0x80000000,0x7fffffff,0x12345678)[mode])
 manager=1 if flags&8 else 0
 put(owner_offset-8,base+0x900 if manager else 0)
 inputs.extend(struct.pack('<4I',mode,owner_offset,manager,kind));inputs.extend(arena);cases+=1
(w/'input.bin').write_bytes(inputs)
for kind in ('retail','compiled'):
 r=subprocess.run([str(exe),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
actual=(w/'compiled.bin').read_bytes();expected=(w/'retail.bin').read_bytes();assert actual==expected,(w,'quadtree unload state');assert len(actual)%(4096+16)==0
snapshots=len(actual)//(4096+16)
headers=[ROOT/'rebuild/include/engine'/p for p in ('CLocalDetailCacheMap.h','CEngineStaticMapFileBlockManager.h')]
report=dict(cases=cases,snapshots=snapshots,omission_checks=2,providers=[proof,parent_proof],source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in [source,parent_source]+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Original87byte leaf and39byte parent natively executed with four audited direct-call fixups. All16 child masks,two owners,loading/static/unrelated flags,signed request IDs,two calls,callbacks change flags and upcoming child pointers. Direct unload plus actual detail notification hidden/visible composition. Full4KiB arena at request release,collection removal,each virtual deleting destructor,finished-block cleanup and return. Fixed-address external vtable;no snapshot normalization. Request/collection/child destruction and finished-block cleanup are recorded boundaries.')
(ROOT/'rebuild/compile-gate/native-quadtree-unload.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_QUADTREE_UNLOAD_GATE_PASS',snapshots,'snapshots')
