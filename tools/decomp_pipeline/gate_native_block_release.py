"""Native signed block-map lookup and actual release composition."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
import check_one as V
from gate_game_component_abi import Coff
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-block-release-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled/00/be'/p for p in ('CEngineStaticMapFileBlockManager_ReleaseRequest_00be0400.cpp','STLStaticMapBlockTree_find_00be08e0.cpp')];fixture=ROOT/'rebuild/tests/integration/NativeBlockRelease_test.cpp'
release='?ReleaseRequest@CEngineStaticMapFileBlockManager@@QAEXJ@Z';objects=[]
for i,p in enumerate(sources):
 obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
find=next(s for s in Coff(objects[1]).symbols if s.startswith('?find@?$_Rb_tree@J'))
aliases={'@ReleaseEntry@12':release,'@FindEntry@16':find}
(w/'block_release_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();body=read_virtual_data(raw,0xbe0400,29);assert body[14]==0xe8 and 0xbe0413+struct.unpack_from('<i',body,15)[0]==0xbe08e0
code=bytearray(144);code[:29]=body;code[64:135]=read_virtual_data(raw,0xbe08e0,71)
(w/'block_release_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(str(b) for b in code)+'};\n')
obj=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proofs=[check(objects[0],release,0xbe0400,29,1,{find:0xbe08e0}),check(objects[1],find,0xbe08e0,71,0,{})]
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omitted in range(2):
 r=subprocess.run(cmd+[str(p) for i,p in enumerate(objects) if i!=omitted],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs=bytearray();cases=0;base=0x24000000;keys=[-2147483648,-1,0,1,2,2147483646,2147483647]
queries=[-2147483648,-2147483647,-2,-1,0,1,2,3,2147483645,2147483646,2147483647]
def balanced(indices):
 if not indices:return []
 mid=len(indices)//2;return [indices[mid]]+balanced(indices[:mid])+balanced(indices[mid+1:])
for fill,kind,owner,count,shape,key,refs in itertools.product((0,0x55,0xff),range(2),(0,0x100),(0,1,3,7),range(3),queries,(0,1,2,0x7fffffff,0xffffffff)):
 for destination in ([0xf20,0xf00,owner+8,0x510] if not kind else [0xf20]):
  arena=bytearray([fill])*4096
  def put(at,value):struct.pack_into('<I',arena,at,value&0xffffffff)
  put(owner+8,base+0x400);put(owner+12,count);put(0x404,0);put(0x408,base+0x400);put(0x40c,base+0x400);put(0x414,refs)
  order=list(range(count)) if shape==0 else list(reversed(range(count))) if shape==1 else balanced(list(range(count)))
  root=None;children={i:[None,None] for i in range(count)};parents={}
  for i in order:
   if root is None:root=i;parents[i]=None;continue
   node=root
   while True:
    side=int(keys[i]>keys[node])
    if children[node][side] is None:children[node][side]=i;parents[i]=node;break
    node=children[node][side]
  if root is not None:
   put(0x404,base+0x500+root*0x40);put(0x408,base+0x500);put(0x40c,base+0x500+(count-1)*0x40)
  for i in range(count):
   at=0x500+i*0x40;put(at+4,base+0x500+parents[i]*0x40 if parents[i] is not None else base+0x400)
   for side,offset in enumerate((8,12)):put(at+offset,base+0x500+children[i][side]*0x40 if children[i][side] is not None else 0)
   put(at+16,keys[i]);put(at+20,refs)
  put(0xf00,key);inputs.extend(struct.pack('<4I',kind,owner,0xf00,destination));inputs.extend(arena);cases+=1
(w/'input.bin').write_bytes(inputs)
for kind in ('retail','compiled'):
 r=subprocess.run([str(exe),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
actual=(w/'compiled.bin').read_bytes();expected=(w/'retail.bin').read_bytes();assert actual==expected,(w,'block lookup/release');assert len(actual)==cases*4100
headers=[ROOT/'rebuild/include'/p for p in ('engine/CEngineStaticMapFileBlockManager.h','fable_stlport_tree.h','fable_counted_pointer.h')]
report=dict(cases=cases,snapshots=cases,omission_checks=2,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Original29byte release and complete71byte/two-return lookup execute natively,one audited relative call. Full4KiB state plus hidden-result pointer. Empty/single/three/seven-node trees,ascending/descending/balanced shapes,signed-key extremes/hits/misses,five count patterns,two owners. Direct lookup hidden output aliases key,tree header slot or node key;actual release composes lookup. Sentinel storage mapped to observe original unchecked absent-key behavior. No callback boundaries or normalized bytes.')
(ROOT/'rebuild/compile-gate/native-block-release.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_BLOCK_RELEASE_GATE_PASS',cases,'snapshots')
