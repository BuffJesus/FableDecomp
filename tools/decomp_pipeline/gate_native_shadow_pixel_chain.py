"""Shadow lookup pixel-generation loop with exact unlock and surface cleanup."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-shadow-pixel-chain-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/b4/CEngineShadowRenderer_BuildDepthPackingLookupTexture_00b4f420.cpp','00/9f/CSurface_Unlock_009f2dc0.cpp','00/9f/CSurface_Uninitialise_009f2e20.cpp','00/9e/CPixelFormat_InitialiseRGBA_009e3790.cpp','00/9f/CSurface_SetPixel_009f31c0.cpp','00/9e/CPixelFormat_GetByteColourDepth_009e3800.cpp','00/9e/CPixelFormat_Initialise_009e3830.cpp','01/29/CPixelFormat_Table_0129ba40.cpp')];fixture=ROOT/'rebuild/tests/integration/NativeShadowPixelChain_test.cpp'
symbol='?BuildDepthPackingLookupTexture@CEngineShadowRenderer@@AAEXXZ'
targets={'??_7CSurface@@6B@':0x122f84c,'?GetSurfaceLevel@CTexture@@QBE?AVCSurface@@J@Z':0x9f9e00,'?Initialise@CPixelFormat@@QAEXJJJJ_N@Z':0x9e3790,'?Initialise@CTexture@@QAE_NABVC2DExtentsI@@JABVCPixelFormat@@KW4ESurfacePool@@_N3@Z':0x9fa280,'?Lock@CSurface@@QAE?AVCSurfaceLockedRect@@K@Z':0x9f33e0,'?SetPixel@CSurface@@QAEXJJABVCRGBColour@@ABVCSurfaceLockedRect@@@Z':0x9f31c0,'?Uninitialise@CSurface@@QAEXXZ':0x9f2e20,'?Unlock@CSurface@@QBEXXZ':0x9f2dc0}

targets.update({'?GD3DFormats@@3QBVCD3DFormatInfo@@B':0x129ba40,'?GetByteColourDepth@CPixelFormat@@QBEKXZ':0x9e3800,'?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z':0x9e3830,'?ConvertRGBToPFDData@CPixelFormat@@QBEXABVCRGBColour@@PAE@Z':0x9e3f60})
callbacks={0x9fa280:'@TextureInit@36',0x9f9e00:'@GetLevel@16',0x9f33e0:'@Lock@16',0x9e3f60:'@Convert@16'}
aliases={'_FormatTable':'?GD3DFormats@@3QBVCD3DFormatInfo@@B','@Build@8':symbol,'_SurfaceVtable':'??_7CSurface@@6B@'};aliases.update({s:callbacks[a] for s,a in targets.items() if a in callbacks})
(w/'shadow_pixel_chain_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();body=bytearray(576);fixups=[];md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
for address,length,offset in ((0xb4f420,231,0),(0x9f2dc0,10,240),(0x9f2e20,32,256),(0x9e3790,87,288),(0x9f31c0,82,384),(0x9e3800,22,480),(0x9e3830,50,512)):
 code=read_virtual_data(raw,address,length);body[offset:offset+length]=code
 for i in md.disasm(code,address):
  if i.mnemonic=='call' and i.operands[0].type==2:assert i.operands[0].imm in targets.values();fixups.append((offset+i.address-address+i.imm_offset,i.operands[0].imm,1))
  else:
   for o in i.operands:
    if o.type==2 and (o.imm==0x122f84c or 0x129ba40<=o.imm<0x129c0dc):fixups.append((offset+i.address-address+i.imm_offset,o.imm,0))
    if o.type==3 and 0x129ba40<=o.mem.disp<0x129c0dc:fixups.append((offset+i.address-address+i.disp_offset,o.mem.disp,0))
assert len(fixups)==14
(w/'shadow_pixel_chain_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for n,p in enumerate(sources+[fixture]):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proofs=[check(objects[0],symbol,0xb4f420,231,8,targets),check(objects[1],'?Unlock@CSurface@@QBEXXZ',0x9f2dc0,10,0,{}),check(objects[2],'?Uninitialise@CSurface@@QAEXXZ',0x9f2e20,32,0,{})]
proofs += [check(objects[n],sym,a,l,c,targets) for n,sym,a,l,c in ((3,'?Initialise@CPixelFormat@@QAEXJJJJ_N@Z',0x9e3790,87,1),(4,'?SetPixel@CSurface@@QAEXJJABVCRGBColour@@ABVCSurfaceLockedRect@@@Z',0x9f31c0,82,3),(5,'?GetByteColourDepth@CPixelFormat@@QBEKXZ',0x9e3800,22,1),(6,'?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z',0x9e3830,50,1))]
assert coff_data(Coff(objects[7]),'?GD3DFormats@@3QBVCD3DFormatInfo@@B',1692)==read_virtual_data(raw,0x129ba40,1692)
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(8):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0
with (w/'input.bin').open('wb') as f:
 for p in itertools.product((0,0x800),(1024,1040),range(2),range(4)):f.write(struct.pack('<4I',*p));cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=120);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as f,(w/'compiled.bin').open('rb') as g:
 while True:
  a=f.read(1<<20);b=g.read(1<<20);assert a==b,w
  if not a:break
snapshots=cases*6;assert (w/'compiled.bin').stat().st_size==snapshots*0xc0000
report=dict(cases=cases,pixel_calls=cases*65536,snapshots=snapshots,omission_checks=8,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual lookup builder,RGBA initializer,SetPixel,byte depth,D3DFORMAT initializer,table,Unlock and surface cleanup. Eight mandatory sources/seven native spans/fourteen fixups. Every65536pixel conversion checks actual address and colour,format1;32cases=2097152pixel conversions plus COM GetDesc calls. Six full768KiB snapshots/case;two placements/pitches/texture results,four rect/buffer/colour mutation modes. Real RGBA selector supplies index1. COM descriptions always valid despite failedHRESULT. Conversion,texture/get-surface/lock and COM remain boundaries;no visual frame claimed.')
(ROOT/'rebuild/compile-gate/native-shadow-pixel-chain.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_SHADOW_PIXEL_CHAIN_GATE_PASS',snapshots,'snapshots')
