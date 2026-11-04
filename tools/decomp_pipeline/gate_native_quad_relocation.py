"""Native quadtree relocation and real retail-empty owner rebinding."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-quad-relocation-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/bd/CLocalDetailCacheMap_QuadRelocation_00bde0c0.cpp','00/bd/CEngineLandscapeMemoryManager_SetVirtualBlockMovableResource_00bd9ab0.cpp','01/43/NEngineInternal_MapRenderers_01436ea8.cpp')];fixture=ROOT/'rebuild/tests/integration/NativeQuadRelocation_test.cpp'
move='?RelocateData@CQuadTreeElement@CLocalDetailCacheMap@@UAEXJPAX0J@Z';post='?OnPostRelocation@CQuadTreeElement@CLocalDetailCacheMap@@QAEXXZ';owner='?SetVirtualBlockMovableResource@CEngineLandscapeMemoryManager@@QAEXPAXPAVCMovableResource@@@Z';glob='?GPLandscapeRenderer@NEngineInternal@@3PAVCEngineLandscapeRenderer@@A';targets={post:0xbddcb0,owner:0xbd9ab0,glob:0x1436ea8}
aliases={'@Relocate@24':move,'@Post@4':post,'@SetOwner@16':owner,'_Renderer':glob}
(w/'quad_relocation_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();code=bytearray(291);fixups=[];md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
for a,n,offset in [(0xbde0c0,155,0),(0xbddcb0,85,192),(0xbd9ab0,3,288)]:
 body=read_virtual_data(raw,a,n);code[offset:offset+n]=body
 for i in md.disasm(body,a):
  if i.mnemonic=='call' and i.operands[0].type==2:
   target=i.operands[0].imm;assert target in targets.values();fixups.append((offset+i.address-a+1,target,1))
  elif i.mnemonic=='mov':
   for op in i.operands:
    if op.type==3 and op.mem.disp==0x1436ea8:fixups.append((offset+i.address-a+i.disp_offset,op.mem.disp,0))
assert len(fixups)==6
(w/'quad_relocation_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,code))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for i,p in enumerate(sources+[fixture]):
 o=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proofs=[check(objects[0],move,0xbde0c0,155,3,targets),check(objects[0],post,0xbddcb0,85,3,targets),check(objects[1],owner,0xbd9ab0,3,0,{})]
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(3):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
base=0x24000000;cases=0;expected=w/'expected.bin'
with (w/'input.bin').open('wb') as f,expected.open('wb') as out:
 for fill,mask,count,target,inplace,params in itertools.product((0,0x95),range(16),(0,1,3),range(9),(0,1),((0,0),(0xffffffff,0x80000000),(0x7fffffff,0xffffffff))):
  a=bytearray([fill])*8192
  def put(off,v):struct.pack_into('<I',a,off,v&0xffffffff)
  def ptr(off):return struct.unpack_from('<I',a,off)[0]
  put(4,base+0x600 if count else 0)
  for i in range(4):
   # Two child slots deliberately share the same object.
   put(8+4*i,base+0x200+(i//2)*0x100 if mask&(1<<i) else 0)
  for off in (0x200,0x300):
   put(off+4,base+0x600 if count else 0)
   for j in range(4):put(off+8+4*j,base+0xa00+j*16 if mask&(1<<j) else 0)
  for i in range(count):
   put(0x600+i*64+4,base+0x600+(i+1)*64 if i+1<count else 0);put(0x600+i*64+32,base+0x900)
  for i in range(3):put(0x900+i*32+12,base+0x900+(i+1)*32 if i<2 else 0)
  put(0x16b0,base+0x1c00)
  old=(ptr(8+4*target)-base if target<4 and ptr(8+4*target) else 0xb00) if target<4 else (0x600+(target-4)*64 if target<7 and target-4<count else 0xb00)
  new=old if inplace else 0xe00
  if old!=0xb00 and new!=old:a[new:new+0x4c]=a[old:old+0x4c]
  f.write(struct.pack('<5I',target==8,old,new,*params));f.write(a);cases+=1
  for step in range(3):
   if step<2 and target!=8:
    found=False
    for j in range(4):
     if ptr(8+4*j)==base+old:put(8+4*j,base+new);found=True;break
    if not found:
     linkoff=4;current=ptr(linkoff)
     while current:
      if current==base+old:put(linkoff,base+new);break
      linkoff=current-base+4;current=ptr(linkoff)
   out.write(struct.pack('<2I',step,base+0x1000));out.write(a)
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
for kind in ('retail','compiled'):
 with expected.open('rb') as e,(w/(kind+'.bin')).open('rb') as f:
  while True:
   x=e.read(1<<20);y=f.read(1<<20);assert x==y,(w,kind,'relocation/independent expected mismatch')
   if not x:break
headers=[ROOT/'rebuild/include'/p for p in ('engine/CLocalDetailCacheMap.h','engine/CEngineLandscapeRenderer.h','engine/CEngineLandscapeMemoryManager.h','fable_movable_resource.h','fable_engine_internal.h')]
report=dict(cases=cases,snapshots=cases*3,omission_checks=3,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Original155/85/3byte relocation/post-relocation/retail-empty owner setter with six audited fixups;all compose with real renderer global. Two relocations or post-relocations and direct empty setter per case. Full8KiB snapshots match both original execution and independently generated expected pointer changes. Child masks,duplicate child pointers,collection lists0/1/3,head/middle/tail/not-found,in-place/copied moves,ignored signed handle/size extremes;no behavior callbacks remain inside these bodies.')
(ROOT/'rebuild/compile-gate/native-quad-relocation.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_QUAD_RELOCATION_GATE_PASS',cases*3,'snapshots')
