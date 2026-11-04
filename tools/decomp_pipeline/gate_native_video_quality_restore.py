"""Retail video-settings fallback, callback ordering and singleton reload parity."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-video-quality-restore-'));env=V.env()
names=('00/b2/CEngine_SetVideoOptions_00b24690.cpp','01/43/NEngineInternal_GPEngine_01436ea4.cpp','01/43/NEngineInternal_Globals_01436e18.cpp','01/43/NEngineInternal_GPWeatherRenderer_01436e44.cpp','01/43/NEngineInternal_EffectGlobals_01436e40.cpp','01/43/NEngineInternal_GPWaterRenderer_01436e54.cpp','01/43/NEngineInternal_GPResourceManager_01436e98.cpp','01/43/NEngineInternal_GPPrimitiveRenderer_01436e80.cpp','01/43/NEngineInternal_MapRenderers_01436ea8.cpp','01/43/NEngineInternal_GPPrimitiveOcclusionManager_01436e58.cpp')
names+=('00/b2/CEngine_SmartRestoreComponentVideoMemory_00b24500.cpp','01/43/NEngineInternal_ScreenEffectGlobals_01436e30.cpp')
names+=('00/b5/EngineWeatherColourQuality_00b50c60.cpp',)
sources=[ROOT/'rebuild/src/compiled'/p for p in names];fixture=ROOT/'rebuild/tests/integration/NativeVideoQualityRestore_test.cpp'
symbol='?SetVideoOptions@CEngine@@UAE_NABVCEngineVideoOptions@@@Z'
globals={
'?GPVideoOptions@NEngineInternal@@3PBVCEngineVideoOptions@@B':0x1436eb0,
'?GPDisplayManager@NEngineInternal@@3PAVCDisplayManager@@A':0x1436e1c,
'?GPWeatherRenderer@NEngineInternal@@3PAVCEngineWeatherRenderer@@A':0x1436e44,
'?GPScreenEffectColourFilter@NEngineInternal@@3PAVCEngineScreenEffectColourFilter@@A':0x1436e40,
'?GPWaterRenderer@NEngineInternal@@3PAVCEngineWaterRenderer@@A':0x1436e54,
'?GPEngine@NEngineInternal@@3PAVCEngine@@A':0x1436ea4,
'?GPResourceManager@NEngineInternal@@3PAVCEngineResourceManager@@A':0x1436e98,
'?GPPrimitiveRenderer@NEngineInternal@@3PAVCEnginePrimitiveRenderer@@A':0x1436e80,
'?GPLocalDetailGenerator@NEngineInternal@@3PAVCEngineLocalDetailGenerator@@A':0x1436e88,
'?GPPrimitiveOcclusionManager@NEngineInternal@@3PAVCEnginePrimitiveOcclusionManager@@A':0x1436e58}
globals.update({'?GPShadowRenderer@NEngineInternal@@3PAVCEngineShadowRenderer@@A':0x1436e60,'?GPScreenEffectDisplacementRenderer@NEngineInternal@@3PAVCEngineScreenEffectDisplacementRenderer@@A':0x1436e38,'?GPScreenEffectGlowRenderer@NEngineInternal@@3PAVCEngineScreenEffectGlowRenderer@@A':0x1436e34,'?GPScreenEffectRadialBlurRenderer@NEngineInternal@@3PAVCEngineScreenEffectRadialBlurRenderer@@A':0x1436e30})
calls={
'?SetDisplayMode@CDisplayManager@@QAE?AW4ECreateDeviceSuccess@1@ABVC2DExtentsI@@KW4ESurfaceMultisampleType@@K@Z':(0x9bff40,'@DisplayMode@24'),
'?ChangePresentationInterval@CDisplayManager@@QAEX_NJ0@Z':(0x9bff10,'@Interval@20'),
'?SetEffectQuality@CEngineWeatherRenderer@@QAEXABVCEngineVideoOptionsDef@@@Z':(0xb50c60,'@WeatherQuality@12'),
'?SetEffectQuality@CEngineScreenEffectColourFilter@@QAEXABVCEngineVideoOptionsDef@@@Z':(0xb5a0b0,'@ColourQuality@12'),
'?ResizeHiresTextureMemory@CEngineResourceManager@@QAE_NJ@Z':(0xb3b4f0,'@ResizeMemory@12'),
'?SetDrawDistanceQuality@CEnginePrimitiveRenderer@@QAEXW4EEngineVideoOptionQuality@@ABVCEngineVideoOptionsDef@@@Z':(0xb32010,'@DrawDistance@16'),
'?UpdateLoadingDistanceVideoOptions@CEngineLocalDetailGenerator@@QAEXABVCEngineVideoOptionsDef@@@Z':(0xb59870,'@LocalDistance@12'),
'?StartNewScene@CEnginePrimitiveOcclusionManager@@QAEXXZ':(0xb2f740,'@Scene@8'),
'?SmartRestoreComponentVideoMemory@CEngine@@AAE?AW4ESmartRestoreComponentVideoMemoryResult@1@XZ':(0xb24500,'@Restore@8')}
shadow='?ChangeVideoOptionShadowQuality@CEngineShadowRenderer@@QAE_NJM@Z';calls[shadow]=(0xb4fc10,'@Shadow@16')
targets=dict(globals);targets.update({s:v[0] for s,v in calls.items()})
aliases={'@SetOptions@12':symbol};aliases.update({s:v[1] for s,v in calls.items()});aliases.update(zip(('_VideoGlobal','_DisplayGlobal','_WeatherGlobal','_ColourGlobal','_WaterGlobal','_EngineGlobal','_ResourceGlobal','_PrimitiveGlobal','_LocalGlobal','_OcclusionGlobal','_ShadowGlobal','_DisplacementGlobal','_GlowGlobal','_RadialGlobal'),globals))
(w/'video_quality_restore_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();body=bytearray(read_virtual_data(raw,0xb24690,443))+bytearray(5)+bytearray(read_virtual_data(raw,0xb24500,399))+bytearray(1)+bytearray(read_virtual_data(raw,0xb50c60,22))+bytearray(10)+bytearray(read_virtual_data(raw,0xb5a0b0,32));md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True;fixups=[]
for i in md.disasm(body[:443],0xb24690):
 if i.mnemonic=='call' and i.operands[0].type==2:
  assert i.operands[0].imm in targets.values();fixups.append((i.address-0xb24690+i.imm_offset,i.operands[0].imm,1))
 else:
  for op in i.operands:
   if op.type==3 and op.mem.disp in globals.values():fixups.append((i.address-0xb24690+i.disp_offset,op.mem.disp,0))
assert len(fixups)==25
for i in md.disasm(body[448:847],0xb24500):
 if i.mnemonic=='call' and i.operands[0].type==2:
  assert i.operands[0].imm==0xb4fc10;fixups.append((448+i.address-0xb24500+i.imm_offset,i.operands[0].imm,1))
 else:
  for op in i.operands:
   if op.type==3 and op.mem.disp in globals.values():fixups.append((448+i.address-0xb24500+i.disp_offset,op.mem.disp,0))
assert len(fixups)==30
(w/'video_quality_restore_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for n,p in enumerate(sources+[fixture]):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proof=check(objects[0],symbol,0xb24690,443,25,targets)
smartproof=check(objects[10],"?SmartRestoreComponentVideoMemory@CEngine@@AAE?AW4ESmartRestoreComponentVideoMemoryResult@1@XZ",0xb24500,399,5,targets)
qualityproofs=[check(objects[12],'?SetEffectQuality@'+owner+'@@QAEXABVCEngineVideoOptionsDef@@@Z',a,n,0,{}) for owner,a,n in [('CEngineWeatherRenderer',0xb50c60,22),('CEngineScreenEffectColourFilter',0xb5a0b0,32)]]
for (s,a),n in zip(globals.items(),(2,2,3,4,5,1,6,7,8,9,4,11,11,11)):assert coff_data(Coff(objects[n]),s,4)==read_virtual_data(raw,a,4)==bytes(4)
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(len(sources)):
 r=subprocess.run(cmd+[str(p) for i,p in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0;BASE=0x24000000
with (w/'input.bin').open('wb') as f:
 for initialized,status,restore,placement,geometry in itertools.product((0,255),range(27),range(3),(0x400,0xc0),range(5)):
  mode=(status+restore+geometry)%8;a=bytearray([0x95])*32768;a[0x84]=initialized;a[0xbd]=0x7f
  for engine,defs in ((0,0x800),(0x200,0xc00)):
   for j in range(4):struct.pack_into('<I',a,engine+0x120+4*j,BASE+defs+128*j);struct.pack_into('<i',a,defs+128*j+0x3c,1000+defs+7*j)
  for j in range(4):struct.pack_into('<2I',a,0x864+128*j,1000+j,0x3f800000+j)
  struct.pack_into('<3I',a,0x168,BASE+0x1b00,BASE+0x1b04,BASE+0x1b04);struct.pack_into('<I',a,0x1b00,BASE+0x1a00)
  mesh=(status+geometry)%4;texture=(restore+geometry)%4;effects=(status+restore)%4
  struct.pack_into('<4I',a,0xe0,mesh,texture,geometry%4,effects)
  x,y=((-10,-20),(640,480),(1024,768),(1920,600),(800,1440))[geometry]
  struct.pack_into('<2i11I',a,placement,x,y,0x3f800000,0x3f800000,32,60,255 if geometry%2 else 0,geometry%4,mesh,texture,geometry%4,effects,geometry)
  a[0x4444]=(0,1,255)[restore]
  for j in range(4):a[0x800+128*j+0x71]=(0,1,127,255)[j];a[0x800+128*j+0x72]=(0,255,0,1)[j]
  f.write(struct.pack('<7I',initialized,status,restore,mode,placement,status%2,geometry%2));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as f,(w/'compiled.bin').open('rb') as g:
 while True:
  a=f.read(1<<20);b=g.read(1<<20);assert a==b,w
  if not a:break
size=(w/'compiled.bin').stat().st_size;assert size%32852==0;snapshots=size//32852
report=dict(cases=cases,snapshots=snapshots,omission_checks=len(sources),providers=[proof,smartproof]+qualityproofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual SetVideoOptions calls actual weather/colour setters and actual SmartRestore. Full32KiB arena plus14globals; all display outcomes,success/reduced-quality/component-failure restore,aliased inputs and callback mutations. Four retail spans/30fixups;13 mandatory production sources. Weather/colour updates observed through subsequent full-state snapshots; their callback-only mutation modes are absent in this composition. Component release/restore,shadow/effect allocation and other display/video callbacks remain explicit boundaries.')
(ROOT/'rebuild/compile-gate/native-video-quality-restore.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_VIDEO_QUALITY_RESTORE_GATE_PASS',snapshots,'snapshots')
