"""Native pool allocation toggle with reentrant disposal boundary."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-landscape-pool-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/bd/CEngineLandscapeMemoryManager_EnablePoolAllocation_00bda070.cpp';fixture=ROOT/'rebuild/tests/integration/NativeLandscapePool_test.cpp'
code=read_virtual_data(EXE.read_bytes(),0xbda070,26);assert code[13]==0xe8 and 0xbda070+18+struct.unpack_from('<i',code,14)[0]==0xbd9ce0
(w/'landscape_pool_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,code))+'};\n')
objects=[]
for i,p in enumerate((source,fixture)):
 o=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proof=check(objects[0],'?EnablePoolAllocation@CEngineLandscapeMemoryManager@@QAEX_N@Z',0xbda070,26,1,{'?FreeDisposalList@CEngineLandscapeMemoryManager@@QAEXXZ':0xbd9ce0});print(proof)
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=snapshots=0
with (w/'input.bin').open('wb') as f,(w/'expected.bin').open('wb') as out:
 for fill,mode,initial,first,second in itertools.product((0,0x95),range(3),(0,1,0x80,0xff),(0,1,0x80,0xff),(0,1,0x80,0xff)):
  a=bytearray([fill])*256;a[0x39]=initial;f.write(struct.pack('<3I',mode,first,second));f.write(a);cases+=1
  for step,arg in enumerate((first,second)):
   if a[0x39]!=arg:
    out.write(struct.pack('<I',1));out.write(a);snapshots+=1
    if mode==1:a[:]=bytes([0xac])*256
    if mode==2:a[0x39]^=0xff
    a[0x39]=arg
   out.write(struct.pack('<I',step+2));out.write(a);snapshots+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind)
 assert (w/(kind+'.bin')).read_bytes()==(w/'expected.bin').read_bytes(),(w,kind)
header=ROOT/'rebuild/include/engine/CEngineLandscapeMemoryManager.h'
report=dict(cases=cases,snapshots=snapshots,omission_checks=1,providers=[proof],source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in (source,header)},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Original26bytes/oneaudited fixup versus compiled toggle and independent expected state. Full256byte snapshots;two calls;initial/request bytes0/1/80/ff. Disposal callback may overwrite entire object or flip enable byte;final requested byte must be stored after callback. FreeDisposalList remains recording boundary.')
(ROOT/'rebuild/compile-gate/native-landscape-pool.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_LANDSCAPE_POOL_GATE_PASS',snapshots,'snapshots')
