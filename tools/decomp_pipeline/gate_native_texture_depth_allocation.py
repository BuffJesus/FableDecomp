"""Texture/depth allocation wrappers, mip defaults and callback mutation parity."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-texture-depth-allocation-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/9f/CSurface_InitialiseAsDepthBuffer_009f32a0.cpp','00/9f/CTexture_Initialise_009fa280.cpp','01/29/CPixelFormat_Table_0129ba40.cpp')];fixture=ROOT/'rebuild/tests/integration/NativeTextureDepthAllocation_test.cpp'
depth='?InitialiseAsDepthBuffer@CSurface@@QAE_NABVC2DExtentsI@@ABVCPixelFormat@@W4ESurfaceMultisampleType@@_N@Z';texture='?Initialise@CTexture@@QAE_NABVC2DExtentsI@@JABVCPixelFormat@@KW4ESurfacePool@@_N3@Z';table='?GD3DFormats@@3QBVCD3DFormatInfo@@B';root='?GFGetSystemManager@@YIPAVCSystemManager@@XZ';calc='?CalcByteLength@CTexture@@AAEXXZ'
targets={table:0x129ba40,root:0x9a4ec0,calc:0x9f9ee0}
(w/'texture_depth_allocation_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in (('@TextureInit@36',texture),('@DepthInit@24',depth),('_FormatTable',table),(root,'@GetSystem@0'),(calc,'@CalcLength@8')))+'\n')
raw=EXE.read_bytes();body=bytearray(293);fixups=[];md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
for address,length,offset in ((0x9f32a0,107,0),(0x9fa280,181,112)):
 code=read_virtual_data(raw,address,length);body[offset:offset+length]=code
 for i in md.disasm(code,address):
  if i.mnemonic=='call' and i.operands[0].type==2:assert i.operands[0].imm in targets.values();fixups.append((offset+i.address-address+i.imm_offset,i.operands[0].imm,1))
  else:
   for o in i.operands:
    if o.type==3 and o.mem.disp==0x129ba40:fixups.append((offset+i.address-address+i.disp_offset,o.mem.disp,0))
assert len(fixups)==5
(w/'texture_depth_allocation_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for n,p in enumerate(sources+[fixture]):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proofs=[check(objects[0],depth,0x9f32a0,107,2,targets),check(objects[1],texture,0x9fa280,181,3,targets)]
assert coff_data(Coff(objects[2]),table,1692)==read_virtual_data(raw,0x129ba40,1692)
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(3):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0;dims=((0,0),(1,7),(7,8),(8,8),(15,16),(512,256),(1023,1024),(0xffffffff,1),(0xffffffff,0xffffffff),(0x80000000,0x7fffffff),(32768,32768),(3,8192))
with (w/'input.bin').open('wb') as f:
 for entry,base,index,geometry,si in itertools.product(range(2),(0,0x400),range(46),range(len(dims)),range(6)):
  status=(0,1,0x7fffffff,0x80000000,0x80004005,0xffffffff)[si];mode=(index+geometry+si)%4;alias=(index+geometry)%3;old=(index+geometry+si)%2;dim=base+8 if alias==1 else 0x800;fmt=base+(8 if entry else 4) if alias==2 else 0x810
  a=bytearray([0x95])*8192;struct.pack_into('<I',a,base+(4 if entry else 0),0x24001840 if old else 0);struct.pack_into('<2I',a,dim,*dims[geometry]);struct.pack_into('<I',a,fmt,index)
  struct.pack_into('<I',a,0x1060,0x24001200);struct.pack_into('<I',a,0x1258,0x24001600);struct.pack_into('<I',a,0x1458,0x24001640)
  levels=(0xffffffff,0,1,3,0x80000000,0x7fffffff)[(index+geometry+si)%6];flags=(0,1,2,0x80000000,0xffffffff)[(index+si)%5];pool=(0,1,2,0xffffffff)[(index+geometry)%4];dynamic=(0,1,255)[(index+si)%3];automatic=(0,1,255)[(geometry+si)%3]
  f.write(struct.pack('<12I',entry,base,dim,fmt,levels,flags,pool,dynamic,automatic,status,mode,(index+si)%2));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=120);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as f,(w/'compiled.bin').open('rb') as g:
 while True:
  a=f.read(1<<20);b=g.read(1<<20);assert a==b,w
  if not a:break
size=(w/'compiled.bin').stat().st_size;assert size%8192==0;snapshots=size//8192
report=dict(cases=cases,snapshots=snapshots,omission_checks=3,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Full8KiB independent event/final model plus immutable1692byte table.46formats,12geometry pairs,sixHRESULTs,both wrappers/two placements;rotating mip counts,old/null resources,raw flags/pool/bools,input aliases and callback mutations. Independently derives mip count from bit length before root callback;validates exact COM slot23/29 args/output pointer. Depth discard ignored/alwaysfalse;ownership1 even failure. Texture success ownership nibble1 then length callback;failure preserves callback output/state. System root,COM allocation/release,and CalcByteLength remain explicit boundaries.')
(ROOT/'rebuild/compile-gate/native-texture-depth-allocation.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_TEXTURE_DEPTH_ALLOCATION_GATE_PASS',snapshots,'snapshots')
