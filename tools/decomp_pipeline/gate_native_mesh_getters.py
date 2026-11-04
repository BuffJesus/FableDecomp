"""Native and independently expected mesh-bank lookup results."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-mesh-getters-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/a2/CMeshDataBank_MeshGetters_00a24280.cpp';fixture=ROOT/'rebuild/tests/integration/NativeMeshGetters_test.cpp'
symbols=['?PeekStaticMesh@CMeshDataBank@@QBEPBVC3DMeshInfo@@K@Z','?PeekRepeatedMesh@CMeshDataBank@@QBEPBVC3DMeshInfo@@K@Z']
(w/'mesh_getters_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in zip(['@Static@12','@Repeated@12'],symbols))+'\n')
body=read_virtual_data(EXE.read_bytes(),0xa24280,32)
(w/'mesh_getters_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\n')
objects=[]
for n,p in enumerate((source,fixture)):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proof=[check(objects[0],s,a,16,0,{}) for s,a in zip(symbols,(0xa24280,0xa24290))]
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0;base=0x24000000
with (w/'input.bin').open('wb') as f:
 for fill,receiver,storage,index,value in itertools.product((0,0x95),(0,0x400),(0x900,0xa00),(0,1,3,15),(0,1,0x24000c00,0xffffffff)):
  a=bytearray([fill])*4096;struct.pack_into('<I',a,receiver+0x380,base+storage);struct.pack_into('<I',a,storage+index*4,value)
  f.write(struct.pack('<3I',receiver,index,value));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
assert (w/'retail.bin').read_bytes()==(w/'compiled.bin').read_bytes()
report=dict(cases=cases,snapshots=cases*2,omission_checks=1,providers=proof,source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Two16byte original bodies with no fixups or external boundaries. Different receiver/storage addresses and indices, null/arbitrary returned pointer bits; both getters checked against independent expected values and full4KiB memory unchanged.')
(ROOT/'rebuild/compile-gate/native-mesh-getters.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_MESH_GETTERS_GATE_PASS',cases*2,'snapshots')
