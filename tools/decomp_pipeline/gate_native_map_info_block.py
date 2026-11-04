"""Native world-map opening, ownership and callback traversal comparisons."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-map-info-block-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/b3/CEngineMap_ReadMapInfoBlock_00b3efa0.cpp';fixture=ROOT/'rebuild/tests/integration/NativeMapInfoBlock_test.cpp'
symbol='?ReadMapInfoBlock@CEngineMap@@CIXAAUCStaticMapInfoBlock@1@AAVCDataInputStream@@@Z'
targets={'?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z':0x993ca0}
aliases={'@ReadInfo@8':symbol,'?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z':'@Overflow@16'}
(w/'map_info_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
body=read_virtual_data(EXE.read_bytes(),0xb3efa0,1597);md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
fixups=[]
for i in md.disasm(body,0xb3efa0):
 if i.mnemonic=='call' and i.operands[0].type==2:
  assert i.operands[0].imm==0x993ca0;fixups.append(i.address-0xb3efa0+1)
assert len(fixups)==18
(w/'map_info_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[]={'+','.join(map(str,fixups))+'};\n')
objects=[]
for n,p in enumerate((source,fixture)):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proof=check(objects[0],symbol,0xb3efa0,1597,18,targets)
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0;base=0x24000000
with (w/'input.bin').open('wb') as f:
 for fill,mode,position,remaining,alignment in itertools.product((0,0x95),range(4),(0,3,0x7fffff00,0xfffffffc),(-1,0,3,4,7,16,63,64,67,87,88,91,92,128),range(4)):
  a=bytearray([fill])*2048
  def put(off,v):struct.pack_into('<I',a,off,v&0xffffffff)
  a[0x500:0x600]=bytes((i*71+17)&255 for i in range(256));put(0x304,position);put(0x308,0x7fffffff);put(0x30c,base+0x500+alignment);put(0x310,0);put(0x314,remaining)
  f.write(struct.pack('<I',mode));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as e,(w/'compiled.bin').open('rb') as f:
 while True:
  x=e.read(1<<20);y=f.read(1<<20);assert x==y,(w,'native mismatch')
  if not x:break
size=(w/'compiled.bin').stat().st_size;assert size%2056==0;snapshots=size//2056
report=dict(cases=cases,snapshots=snapshots,omission_checks=1,provider=proof,source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Original1597byte body/18audited overflow fixups. Full2KiB snapshots;unaligned metadata,full and split source windows,negative/zero/short remaining chunks,near-INT_MAX valid positions and UINT_MAX wrapping. Overflow callback refills different chunk sizes or mutates previously read header fields. Invalid first reads leaving scalar temporaries uninitialized are excluded. Only source-chunk overflow is a boundary.')
(ROOT/'rebuild/compile-gate/native-map-info-block.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_MAP_INFO_BLOCK_GATE_PASS',snapshots,'snapshots')
