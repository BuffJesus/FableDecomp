"""Signed surface-copy eligibility with actual pixel helpers and repeated descriptions."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-surface-signed-check-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/9f/CSurface_CanDoProperSignedConversion_009f35d0.cpp','00/41/CSurface_GetHeight_00415bf0.cpp','00/9e/CPixelFormat_IsSigned_009e3970.cpp','00/9e/CPixelFormat_GetByteColourDepth_009e3800.cpp','00/9e/CPixelFormat_Initialise_009e3830.cpp','01/29/CPixelFormat_Table_0129ba40.cpp')];fixture=ROOT/'rebuild/tests/integration/NativeSurfaceSignedCheck_test.cpp'
parent='?CanDoProperSignedConversion@CSurface@@AAE_NPBVC2DBoxI@@0ABVCPixelFormat@@@Z';table='?GD3DFormats@@3QBVCD3DFormatInfo@@B'
targets={'?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z':0x9e3830,'?IsSigned@CPixelFormat@@QBE_NXZ':0x9e3970,'?GetByteColourDepth@CPixelFormat@@QBEKXZ':0x9e3800,'?GetPixelFormat@CSurface@@QBE?AVCPixelFormat@@XZ':0x9f2f60,'?GetHeight@CSurface@@QBEKXZ':0x415bf0,'?GetWidth@CSurface@@QBEKXZ':0x415bd0,'?GetDimensions@CSurface@@QBE?AVC2DExtentsI@@XZ':0x9f2f90,table:0x129ba40}
(w/'surface_signed_check_aliases.h').write_text('#pragma comment(linker,"/alternatename:@Check@20='+parent+'")\n#pragma comment(linker,"/alternatename:_FormatTable='+table+'")\n')
raw=EXE.read_bytes();body=bytearray(576);fixups=[];md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
spans=[(0x9f35d0,271,0),(0x415bd0,18,288),(0x415bf0,19,320),(0x9f2f90,39,352),(0x9f2f60,43,400),(0x9e3970,17,448),(0x9e3800,22,480),(0x9e3830,50,512)]
for address,length,offset in spans:
 code=read_virtual_data(raw,address,length);body[offset:offset+length]=code
 for i in md.disasm(code,address):
  if i.mnemonic=='call' and i.operands[0].type==2:fixups.append((offset+i.address-address+i.imm_offset,i.operands[0].imm,1))
  else:
   for o in i.operands:
    if o.type==2 and 0x129ba40<=o.imm<0x129c0dc:fixups.append((offset+i.address-address+i.imm_offset,o.imm,0))
    if o.type==3 and 0x129ba40<=o.mem.disp<0x129c0dc:fixups.append((offset+i.address-address+i.disp_offset,o.mem.disp,0))
assert len(fixups)==18,len(fixups)
(w/'surface_signed_check_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for n,p in enumerate(sources+[fixture]):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
specs=[(0,parent,0x9f35d0,271,12),(1,'?GetHeight@CSurface@@QBEKXZ',0x415bf0,19,1),(0,'?GetDimensions@CSurface@@QBE?AVC2DExtentsI@@XZ',0x9f2f90,39,0),(0,'?GetPixelFormat@CSurface@@QBE?AVCPixelFormat@@XZ',0x9f2f60,43,1),(2,'?IsSigned@CPixelFormat@@QBE_NXZ',0x9e3970,17,1),(3,'?GetByteColourDepth@CPixelFormat@@QBEKXZ',0x9e3800,22,1),(4,'?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z',0x9e3830,50,1)]
proofs=[check(objects[n],s,a,l,c,targets) for n,s,a,l,c in specs]
assert coff_data(Coff(objects[5]),table,1692)==read_virtual_data(raw,0x129ba40,1692)
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(6):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0
pairs=[(0,0),(0,1),(1,0),(1,1)]+[(i,1) for i in range(2,6)]+[(1,i) for i in range(2,6)]
with (w/'input.bin').open('wb') as f:
 for base,dest,source,boxes,mode in itertools.product((0,128),range(46),range(46),pairs,range(4)):
  status=0x80004005 if (dest+source+sum(boxes)+mode)%2 else 0
  f.write(struct.pack('<7I',base,dest,source,*boxes,status,mode));cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=120);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as f,(w/'compiled.bin').open('rb') as g:
 while True:
  a=f.read(1<<20);b=g.read(1<<20);assert a==b,w
  if not a:break
size=(w/'compiled.bin').stat().st_size;assert size%1024==0;snapshots=size//1024
report=dict(cases=cases,snapshots=snapshots,omission_checks=6,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual eligibility predicate plus pixel selection/signed/byte queries,format table,dimension/format getters and height getter. Six mandatory sources/eight native spans/eighteen fixups. GetWidth uses canonical inline COMDAT with inlined dimensions;behavior checked in composition,not claimed as standalone matching provider.46x46formats,two placements,twelve box patterns,four mutation modes,rotating success/failure HRESULT with valid description. Independent eligibility/event model compares every COM receiver/full1KiB state and final result. Callback can replace surface,change source format/destination box,or change surface format/dimensions after first description. COM only boundary.')
(ROOT/'rebuild/compile-gate/native-surface-signed-check.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_SURFACE_SIGNED_CHECK_GATE_PASS',snapshots,'snapshots')
