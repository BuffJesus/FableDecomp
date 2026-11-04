"""Compare scenery-type loading with the original implementation."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from gate_game_component_abi import Coff
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-detail-type-load-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/be/CLocalDetailObjectCollectionType_Load_00be27b0.cpp';fixture=ROOT/'rebuild/tests/integration/NativeDetailTypeLoad_test.cpp'
symbol='?Load@CLocalDetailObjectCollectionType@@QAEXAAVCDataInputStream@@_N@Z'
o=w/'source.obj';r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(o),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr;c=Coff(o)
targets={'?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z':0x993ca0,'??1CCharString@@QAE@XZ':0x99eae0,'?GPResourceManager@NEngineInternal@@3PAVCEngineResourceManager@@A':0x1436e98,'?FindIndexBySymbol@CBankFile@@QBEKABVCCharString@@@Z':0x9ccdc0,'?PeekStaticMesh@CMeshDataBank@@QBEPBVC3DMeshInfo@@K@Z':0xa24280,'?PeekRepeatedMesh@CMeshDataBank@@QBEPBVC3DMeshInfo@@K@Z':0xa24290}
boundary={0x993ca0:'@Overflow@16',0x99eae0:'@Destroy@4',0x9ccdc0:'@Find@12',0xa24280:'@Static@12',0xa24290:'@Repeated@12'}
aliases={'@Load@16':symbol};aliases.update({s:boundary[a] for s,a in targets.items() if a in boundary})
(w/'detail_type_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();body=read_virtual_data(raw,0xbe27b0,706);md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True;fixups=[]
for i in md.disasm(body,0xbe27b0):
 if i.mnemonic=='call' and i.operands[0].type==2:
  a=i.operands[0].imm;assert a in targets.values();fixups.append((i.address-0xbe27b0+1,a,0))
sec,offset=c.symbols[symbol]
for i in md.disasm(body,0xbe27b0):
 for op in i.operands:
  if op.type==3 and op.mem.disp==0x1436e98:
   off=i.address-0xbe27b0+i.disp_offset;assert struct.unpack_from('<I',body,off)[0]==0x1436e98;fixups.append((off,0x1436e98,1))
assert len(fixups)==24
(w/'detail_type_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
proof=check(o,symbol,0xbe27b0,706,24,targets)
f=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(f),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
r=subprocess.run(cmd+[str(f)],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+[str(f),str(o)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0;base=0x24000000
with (w/'input.bin').open('wb') as f:
 for fill,mode,retail,pattern,remaining,position,index_case,primitive in itertools.product((0,0x95),range(6),(0,1),(0,1,3,7),(0,3,12,64),(0,0xffffffff),range(4),(0,1,2)):
  a=bytearray([fill])*4096
  def put(off,v):struct.pack_into('<I',a,off,v&0xffffffff)
  put(0x204,position);put(0x208,128);put(0x20c,base+0x300);put(0x214,remaining)
  put(0x410,base+0x600);put(0x450,base+0x640)
  indices=(0,1,0x7fffffff,0xffffffff)
  for i in range(3):
   put(0x300+i*4,indices[(i+index_case)%4]);put(0x900+i*4,indices[(i+index_case)%4]);put(0x804+i*32,0 if index_case==0 else 5)
  block=0x30c if retail else 0x300
  for i in range(13):put(block+i*4,0x23450000+i)
  put(block+36,primitive)
  f.write(struct.pack('<3I',mode,pattern,retail));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as e,(w/'compiled.bin').open('rb') as f:
 while True:
  x=e.read(1<<20);y=f.read(1<<20);assert x==y,(w,'native mismatch')
  if not x:break
size=(w/'compiled.bin').stat().st_size;assert size%4108==0;snapshots=size//4108
report=dict(cases=cases,snapshots=snapshots,omission_checks=1,provider=proof,source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Original706byte body/24 audited fixups. Full4KiB snapshots, both file formats, zero/empty/nonempty names, zero/positive/high-bit indices, primitive types, full/split/wrapped stream reads, callbacks mutate subsequent indices and active resource manager. Stream strings/overflow, string destruction, symbol lookup and mesh getters remain boundaries. Invalid scalar reads leaving uninitialized temporaries are excluded.')
(ROOT/'rebuild/compile-gate/native-detail-type-load.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_DETAIL_TYPE_LOAD_GATE_PASS',snapshots,'snapshots')
