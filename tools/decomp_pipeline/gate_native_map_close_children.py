"""Landscape/detail close and actual quad-tree clear lifecycle."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-map-close-children-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled/00/bd'/p for p in ('CLocalDetailCacheMap_CQuadTreeElement_Clear_00bddc00.cpp','CLocalDetailCacheMap_CloseStaticMap_00bddd50.cpp','CEngineLandscapeMap_CloseStaticMap_00bdc4f0.cpp')];fixture=ROOT/'rebuild/tests/integration/NativeMapCloseChildren_test.cpp'
quad='?Clear@CQuadTreeElement@CLocalDetailCacheMap@@QAEXPAVCEngineStaticMapFileBlockManager@@@Z';detail='?CloseStaticMap@CLocalDetailCacheMap@@QAEXXZ';land='?CloseStaticMap@CEngineLandscapeMap@@QAEXXZ';release='?ReleaseRequest@CEngineStaticMapFileBlockManager@@QAEXJ@Z';clear='?Clear@CEngineLandscapeMap@@QAEXXZ'
aliases={'@Entry@12':quad,'@Detail@4':detail,'@Landscape@4':land,release:'@Release@12',clear:'@LandscapeClear@4'}
(w/'map_close_children_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();code=bytearray(177)
for a,n,offset,fix,target in ((0xbddc00,127,0,19,0xbe0400),(0xbddd50,24,128,11,0xbddc00),(0xbdc4f0,17,160,4,0xbdc450)):
 body=read_virtual_data(raw,a,n);code[offset:offset+n]=body;assert body[fix-1]==0xe8 and a+fix+4+struct.unpack_from('<i',body,fix)[0]==target
(w/'map_close_children_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(str(b) for b in code)+'};\n')
objects=[]
for i,p in enumerate(sources+[fixture]):
 obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
targets={quad:0xbddc00,release:0xbe0400,clear:0xbdc450}
proofs=[check(objects[i],s,a,n,1,targets) for i,s,a,n in ((0,quad,0xbddc00,127),(1,detail,0xbddd50,24),(2,land,0xbdc4f0,17))]
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omit in range(3):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs=bytearray();cases=0;base=0x24000000
specs=list(itertools.product((0,0xff),range(6),range(2),(0,4,8,12,0x83,0xff),range(16),(0,1,3)))
specs.extend((fill,mode,2,0,0,0) for fill,mode in itertools.product(range(256),range(6)))
for fill,mode,kind,flags,mask,length in specs:
 arena=bytearray([fill])*4096
 def put(off,value):struct.pack_into('<I',arena,off,value)
 put(0xf8,base+0xa00 if flags&8 else 0);put(0x134,0x80000000+mode);arena[0x148]=flags
 for i in range(4):put(0x108+i*4,base+0x400+i*0x80 if mask&(1<<i) else 0)
 for off in (0x400,0x480,0x500,0x580,0x700):put(off,0x25000000)
 put(0x104,base+0xc00 if length else 0)
 for i in range(3):put(0xc00+i*0x80,0x25000004);put(0xc04+i*0x80,base+0xc00+(i+1)*0x80 if i+1<length else 0)
 inputs.extend(struct.pack('<3I',mode,kind,1 if flags&8 else 0));inputs.extend(arena);cases+=1
(w/'input.bin').write_bytes(inputs)
for kind in ('retail','compiled'):
 r=subprocess.run([str(exe),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
actual=(w/'compiled.bin').read_bytes();expected=(w/'retail.bin').read_bytes();assert actual==expected,(w,'quad-tree clear/close state');assert len(actual)%(4096+12)==0
snapshots=len(actual)//(4096+12)
headers=[ROOT/'rebuild/include'/p for p in ('engine/CLocalDetailCacheMap.h','engine/CEngineStaticMapFileBlockManager.h','candidates/landscape_background_lod_compiled.h')]
report=dict(cases=cases,snapshots=snapshots,omission_checks=3,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Three original bodies natively execute with three audited calls. Actual quad clear composes in detail close. Two-call sequences,all16child masks,0/1/3collection nodes,loading/static/unrelated flags,callback changes flags,later child slots,current collection head and upcoming links. Landscape Clear overwrites owner before manager is zeroed;all256fill bytes for landscape. Full4KiB snapshots at release,deleting destructors,landscape Clear and returns. Request release,virtual deletion and landscape Clear remain boundaries;no normalization.')
(ROOT/'rebuild/compile-gate/native-map-close-children.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_MAP_CLOSE_CHILDREN_GATE_PASS',snapshots,'snapshots')
