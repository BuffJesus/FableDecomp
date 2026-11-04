"""Shadow texture creation branch/failure policy, raw state and singleton reload parity."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-shadow-initialise-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/b4/CEngineShadowRenderer_InitialiseTextures_00b4f8e0.cpp','01/43/NEngineInternal_Globals_01436e18.cpp')];fixture=ROOT/'rebuild/tests/integration/NativeShadowInitialise_test.cpp'
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

aliases={'@Initialise@8':symbol,'_DisplayGlobal':'?GPDisplayManager@NEngineInternal@@3PAVCDisplayManager@@A','_SurfaceVtable':'??_7CSurface@@6B@'}
callback_names={0xb4b830:'@ShadowCleanup@8',0x9e3830:'@PixelInit@12',0x9be8f0:'@ValidTexture@12',0x9be610:'@BestOpaque@20',0x9be930:'@ValidTarget@16',0x9bee00:'@ShaderVersion@8',0x9fa280:'@TextureInit@36',0x9f32a0:'@DepthInit@24',0x9fa1c0:'@TextureAssign@12',0x9f9e00:'@GetLevel@16',0x9f2e70:'@SurfaceAssign@12',0x9f2e20:'@SurfaceCleanup@8',0x9f9f70:'@TextureCleanup@8',0xa10e30:'@TargetCleanup@8',0xa10e60:'@AttachTarget@12',0xa10e90:'@AttachDepth@12',0xb4f420:'@BuildLookup@8'}
aliases.update({s:callback_names[a] for s,a in targets.items() if a in callback_names})
(w/'shadow_initialise_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();body=read_virtual_data(raw,0xb4f8e0,811);fixups=[];md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
for i in md.disasm(body,0xb4f8e0):
 if i.mnemonic in ('call','jmp') and i.operands[0].type==2 and not 0xb4f8e0<=i.operands[0].imm<0xb4fc0b:assert i.operands[0].imm in targets.values();fixups.append((i.address-0xb4f8e0+i.imm_offset,i.operands[0].imm,1))
 else:
  for o in i.operands:
   if o.type==2 and o.imm==0x122f84c:fixups.append((i.address-0xb4f8e0+i.imm_offset,o.imm,0))
   elif o.type==3 and o.mem.disp==0x1436e1c:fixups.append((i.address-0xb4f8e0+i.disp_offset,o.mem.disp,0))
assert len(fixups)==41
(w/'shadow_initialise_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for n,p in enumerate(sources+[fixture]):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proofs=[check(objects[0],symbol,0xb4f8e0,811,41,targets)]
assert coff_data(Coff(objects[1]),'?GPDisplayManager@NEngineInternal@@3PAVCDisplayManager@@A',4)==read_virtual_data(raw,0x1436e1c,4)==bytes(4)
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(2):
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
report=dict(cases=cases,snapshots=snapshots,omission_checks=2,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual parent and display singleton;full8KiB plus callback args and global snapshots. Independent selected technique,result,allocation/fallback/build counts,stored size/allocated flag.128 combined capability/allocation policies,negative/zero/positive size,two placements,first/second/no fallback format,four callback mutation modes. Singleton replacement,cached size across first path,rendering-technique mutation during build. Temporary surface identity checked within run;raw stack pointers excluded. All child operations remain explicit callbacks;no GPU allocation claimed.')
(ROOT/'rebuild/compile-gate/native-shadow-initialise.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_SHADOW_INITIALISE_GATE_PASS',snapshots,'snapshots')
