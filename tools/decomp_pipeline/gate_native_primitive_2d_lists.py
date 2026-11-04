"""Grouped primitive intrusive-list insertion/removal, independently rebuilt graph."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-primitive-2d-lists-'));env=V.env();source=ROOT/'rebuild/src/compiled/00/b8/CEngineInternalPrimitiveBase_Lists_00b8fdf0.cpp';fixture=ROOT/'rebuild/tests/integration/NativePrimitive2DLists_test.cpp'
source2=ROOT/'rebuild/src/compiled/00/b4/CEnginePrimitiveRenderer2D_AddPrimitive_00b4a670.cpp'
route='?AddPrimitive@CEnginePrimitiveRenderer2D@@QAEXAAVCEngineInternalPrimitiveBase@@@Z'
add='?AddToList@CEngineInternalPrimitiveBase@@QAEXPAPAV1@@Z';remove='?RemoveFromList@CEngineInternalPrimitiveBase@@QAEXXZ'
(w/'primitive_2d_lists_aliases.h').write_text('#pragma comment(linker,"/alternatename:@Add@12='+route+'")\n#pragma comment(linker,"/alternatename:@Remove@8='+remove+'")\n')
raw=EXE.read_bytes();body=bytearray(278);body[:106]=read_virtual_data(raw,0xb8fdf0,106);body[112:228]=read_virtual_data(raw,0xb8fe60,116)
body[240:]=read_virtual_data(raw,0xb4a670,38)
(w/'primitive_2d_lists_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\n')
objects=[]
for n,p in enumerate((source,source2,fixture)):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proofs=[check(objects[0],add,0xb8fdf0,106,0,{}),check(objects[0],remove,0xb8fe60,116,0,{}),check(objects[1],route,0xb4a670,38,2,{add:0xb8fdf0})]
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(2):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0
with (w/'input.bin').open('wb') as f:
 for base,counts,mask,routeflags,cell in itertools.product((0,0x800),itertools.product(range(4),repeat=3),(0,1,0x80000000,0xfedcba98),(0,0x400,0xffffffff,0xfffffbff,0x80000400,0x80000000),range(2)):
  # Empty groups are fine; each nonempty group has a distinct layer mask.
  for selected in range(sum(counts)+1):
   flags=(0,1,2,3,255)[(sum(counts)+selected+routeflags)%5];local=(0,1,0x7fffffff)[(sum(counts)+selected)%3];f.write(struct.pack('<10I',base,*counts,mask,selected,flags,cell,local,routeflags));cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=120);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as f,(w/'compiled.bin').open('rb') as g:
 while True:
  a=f.read(1<<20);b=g.read(1<<20);assert a==b,w
  if not a:break
snapshots=cases*2;assert (w/'compiled.bin').stat().st_size==snapshots*4096
report=dict(cases=cases,snapshots=snapshots,omission_checks=2,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in (source,source2)},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual2D routing via bit0x400 into normal/displacement head,then actual AddToList/RemoveFromList with no callbacks;unselectedhead preserved. Independent graph reconstruction of grouped render-mask lists after insertion and arbitrary-node removal. Zero-to-nine existing nodes across three distinct groups,matching/new layer masks,all removal positions,two placements,five state flag bytes,optional sharedcell,count0/1/MAX_INT. Full4KiB snapshots include forward/back primitive links,group leader links,detached nodes,cellcount,all guards. Valid acyclic grouped lists;no invalid self-insertion or dangling pointers.')
(ROOT/'rebuild/compile-gate/native-primitive-2d-lists.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_PRIMITIVE_2D_LISTS_GATE_PASS',snapshots,'snapshots')
