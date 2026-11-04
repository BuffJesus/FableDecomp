"""Retail video-settings fallback, callback ordering and singleton reload parity."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-video-scene-restore-'));env=V.env()
names=('00/b2/CEngine_SetVideoOptions_00b24690.cpp','01/43/NEngineInternal_GPEngine_01436ea4.cpp','01/43/NEngineInternal_Globals_01436e18.cpp','01/43/NEngineInternal_GPWeatherRenderer_01436e44.cpp','01/43/NEngineInternal_EffectGlobals_01436e40.cpp','01/43/NEngineInternal_GPWaterRenderer_01436e54.cpp','01/43/NEngineInternal_GPResourceManager_01436e98.cpp','01/43/NEngineInternal_GPPrimitiveRenderer_01436e80.cpp','01/43/NEngineInternal_MapRenderers_01436ea8.cpp','01/43/NEngineInternal_GPPrimitiveOcclusionManager_01436e58.cpp')
names+=('00/b2/CEngine_SmartRestoreComponentVideoMemory_00b24500.cpp','01/43/NEngineInternal_ScreenEffectGlobals_01436e30.cpp')
names+=('00/b5/EngineWeatherColourQuality_00b50c60.cpp',)
names+=('00/b3/CEnginePrimitiveRenderer_SetDrawDistanceQuality_00b32010.cpp',)
names+=('00/b9/CEngineScene_InvalidateBoundingBoxesAndDrawDistances_00b93500.cpp','00/b9/CEngineSceneGrid_InvalidateBoundingBoxAndDrawDistance_00b91ca0.cpp')
sources=[ROOT/'rebuild/src/compiled'/p for p in names];fixture=ROOT/'rebuild/tests/integration/NativeVideoSceneRestore_test.cpp'
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
child='?InvalidateBoundingBoxesAndDrawDistances@CEngineScene@@QAEXXZ';calls[child]=(0xb93500,'@Invalidate@8')
gridchild='?InvalidateBoundingBoxAndDrawDistance@CEngineSceneGrid@@QAEXKK_N@Z';calls[gridchild]=(0xb91ca0,'@UnusedGrid@20')
targets=dict(globals);targets.update({s:v[0] for s,v in calls.items()})
aliases={'@SetOptions@12':symbol};aliases.update({s:v[1] for s,v in calls.items()});aliases.update(zip(('_VideoGlobal','_DisplayGlobal','_WeatherGlobal','_ColourGlobal','_WaterGlobal','_EngineGlobal','_ResourceGlobal','_PrimitiveGlobal','_LocalGlobal','_OcclusionGlobal','_ShadowGlobal','_DisplacementGlobal','_GlowGlobal','_RadialGlobal'),globals))
(w/'video_scene_restore_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();body=bytearray(read_virtual_data(raw,0xb24690,443))+bytearray(5)+bytearray(read_virtual_data(raw,0xb24500,399))+bytearray(1)+bytearray(read_virtual_data(raw,0xb50c60,22))+bytearray(10)+bytearray(read_virtual_data(raw,0xb5a0b0,32))+bytearray(read_virtual_data(raw,0xb32010,134))+bytearray(10)+bytearray(read_virtual_data(raw,0xb93500,181))+bytearray(11)+bytearray(read_virtual_data(raw,0xb91ca0,122));md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True;fixups=[]
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
assert body[1010]==0xe8 and 0xb32010+103+struct.unpack_from("<i",body,1011)[0]==0xb93500
fixups.append((1011,0xb93500,1))
assert body[1102]==0xe8 and 0xb93500+51+struct.unpack_from('<i',body,1103)[0]==0xb91ca0
assert body[1338]==0xe8 and 0xb91ca0+95+struct.unpack_from('<i',body,1339)[0]==0xb91ca0
fixups.extend(((1103,0xb91ca0,1),(1339,0xb91ca0,1)))
(w/'video_scene_restore_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for n,p in enumerate(sources+[fixture]):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proof=check(objects[0],symbol,0xb24690,443,25,targets)
smartproof=check(objects[10],"?SmartRestoreComponentVideoMemory@CEngine@@AAE?AW4ESmartRestoreComponentVideoMemoryResult@1@XZ",0xb24500,399,5,targets)
qualityproofs=[check(objects[12],'?SetEffectQuality@'+owner+'@@QAEXABVCEngineVideoOptionsDef@@@Z',a,n,0,{}) for owner,a,n in [('CEngineWeatherRenderer',0xb50c60,22),('CEngineScreenEffectColourFilter',0xb5a0b0,32)]]
drawproof=check(objects[13],"?SetDrawDistanceQuality@CEnginePrimitiveRenderer@@QAEXW4EEngineVideoOptionQuality@@ABVCEngineVideoOptionsDef@@@Z",0xb32010,134,1,{child:0xb93500})
sceneproof=check(objects[14],child,0xb93500,181,1,{gridchild:0xb91ca0})
gridproof=check(objects[15],gridchild,0xb91ca0,122,1,{gridchild:0xb91ca0})
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
  struct.pack_into('<I',a,0x5290,BASE+0x5600);struct.pack_into('<3I',a,0x5338,BASE+0x5400,BASE+0x5400+4*(geometry%4),BASE+0x540c)
  for j in range(3):struct.pack_into('<I',a,0x5400+4*j,BASE+0x5500+64*j)
  for sc in range(3):
   scene=0x5500+64*sc;outerheader=0x6000+32*sc;outernode=0x6100+48*sc;innerheader=0x6200+32*sc;innernode=0x6300+48*sc;scene_map=0x6400+64*sc;levels=0x6500+8*sc;root=0x6600+32*sc;down=0x6700+32*sc;array=0x6800+32*sc;cell=0x6a00+400*sc
   struct.pack_into('<3I',a,scene+4,BASE+outerheader,1,0)
   for header,node in ((outerheader,outernode),(innerheader,innernode)):
    struct.pack_into('<4I',a,header,0,BASE+node,BASE+node,BASE+node);struct.pack_into('<5I',a,node,1,BASE+header,0,0,sc)
   struct.pack_into('<3I',a,outernode+0x14,BASE+innerheader,1,0);struct.pack_into('<2I',a,innernode+0x14,BASE+scene_map,0)
   struct.pack_into('<3I',a,scene_map+0x18,BASE+levels,BASE+levels+8,BASE+levels+8);struct.pack_into('<2I',a,levels,BASE+root,0)
   struct.pack_into('<5I',a,root+0xc,BASE+down if (status+sc)%2 else 0,1,BASE+array,BASE+array+4,BASE+array+4)
   struct.pack_into('<I',a,array,BASE+cell if (geometry+sc)%3 else 0)
   struct.pack_into('<5I',a,down+0xc,0,2,BASE+array+4,BASE+array+20,BASE+array+20)
   for k in range(4):struct.pack_into('<I',a,array+4+4*k,BASE+cell+72*(k+1) if (geometry+k+sc)%3 else 0)
   for k in range(5):a[cell+72*k+0x44]=(status*17+restore*7+sc*13+k*5)%256
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
report=dict(cases=cases,snapshots=snapshots,omission_checks=len(sources),providers=[proof,smartproof,drawproof,sceneproof,gridproof]+qualityproofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual video options,weather/colour/draw-distance setters,scene nested-map traversal,recursive grid invalidation and smart restore. Full32KiB arena plus14globals;seven native spans/33fixups,16mandatory production TUs. Independently expected root/child cell flags,nullable cells/lower levels and display fallback matrix. Display,water quality,hires resource resize,local distance,occlusion,component restore and shadow/effect allocations remain explicit boundaries.')
(ROOT/'rebuild/compile-gate/native-video-scene-restore.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_VIDEO_SCENE_RESTORE_GATE_PASS',snapshots,'snapshots')
