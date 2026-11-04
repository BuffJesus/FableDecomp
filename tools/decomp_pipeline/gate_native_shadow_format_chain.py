"""Shadow texture creation branch/failure policy, raw state and singleton reload parity."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-shadow-format-chain-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/b4/CEngineShadowRenderer_InitialiseTextures_00b4f8e0.cpp','01/43/NEngineInternal_Globals_01436e18.cpp')];fixture=ROOT/'rebuild/tests/integration/NativeShadowFormatChain_test.cpp'
sources += [ROOT/'rebuild/src/compiled'/p for p in ('00/b4/CEngineShadowRenderer_TextureCleanup_00b4b830.cpp','00/a1/CRenderTarget_Uninitialise_00a10e30.cpp','00/a1/CRenderTarget_AttachSurfaces_00a10e60.cpp','00/9f/CSurface_Assignment_009f2e70.cpp','00/9f/CSurface_Uninitialise_009f2e20.cpp','00/9f/CTexture_Uninitialise_009f9f70.cpp','00/9f/CTexture_Assignment_009fa1c0.cpp','00/9f/CTexture_GetSurfaceLevel_009f9e00.cpp','00/9f/CSurface_CopyConstructor_009f2d60.cpp','00/9f/CSurface_InitialiseFromTextureMipmap_009f2f10.cpp')]
sources += [ROOT/'rebuild/src/compiled'/p for p in ('00/9e/CPixelFormat_Initialise_009e3830.cpp','01/29/CPixelFormat_Table_0129ba40.cpp','00/9b/CDisplayManager_GetPixelShaderVersion_009bee00.cpp')]
symbol='?InitialiseTextures@CEngineShadowRenderer@@AAE_NXZ'
targets={
'??4CSurface@@QAEAAV0@ABV0@@Z':0x9f2e70,'??4CTexture@@QAEABV0@ABV0@@Z':0x9fa1c0,'??_7CSurface@@6B@':0x122f84c,
'?AttachTarget@CRenderTarget@@QAEXABVCSurface@@@Z':0xa10e60,'?AttachZBuffer@CRenderTarget@@QAEXABVCSurface@@@Z':0xa10e90,
'?BuildDepthPackingLookupTexture@CEngineShadowRenderer@@AAEXXZ':0xb4f420,'?GPDisplayManager@NEngineInternal@@3PAVCDisplayManager@@A':0x1436e1c,
'?GetBestOpaqueTextureFormat@CDisplayManager@@QAE_NKAAVCPixelFormat@@_N@Z':0x9be610,'?GetPixelShaderVersion@CDisplayManager@@QBEKXZ':0x9bee00,
'?GetSurfaceLevel@CTexture@@QBE?AVCSurface@@J@Z':0x9f9e00,'?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z':0x9e3830,
'?Initialise@CTexture@@QAE_NABVC2DExtentsI@@JABVCPixelFormat@@KW4ESurfacePool@@_N3@Z':0x9fa280,
'?InitialiseAsDepthBuffer@CSurface@@QAE_NABVC2DExtentsI@@ABVCPixelFormat@@W4ESurfaceMultisampleType@@_N@Z':0x9f32a0,
'?IsValidRenderTargetFormat@CDisplayManager@@QAE_NABVCPixelFormat@@0@Z':0x9be930,'?IsValidTextureFormat@CDisplayManager@@QAE_NABVCPixelFormat@@@Z':0x9be8f0,
'?Uninitialise@CRenderTarget@@QAEXXZ':0xa10e30,'?Uninitialise@CSurface@@QAEXXZ':0x9f2e20,'?Uninitialise@CTexture@@QAEXXZ':0x9f9f70,'?UninitialiseTextures@CEngineShadowRenderer@@AAEXXZ':0xb4b830}

copy='??0CSurface@@QAE@ABV0@@Z';mipmap='?InitialiseFromTextureMipmap@CSurface@@QAEXPAUIDirect3DSurface9@@@Z'
targets[copy]=0x9f2d60;targets[mipmap]=0x9f2f10
table='?GD3DFormats@@3QBVCD3DFormatInfo@@B';targets[table]=0x129ba40
aliases={'@Initialise@8':symbol,'_DisplayGlobal':'?GPDisplayManager@NEngineInternal@@3PAVCDisplayManager@@A','_SurfaceVtable':'??_7CSurface@@6B@'}
callback_names={0xb4b830:'@ShadowCleanup@8',0x9e3830:'@PixelInit@12',0x9be8f0:'@ValidTexture@12',0x9be610:'@BestOpaque@20',0x9be930:'@ValidTarget@16',0x9bee00:'@ShaderVersion@8',0x9fa280:'@TextureInit@36',0x9f32a0:'@DepthInit@24',0x9fa1c0:'@TextureAssign@12',0x9f9e00:'@GetLevel@16',0x9f2e70:'@SurfaceAssign@12',0x9f2e20:'@SurfaceCleanup@8',0x9f9f70:'@TextureCleanup@8',0xa10e30:'@TargetCleanup@8',0xa10e60:'@AttachTarget@12',0xa10e90:'@AttachDepth@12',0xb4f420:'@BuildLookup@8'}
aliases['_FormatTable']=table
aliases.update({s:callback_names[a] for s,a in targets.items() if a in callback_names})
(w/'shadow_format_chain_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();body=bytearray(1645);fixups=[];md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
spans=[(0,0xb4f8e0,811,41,0),(2,0xb4b830,77,5,816),(3,0xa10e30,36,2,896),(4,0xa10e60,35,2,944),(4,0xa10e90,35,2,992),(5,0x9f2e70,76,0,1040),(6,0x9f2e20,32,0,1120),(7,0x9f9f70,34,0,1152),(8,0x9fa1c0,98,0,1200),(9,0x9f9e00,96,5,1312),(10,0x9f2d60,55,1,1408),(11,0x9f2f10,66,0,1472),(12,0x9e3830,50,1,1552),(14,0x9bee00,29,0,1616)]
for _,address,length,_,offset in spans:
 code=read_virtual_data(raw,address,length);body[offset:offset+length]=code
 for i in md.disasm(code,address):
  if i.mnemonic in ('call','jmp') and i.operands[0].type==2 and not address<=i.operands[0].imm<address+length:
   assert i.operands[0].imm in targets.values();fixups.append((offset+i.address-address+i.imm_offset,i.operands[0].imm,1))
  else:
   for o in i.operands:
    if o.type==2 and o.imm in (0x122f84c,0x129ba40,0x129ba48):fixups.append((offset+i.address-address+i.imm_offset,o.imm,0))
    elif o.type==3 and o.mem.disp in (0x1436e1c,0x129ba40,0x129ba48):fixups.append((offset+i.address-address+i.disp_offset,o.mem.disp,0))
assert len(fixups)==59,(len(fixups),fixups[-8:])
(w/'shadow_format_chain_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for n,p in enumerate(sources+[fixture]):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
reverse={a:s for s,a in targets.items()};reverse[0xb4f8e0]=symbol
proofs=[check(objects[n],reverse[a],a,l,c,targets) for n,a,l,c,_ in spans]
assert coff_data(Coff(objects[1]),'?GPDisplayManager@NEngineInternal@@3PAVCDisplayManager@@A',4)==read_virtual_data(raw,0x1436e1c,4)==bytes(4)
assert coff_data(Coff(objects[13]),table,1692)==read_virtual_data(raw,0x129ba40,1692)
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(len(sources)):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0
with (w/'input.bin').open('wb') as f:
 for base,size,policy,choice,mode in itertools.product((0,0x1000),(0xffffffff,0,256),range(128),(0,1,8),range(4)):
  a=bytearray([0x95])*8192;struct.pack_into('<I',a,base+0x338,size);a[base+0x2af]=cases%256
  f.write(struct.pack('<5I',base,size,policy,choice,mode));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=120);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as f,(w/'compiled.bin').open('rb') as g:
 while True:
  a=f.read(1<<20);b=g.read(1<<20);assert a==b,w
  if not a:break
size=(w/'compiled.bin').stat().st_size;assert size%8220==0;snapshots=size//8220
report=dict(cases=cases,snapshots=snapshots,omission_checks=len(sources),providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual parent,shadow/render-target cleanup,attachments,surface assignment/copy/mipmap/getter and texture assignment/cleanup;14native spans/59fixups,15mandatory sources;actual pixel-format initialization/table and shader-version query.full8KiB plus callback args and global snapshots. Independent selected technique,result,allocation/fallback/build counts,stored size/allocated flag.128 combined capability/allocation policies,negative/zero/positive size,two placements,first/second/no fallback format,four callback mutation modes. Singleton replacement,cached size across first path,rendering-technique mutation during build. Temporary surface identity checked within run;raw stack pointers excluded. Remaining boundaries:display format-capability queries,texture/depth allocation,lookup builder,and COM GetSurfaceLevel/AddRef/Release. No GPU allocation claimed.')
(ROOT/'rebuild/compile-gate/native-shadow-format-chain.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_SHADOW_FORMAT_CHAIN_GATE_PASS',snapshots,'snapshots')
