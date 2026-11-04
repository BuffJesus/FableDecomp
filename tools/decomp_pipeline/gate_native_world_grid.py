"""Native world-grid geometry, allocation and cell linking."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-world-grid-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/b4/CEngineWorldMap_AutoBuildWorldMapGrid_00b40070.cpp','00/b3/CEngineWorldMap_LinkMapToWorld_00b3edf0.cpp','00/b3/CEngineWorldMap_UnlinkMapFromWorld_00b3ee80.cpp')];fixture=ROOT/'rebuild/tests/integration/NativeWorldGrid_test.cpp'
unlink='?UnlinkMapFromWorld@CEngineWorldMap@@AAEXPAVCEngineMap@@J@Z'
build='?AutoBuildWorldMapGrid@CEngineWorldMap@@AAEXXZ';link='?LinkMapToWorld@CEngineWorldMap@@AAEXPAVCEngineMap@@J@Z';targets={'??_U@YAPAXI@Z':0xbfeb22,link:0xb3edf0}
(w/'world_grid_aliases.h').write_text('#pragma comment(linker,"/alternatename:@Build@4='+build+'")\n#pragma comment(linker,"/alternatename:@Link@16='+link+'")\n#pragma comment(linker,"/alternatename:@Unlink@16='+unlink+'")\n')
raw=EXE.read_bytes();body=read_virtual_data(raw,0xb40070,361);code=bytearray(663);code[:361]=body;code[384:515]=read_virtual_data(raw,0xb3edf0,131);code[544:]=read_virtual_data(raw,0xb3ee80,119)
for off,target in [(264,0xbfeb22),(335,0xb3edf0)]:assert body[off-1]==0xe8 and 0xb40070+off+4+struct.unpack_from('<i',body,off)[0]==target
(w/'world_grid_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,code))+'};\n')
objects=[]
for i,p in enumerate(sources+[fixture]):
 o=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proofs=[check(objects[0],build,0xb40070,361,2,targets),check(objects[1],link,0xb3edf0,131,0,{}),check(objects[2],unlink,0xb3ee80,119,0,{})]
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(3):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
base=0x24000000;cases=snapshots=0
with (w/'input.bin').open('wb') as f:
 for origin,width,mask,mode,existing in itertools.product((0,31,32,33,65500),(0,1,31,32,33,95),range(16),range(4),(0,1)):
  a=bytearray([0x95])*65536
  def put(off,v):struct.pack_into('<I',a,off,v&0xffffffff)
  put(12,base+0x1000 if existing else 0);put(0x20,base+0x400);put(0x24,base+0x410);put(0x28,base+0x410)
  # Existing grids deliberately bypass rebuilding; no manual link in that branch.
  for i in range(4):
   put(0x400+i*4,base+0x600+i*0x100 if mask&(1<<i) else 0);put(0x500+i*4,base+0x600+(3-i)*0x100 if mask&(1<<(3-i)) else 0)
   struct.pack_into('<4H',a,0x65c+i*0x100,width,33,origin+i*3,origin+i*2)
  manual=bool(mask&1) and mode==0 and not existing;id=(0,0x7fff,0x8000,0xffff,0x12345678,0xffffffff)[width%6]
  f.write(struct.pack('<3I',mode,manual,id));f.write(a);cases+=1;snapshots+=2+bool(mask and not existing)+3*manual
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as r,(w/'compiled.bin').open('rb') as c:
 while True:
  x=r.read(1<<20);y=c.read(1<<20);assert x==y,(w,'geometry/grid mismatch')
  if not x:break
assert (w/'compiled.bin').stat().st_size==snapshots*(65536+8)
headers=[ROOT/'rebuild/include'/p for p in ('engine/CEngineWorldMap.h','engine/CEngineMap.h')]
report=dict(cases=cases,snapshots=snapshots,omission_checks=3,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Original361byte builder,131byte cell linker and119byte unlinker with two audited fixups. Two build calls plus selected direct links;all16map-null masks,positions0/31/32/33/65500,widths0/1/31/32/33/95,existing-grid no-op,16bit ID truncation and repeated unlink with mismatched IDs (retail ignores the ID). Allocation callback records full64KiB arena and mutates list size/storage or dimensions before memset/linking. Original and compiled full arenas/events compare byte-for-byte;allocator alone is a boundary,all grid writes are production code.')
(ROOT/'rebuild/compile-gate/native-world-grid.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_WORLD_GRID_GATE_PASS',snapshots,'snapshots')
