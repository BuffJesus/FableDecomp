"""Water header and its renderer caller against the original, separately and composed."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-water-header-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled/00/b6/CWaterStaticMapInfoBlock_Load_00b6d6e0.cpp',ROOT/'rebuild/src/compiled/00/b6/CEngineWaterRenderer_LoadNonPatchData_00b6daf0.cpp'];fixture=ROOT/'rebuild/tests/integration/NativeWaterHeader_test.cpp'
symbols=['?Load@CWaterStaticMapInfoBlock@@QAE_NAAVCMemoryDataInputStream@@J@Z','?LoadNonPatchData@CEngineWaterRenderer@@QAEXAAVCMemoryDataInputStream@@0@Z']
targets={'?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z':0x993ca0,symbols[0]:0xb6d6e0}
aliases={'@Header@16':symbols[0],'@NonPatch@16':symbols[1],'?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z':'@Overflow@16'}
(w/'water_header_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
body=bytearray(512);md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True;fixups=[]
for address,length,offset in [(0xb6d6e0,120,0),(0xb6daf0,130,256)]:
 code=read_virtual_data(EXE.read_bytes(),address,length);body[offset:offset+length]=code
 for i in md.disasm(code,address):
  if i.mnemonic=='call' and i.operands[0].type==2:
   assert i.operands[0].imm in targets.values();fixups.append((offset+i.address-address+1,i.operands[0].imm))
assert len(fixups)==4
(w/'water_header_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][2]={'+','.join('{%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for n,p in enumerate(sources+[fixture]):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proofs=[check(objects[i],symbols[i],a,n,c,targets) for i,(a,n,c) in enumerate([(0xb6d6e0,120,1),(0xb6daf0,130,3)])]
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omitted in range(2):
 r=subprocess.run(cmd+[str(p) for i,p in enumerate(objects) if i!=omitted],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0;base=0x24000000
with (w/'input.bin').open('wb') as f:
 for kind,fill,mode,size,remaining,position in itertools.product((0,1),(0,0x95),range(4),(-1,0,7,8,9,0x7fffffff),(0,3,4,7,8,16),(0,4,0x7fffffff,0xfffffffc)):
  if kind and position==0x7fffffff:continue
  a=bytearray([fill])*2048;struct.pack_into('<6I',a,0x204,position,64,base+0x500,0,remaining,base+0x500)
  struct.pack_into('<3I',a,0x500,size&0xffffffff,0x12345678,0xabcdef01);struct.pack_into('<2I',a,0x520,0x13579bdf,0x2468ace0)
  f.write(struct.pack('<3I',kind,mode,size&0xffffffff));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as e,(w/'compiled.bin').open('rb') as f:
 while True:
  x=e.read(1<<20);y=f.read(1<<20);assert x==y,(w,'native mismatch')
  if not x:break
size=(w/'compiled.bin').stat().st_size;assert size%2060==0;snapshots=size//2060
report=dict(cases=cases,snapshots=snapshots,omission_checks=2,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual120byte header and130byte renderer caller/four audited fixups. Direct header result and full2KiB state; composed caller uses real header Load. Sizes other than8 rejected; contiguous/overflow windows, wrapped/rejected positioning and callback mutation. Caller prefix read always allowed, excluding original uninitialized size behavior. Chunk overflow remains a bounded callback; local output pointers normalized in event arguments, no snapshot masking.')
(ROOT/'rebuild/compile-gate/native-water-header.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_WATER_HEADER_GATE_PASS',snapshots,'snapshots')
