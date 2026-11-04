"""Actual texture-pool binary selection through its resize parent."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='texture-pool-selection-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/a6/CTextureManager_ResizePoolTextureCount_00a6abe0.cpp','00/a6/CTextureManager_GetPoolIndex_00a69730.cpp')];fixture=ROOT/'rebuild/tests/integration/NativeTexturePoolSelection_test.cpp'
resize='?ResizePoolTextureCount@CTextureManager@@QAE_NKK@Z';index='?GetPoolIndex@CTextureManager@@AAEJK@Z';frame='?FrameEnd@CTextureManager@@QAEXXZ';pool='?ResizePool@CPreallocTexturePool@@QAE_NK@Z'
aliases={'@ResizeManager@16':resize,'@GetIndex@12':index,frame:'@Frame@8',pool:'@Resize@12'}
(w/'texture_pool_selection_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();body=read_virtual_data(raw,0xa6abe0,62)+bytes(2)+read_virtual_data(raw,0xa69730,99);md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True;fixups=[]
for i in md.disasm(body[:62],0xa6abe0):
 if i.mnemonic=='call':assert i.operands[0].imm in (0xa69730,0xa698b0,0xa6a670);fixups.append((i.address-0xa6abe0+i.imm_offset,i.operands[0].imm))
assert len(fixups)==3
(w/'texture_pool_selection_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][2]={'+','.join('{%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for n,p in enumerate(sources+[fixture]):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proofs=[check(objects[0],resize,0xa6abe0,62,3,{index:0xa69730,frame:0xa698b0,pool:0xa6a670}),check(objects[1],index,0xa69730,99,0,{})]
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(2):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0
with (w/'input.bin').open('wb') as f:
 for count,pattern in itertools.product((0,1,2,3,8,16),range(4)):
  sizes=[(j*17+1,0x80000000+j*4096,(j//3)*100,0xffffffff-count+j+1)[pattern] for j in range(count)]
  queries=sorted({0,1,0x7fffffff,0x80000000,0xffffffff}|{value+d for value in sizes for d in (-1,0,1) if 0<=value+d<=0xffffffff})
  for query,mode,result,base in itertools.product(queries,range(5),range(2),(0,0x800)):
   a=bytearray([0x95])*8192;struct.pack_into('<I',a,base+8,count)
   for j,value in enumerate(sizes):struct.pack_into('<I',a,base+0xc+4*j,value)
   requested=(0,1,31,0x80000000,0xffffffff)[mode];f.write(struct.pack('<6I',base,query,count,requested,mode,result));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as f,(w/'compiled.bin').open('rb') as g:
 while True:
  a=f.read(1<<20);b=g.read(1<<20);assert a==b,w
  if not a:break
size=(w/'compiled.bin').stat().st_size;assert size%8204==0;snapshots=size//8204
report=dict(cases=cases,snapshots=snapshots,omission_checks=2,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual private binary-search selector and resize parent. Full8KiB arena after direct lookup,frame callback,pool callback,final result. Empty/1/2/3/8/16pools,strict/duplicate/high-bit/extreme sorted sizes,exact/gap/out-of-range queries,two object positions and resize outcomes. Independent selection membership/lower-bound/no-write assertions. Frame callback mutates PoolCount,size entries,failure array;cached index still selects84byte pool. FrameEnd and pool ResizePool internals remain explicit callbacks.')
(ROOT/'rebuild/compile-gate/native-texture-pool-selection.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_TEXTURE_POOL_SELECTION_GATE_PASS',snapshots,'snapshots')
