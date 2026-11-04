"""Real recursive grid invalidation, checked against independently selected cells."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='grid-invalidation-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/b9/CEngineSceneGrid_InvalidateBoundingBoxAndDrawDistance_00b91ca0.cpp';fixture=ROOT/'rebuild/tests/integration/NativeGridInvalidation_test.cpp'
symbol='?InvalidateBoundingBoxAndDrawDistance@CEngineSceneGrid@@QAEXKK_N@Z'
(w/'grid_invalidation_aliases.h').write_text('#pragma comment(linker,"/alternatename:@InvalidateGrid@20='+symbol+'")\n')
body=read_virtual_data(EXE.read_bytes(),0xb91ca0,122);assert body[90]==0xe8 and 0xb91ca0+95+struct.unpack_from('<i',body,91)[0]==0xb91ca0
(w/'grid_invalidation_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\n')
objects=[]
for n,p in enumerate((source,fixture)):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proof=check(objects[0],symbol,0xb91ca0,122,1,{symbol:0xb91ca0})
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
BASE=0x24000000;cases=0;changed=0
with (w/'input.bin').open('wb') as f:
 for flags,holes,depth,children in itertools.product(range(256),range(4),(1,2,3),(0,1,255)):
  a=bytearray([0x95])*16384;x=(flags+holes)%2;y=(flags//2+holes)%2;present={};cells={}
  for level in range(depth):
   side=2<<level;grid=0x100+32*level;array=0x400+256*level
   struct.pack_into('<I',a,grid+0xc,BASE+grid+32 if level+1<depth else 0);struct.pack_into('<4I',a,grid+0x10,side,BASE+array,BASE+array+4*side*side,BASE+array+4*side*side)
   for cy in range(side):
    for cx in range(side):
     index=cy*side+cx;cell=0x1000+72*((0,4,20)[level]+index)
     exists=holes==0 or (holes==1 and (cx+cy+level)%2==0) or (holes==2 and level>0) or (holes==3 and (cx+2*cy+level)%3!=0)
     present[level,cx,cy]=exists;cells[level,cx,cy]=cell;a[cell+0x44]=(flags+13*index+level)%256
     struct.pack_into('<I',a,array+4*index,BASE+cell if exists else 0)
  expected=bytearray(a)
  # A cell is selected iff it lies under the chosen root and every ancestor exists.
  for (level,cx,cy),cell in cells.items():
   if level and not children:continue
   if (cx>>level,cy>>level)!=(x,y):continue
   if all(present[parent,cx>>(level-parent),cy>>(level-parent)] for parent in range(level+1)):
    expected[cell+0x44]&=0xfe;changed+=1
  f.write(struct.pack('<3I',x,y,children));f.write(a);f.write(expected);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as f,(w/'compiled.bin').open('rb') as g:
 while True:
  a=f.read(1<<20);b=g.read(1<<20);assert a==b,w
  if not a:break
assert (w/'compiled.bin').stat().st_size==cases*16384
report=dict(cases=cases,snapshots=cases,selected_cells=changed,omission_checks=1,providers=[proof],source_sha256={str(source.relative_to(ROOT)):hashlib.sha256(source.read_bytes()).hexdigest()},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Full16KiB arena compared with retail and independently expected cells selected by coordinate ancestry. All256 initial flag values,four missing-cell patterns,one-to-three grid levels,four root coordinates and enabled/disabled recursion. Actual recursion executes;no callbacks or external boundaries. All other flag bits and arena bytes preserved.')
(ROOT/'rebuild/compile-gate/native-grid-invalidation.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_GRID_INVALIDATION_GATE_PASS',cases,'snapshots')
