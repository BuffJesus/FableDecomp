"""Canonical signed-format query, existing surface ownership and image creation."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-surface-ownership-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/9e/CPixelFormat_IsSigned_009e3970.cpp','00/9f/CSurface_InitialiseFromPreallocatedSurface_009f2ec0.cpp','00/9f/CSurface_InitialiseAsImage_009f3230.cpp','01/29/CPixelFormat_Table_0129ba40.cpp')];fixture=ROOT/'rebuild/tests/integration/NativeSurfaceOwnership_test.cpp'
signed='?IsSigned@CPixelFormat@@QBE_NXZ';attach='?InitialiseFromPreallocatedSurface@CSurface@@QAEXPAUIDirect3DSurface9@@_N@Z';image='?InitialiseAsImage@CSurface@@QAE_NABVC2DExtentsI@@ABVCPixelFormat@@_N@Z';table='?GD3DFormats@@3QBVCD3DFormatInfo@@B';system='?GFGetSystemManager@@YIPAVCSystemManager@@XZ'
targets={table:0x129ba40,system:0x9a4ec0};aliases={'@Signed@8':signed,'@Attach@16':attach,'@Image@20':image,'_FormatTable':table,system:'@GetSystem@0'}
(w/'surface_ownership_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();body=bytearray(224);fixups=[];md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
specs=[(signed,0x9e3970,17,1,0),(attach,0x9f2ec0,65,0,32),(image,0x9f3230,107,2,112)]
for sym,address,length,count,offset in specs:
 code=read_virtual_data(raw,address,length);body[offset:offset+length]=code
 for i in md.disasm(code,address):
  if i.mnemonic=='call' and i.operands[0].type==2:fixups.append((offset+i.address-address+i.imm_offset,i.operands[0].imm,1))
  else:
   for o in i.operands:
    if o.type==3 and 0x129ba40<=o.mem.disp<0x129c0dc:fixups.append((offset+i.address-address+i.disp_offset,o.mem.disp,0))
assert len(fixups)==3
(w/'surface_ownership_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for n,p in enumerate(sources+[fixture]):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proofs=[check(objects[n],s,a,l,c,targets) for n,(s,a,l,c,off) in enumerate(specs)]
assert coff_data(Coff(objects[3]),table,1692)==read_virtual_data(raw,0x129ba40,1692)
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(4):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=snapshots=0
with (w/'input.bin').open('wb') as f:
 def emit(p):
  global cases,snapshots
  f.write(struct.pack('<13I',*(v&0xffffffff for v in p)));cases+=1;snapshots+=1+(int(bool(p[2]))+int(bool(p[4])) if p[0]==1 else int(bool(p[2]))+2 if p[0]==2 else 0)
 for base,idx in itertools.product((0,128),range(47)):emit([0,base,0,0,0,idx,0,0,0,0,0,0x300,0x320])
 for base,old,new,add,mode in itertools.product((0,128),(0,0x24001800),(0,0x24001800,0x24001840),range(2),range(4)):
  if add and not new:continue
  emit([1,base,old,new,add,0,0,0,0,mode,0,0x300,0x320])
 geometry=((0,0),(1,1),(256,256),(513,31),(0xffffffff,0x80000000),(0x7fffffff,7));statuses=(0,1,0x7fffffff,0x80000000,0x80004005,0xffffffff)
 for base,idx,g,h,dynamic in itertools.product((0,128),range(46),range(6),range(6),range(2)):
  seed=idx+g+h+dynamic;old=0x24001800 if seed%2 else 0;mode=seed%4;alias=(idx+g+h)%3;dims=base+8 if alias==1 else 0x300;fmt=base+12 if alias==2 else 0x320
  emit([2,base,old,0,dynamic,idx,*geometry[g],statuses[h],mode,(idx+h)%2,dims,fmt])
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=120);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as f,(w/'compiled.bin').open('rb') as g:
 while True:
  a=f.read(1<<20);b=g.read(1<<20);assert a==b,w
  if not a:break
assert (w/'compiled.bin').stat().st_size==snapshots*8192
report=dict(cases=cases,snapshots=snapshots,omission_checks=4,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Three actual methods and immutable1692byte table. Full8KiB independently expected state at COM/root callbacks and return. IsSigned47rows/two placements. Attachment old/new/null/same pointers,AddRef flag,release/ref callbacks mutate receiver fields;null plus AddRef excluded as native null dereference. Image46formats,six dimensions/six signed HRESULTs/two dynamic flags/two placements;rotating aliases,old/null surface,root display/dimension/format mutations and output pointer on failedHRESULT. Cached dimensions before root,format afterward,scratch pool3,allocation1 even failure. COM and system root remain boundaries.')
(ROOT/'rebuild/compile-gate/native-surface-ownership.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_SURFACE_OWNERSHIP_GATE_PASS',snapshots,'snapshots')
