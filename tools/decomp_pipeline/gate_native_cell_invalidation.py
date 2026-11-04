"""Real recursive grid invalidation, checked against independently selected cells."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='cell-invalidation-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/b9/CEngineSceneGridCell_InvalidateAll_00b91710.cpp';fixture=ROOT/'rebuild/tests/integration/NativeCellInvalidation_test.cpp'
symbol='?InvalidateAll@CEngineSceneGridCell@@QAEXXZ'
(w/'cell_invalidation_aliases.h').write_text('#pragma comment(linker,"/alternatename:@InvalidateCell@8='+symbol+'")\n')
body=read_virtual_data(EXE.read_bytes(),0xb91710,19)
(w/'cell_invalidation_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\n')
objects=[]
for n,p in enumerate((source,fixture)):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proof=check(objects[0],symbol,0xb91710,19,0,{})
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
BASE=0x24000000;cases=0;changed=0
with (w/'input.bin').open('wb') as f:
 for flags,depth,stop,order in itertools.product(range(256),range(1,6),range(6),range(2)):
  a=bytearray([0x95])*4096;offsets=[0x200+80*((3*j)%5 if order else j) for j in range(depth)]
  for j,offset in enumerate(offsets):
   value=(flags+19*j)%256
   if j==stop:value&=0xf8
   a[offset+0x44]=value;parent=BASE+offsets[j+1] if j+1<depth else 0
   if value&7==0:parent=0xdeadbeef
   struct.pack_into('<I',a,offset,parent)
  expected=bytearray(a)
  first_invalid=next((j for j,o in enumerate(offsets) if a[o+0x44]&7==0),depth)
  for offset in offsets[:first_invalid]:expected[offset+0x44]&=0xf8;changed+=1
  f.write(struct.pack('<I',offsets[0]));f.write(a);f.write(expected);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as f,(w/'compiled.bin').open('rb') as g:
 while True:
  a=f.read(1<<20);b=g.read(1<<20);assert a==b,w
  if not a:break
assert (w/'compiled.bin').stat().st_size==cases*4096
report=dict(cases=cases,snapshots=cases,selected_cells=changed,omission_checks=1,providers=[proof],source_sha256={str(source.relative_to(ROOT)):hashlib.sha256(source.read_bytes()).hexdigest()},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Full4KiB arena,all256flag patterns,one-to-five ancestors,six early-stop positions and two address orders. Independently expected ancestor prefix only clears low3bits;other flags preserved. Already invalid cells carry poisoned parent pointers to verify early return before dereference. No callbacks or external boundaries.')
(ROOT/'rebuild/compile-gate/native-cell-invalidation.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_CELL_INVALIDATION_GATE_PASS',cases,'snapshots')
