"""Nested STLport scene-map traversal and mutable next-map/grid ownership."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='scene-grid-invalidation-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/b9/CEngineScene_InvalidateBoundingBoxesAndDrawDistances_00b93500.cpp';fixture=ROOT/'rebuild/tests/integration/NativeSceneGridInvalidation_test.cpp'
grid_source=ROOT/'rebuild/src/compiled/00/b9/CEngineSceneGrid_InvalidateBoundingBoxAndDrawDistance_00b91ca0.cpp'
symbol='?InvalidateBoundingBoxesAndDrawDistances@CEngineScene@@QAEXXZ';child='?InvalidateBoundingBoxAndDrawDistance@CEngineSceneGrid@@QAEXKK_N@Z'
(w/'scene_grid_invalidation_aliases.h').write_text('#pragma comment(linker,"/alternatename:@InvalidateScene@8='+symbol+'")\n#pragma comment(linker,"/alternatename:'+child+'=@InvalidateGrid@20")\n')
body=read_virtual_data(EXE.read_bytes(),0xb93500,181)+bytes(11)+read_virtual_data(EXE.read_bytes(),0xb91ca0,122);assert body[46]==0xe8 and 0xb93500+51+struct.unpack_from('<i',body,47)[0]==0xb91ca0
(w/'scene_grid_invalidation_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\n')
objects=[]
for n,p in enumerate((source,grid_source,fixture)):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proof=check(objects[0],symbol,0xb93500,181,1,{child:0xb91ca0})
gridproof=check(objects[1],child,0xb91ca0,122,1,{child:0xb91ca0})
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in (0,1):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
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
  a=bytearray([0x95])*32768;ids=[];tree(a,0x100,[0x200+64*j for j in range(count)],outershape);struct.pack_into('<3I',a,4,BASE+0x100,count,0)
  for column in range(count):
   size=(pattern+column)%4;header=0x400+64*column;nodes=[0x600+48*(3*column+j) for j in range(size)]
   tree(a,header,nodes,innershape);struct.pack_into('<i3I',a,0x210+64*column,column-1,BASE+header,size,0)
   for j,node in enumerate(nodes):
    n=3*column+j;ids.append(n);struct.pack_into('<i2I',a,node+0x10,j-1,BASE+0x1400+64*n,0)
    for obj,vector,grid in ((0x1400+64*n,0x1800+16*n,0x2000+32*n),(0x3000+64*n,0x3400+16*n,0x3800+32*n)):
     struct.pack_into('<3I',a,obj+0x18,BASE+vector,BASE+vector+8,BASE+vector+8);struct.pack_into('<2I',a,vector,BASE+grid,0)
  expected=bytearray(a)
  for n in ids:
   root=0x2000+32*n;down=0x3000+32*n;rootarray=0x2400+8*n;childarray=0x3400+16*n;cell=0x4000+5*72*n
   root_exists=(pattern+n)%3!=0;has_children=bool(mode&1)
   struct.pack_into('<5I',a,root+0xc,BASE+down if has_children else 0,1,BASE+rootarray,BASE+rootarray+4,BASE+rootarray+4)
   struct.pack_into('<I',a,rootarray,BASE+cell if root_exists else 0)
   struct.pack_into('<5I',a,down+0xc,0,2,BASE+childarray,BASE+childarray+16,BASE+childarray+16)
   for j in range(5):a[cell+72*j+0x44]=(pattern*29+n*13+j*7+mode)%256
   for j in range(4):struct.pack_into('<I',a,childarray+4*j,BASE+cell+72*(j+1) if (pattern+n+j)%3 else 0)
  expected=bytearray(a)
  for n in ids:
   cell=0x4000+5*72*n
   if (pattern+n)%3!=0:
    expected[cell+0x44]&=0xfe
    if mode&1:
     for j in range(4):
      if (pattern+n+j)%3:expected[cell+72*(j+1)+0x44]&=0xfe
  f.write(struct.pack('<11I',len(ids),mode,*(ids+[0]*(9-len(ids)))));f.write(a);f.write(expected);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as f,(w/'compiled.bin').open('rb') as g:
 while True:
  a=f.read(1<<20);b=g.read(1<<20);assert a==b,w
  if not a:break
size=(w/'compiled.bin').stat().st_size;assert size%32776==0;snapshots=size//32776
report=dict(cases=cases,snapshots=snapshots,omission_checks=2,providers=[proof,gridproof],source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in (source,grid_source)},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual scene nested-map traversal calls actual recursive grid invalidation. Full32KiB arena compared with retail and independently expected flag changes. Zero-to-three outer/inner maps;balanced/left/right trees;missing roots/children,nullable lower levels and varied flags. Two original spans/twofixups,two mandatory TUs. No callback stand-ins remain in this composition.')
(ROOT/'rebuild/compile-gate/native-scene-grid-invalidation.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_SCENE_GRID_INVALIDATION_GATE_PASS',snapshots,'snapshots')
