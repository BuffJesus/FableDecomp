"""Nested STLport scene-map traversal and mutable next-map/grid ownership."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='scene-invalidation-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/b9/CEngineScene_InvalidateBoundingBoxesAndDrawDistances_00b93500.cpp';fixture=ROOT/'rebuild/tests/integration/NativeSceneInvalidation_test.cpp'
symbol='?InvalidateBoundingBoxesAndDrawDistances@CEngineScene@@QAEXXZ';child='?InvalidateBoundingBoxAndDrawDistance@CEngineSceneGrid@@QAEXKK_N@Z'
(w/'scene_invalidation_aliases.h').write_text('#pragma comment(linker,"/alternatename:@InvalidateScene@8='+symbol+'")\n#pragma comment(linker,"/alternatename:'+child+'=@InvalidateGrid@20")\n')
body=read_virtual_data(EXE.read_bytes(),0xb93500,181);assert body[46]==0xe8 and 0xb93500+51+struct.unpack_from('<i',body,47)[0]==0xb91ca0
(w/'scene_invalidation_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\n')
objects=[]
for n,p in enumerate((source,fixture)):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proof=check(objects[0],symbol,0xb93500,181,1,{child:0xb91ca0})
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
BASE=0x24000000
def tree(a,header,nodes,shape):
 def build(group,parent):
  if not group:return 0
  pivot=(len(group)//2,0,len(group)-1)[shape];node=group[pivot]
  left=build(group[:pivot],node);right=build(group[pivot+1:],node)
  struct.pack_into('<4I',a,node,1,BASE+parent,BASE+left if left else 0,BASE+right if right else 0);return node
 root=build(nodes,header);struct.pack_into('<4I',a,header,0,BASE+root if root else 0,BASE+(nodes[0] if nodes else header),BASE+(nodes[-1] if nodes else header))
cases=0
with (w/'input.bin').open('wb') as f:
 for count,pattern,outershape,innershape,mode in itertools.product(range(4),range(8),range(3),range(3),range(4)):
  a=bytearray([0x95])*16384;ids=[];tree(a,0x100,[0x200+64*j for j in range(count)],outershape);struct.pack_into('<3I',a,4,BASE+0x100,count,0)
  for column in range(count):
   size=(pattern+column)%4;header=0x400+64*column;nodes=[0x600+48*(3*column+j) for j in range(size)]
   tree(a,header,nodes,innershape);struct.pack_into('<i3I',a,0x210+64*column,column-1,BASE+header,size,0)
   for j,node in enumerate(nodes):
    n=3*column+j;ids.append(n);struct.pack_into('<i2I',a,node+0x10,j-1,BASE+0x1400+64*n,0)
    for obj,vector,grid in ((0x1400+64*n,0x1800+16*n,0x2000+32*n),(0x3000+64*n,0x3400+16*n,0x3800+32*n)):
     struct.pack_into('<3I',a,obj+0x18,BASE+vector,BASE+vector+8,BASE+vector+8);struct.pack_into('<2I',a,vector,BASE+grid,0)
  f.write(struct.pack('<11I',len(ids),mode,*(ids+[0]*(9-len(ids)))));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as f,(w/'compiled.bin').open('rb') as g:
 while True:
  a=f.read(1<<20);b=g.read(1<<20);assert a==b,w
  if not a:break
size=(w/'compiled.bin').stat().st_size;assert size%16392==0;snapshots=size//16392
report=dict(cases=cases,snapshots=snapshots,omission_checks=1,providers=[proof],source_sha256={str(source.relative_to(ROOT)):hashlib.sha256(source.read_bytes()).hexdigest()},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Full16KiB arena. Zero-to-three outer columns and inner maps;balanced,left-chain,right-chain topology independently selected. Exact callback order/receiver/zero coordinates/true recursion checked. Callbacks replace next map object or GridLevels array,or mutate keys. Actual shared STLport iterator increments execute;grid invalidation internals remain an explicit boundary.')
(ROOT/'rebuild/compile-gate/native-scene-invalidation.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_SCENE_INVALIDATION_GATE_PASS',snapshots,'snapshots')
