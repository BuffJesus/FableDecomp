"""Retail video-settings fallback, callback ordering and singleton reload parity."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-video-settings-chain-'));env=V.env()
names=('00/b2/CEngine_SetVideoOptions_00b24690.cpp','01/43/NEngineInternal_GPEngine_01436ea4.cpp','01/43/NEngineInternal_Globals_01436e18.cpp','01/43/NEngineInternal_GPWeatherRenderer_01436e44.cpp','01/43/NEngineInternal_EffectGlobals_01436e40.cpp','01/43/NEngineInternal_GPWaterRenderer_01436e54.cpp','01/43/NEngineInternal_GPResourceManager_01436e98.cpp','01/43/NEngineInternal_GPPrimitiveRenderer_01436e80.cpp','01/43/NEngineInternal_MapRenderers_01436ea8.cpp','01/43/NEngineInternal_GPPrimitiveOcclusionManager_01436e58.cpp')
names+=('00/b2/CEngine_SmartRestoreComponentVideoMemory_00b24500.cpp','01/43/NEngineInternal_ScreenEffectGlobals_01436e30.cpp')
names+=('00/b5/EngineWeatherColourQuality_00b50c60.cpp',)
names+=('00/b3/CEnginePrimitiveRenderer_SetDrawDistanceQuality_00b32010.cpp',)
names+=('00/b9/CEngineScene_InvalidateBoundingBoxesAndDrawDistances_00b93500.cpp','00/b9/CEngineSceneGrid_InvalidateBoundingBoxAndDrawDistance_00b91ca0.cpp')
names+=('00/b3/CEngineResourceManager_ResizeHiresTextureMemory_00b3b4f0.cpp','01/43/NEngineInternal_GPSurfaceCompositionManager_01436eac.cpp','00/9f/CGraphicDataBank_SceneDelay_009fd0c0.cpp','00/9f/CResourceList_IncrementFrameCounter_009fc160.cpp','00/a6/CTextureManager_CalculateTextureSize_00a698f0.cpp','00/9e/CPixelFormat_GetColourDepth_009e3820.cpp','00/9e/CPixelFormat_Initialise_009e3830.cpp','01/29/CPixelFormat_Table_0129ba40.cpp','00/b2/CEngineSurfaceCompositionManager_ChangeSceneForTextureBank_00b2ad80.cpp','00/a6/CTextureManager_ResizePoolTextureCount_00a6abe0.cpp','00/a6/CTextureManager_GetPoolIndex_00a69730.cpp')
names+=('00/b5/CEngineLocalDetailGenerator_UpdateLoadingDistanceVideoOptions_00b59870.cpp',)
names+=('00/b2/CEnginePrimitiveOcclusionManager_StartNewScene_00b2f740.cpp',)
sources=[ROOT/'rebuild/src/compiled'/p for p in names];fixture=ROOT/'rebuild/tests/integration/NativeVideoSettingsChain_test.cpp'
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
surface='?GPSurfaceCompositionManager@NEngineInternal@@3PAVCEngineSurfaceCompositionManager@@A';globals[surface]=0x1436eac
table='?GD3DFormats@@3QBVCD3DFormatInfo@@B';depth='?GetColourDepth@CPixelFormat@@QBEKXZ';increment='?IncrementFrameCounter@CResourceList@@QAEXK@Z';index='?GetPoolIndex@CTextureManager@@AAEJK@Z';frame='?FrameEnd@CTextureManager@@QAEXXZ';pool='?ResizePool@CPreallocTexturePool@@QAE_NK@Z'
calls.update({'?ChangeScene@CGraphicDataBank@@QAEXXZ':(0x9fd0c0,'@UnusedBank@8'),'?SetReduceSizeFrameDelay@CGraphicDataBank@@QAEXK@Z':(0x9fd100,'@UnusedDelay@12'),increment:(0x9fc160,'@UnusedInc@12'),'?CalculateTextureSize@CTextureManager@@SIKKK_NABVCPixelFormat@@@Z':(0xa698f0,'@UnusedSize@16'),depth:(0x9e3820,'@UnusedDepth@8'),'?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z':(0x9e3830,'@UnusedInit@12'),'?ChangeSceneForTextureBank@CEngineSurfaceCompositionManager@@QAEXXZ':(0xb2ad80,'@UnusedComposition@8'),'?ResizePoolTextureCount@CTextureManager@@QAE_NKK@Z':(0xa6abe0,'@UnusedResize@16'),index:(0xa69730,'@UnusedIndex@12'),frame:(0xa698b0,'@ResourceFrame@8'),pool:(0xa6a670,'@ResizePrealloc@12')})
targets=dict(globals);targets[table]=0x129ba40;targets.update({s:v[0] for s,v in calls.items()})
aliases={'@SetOptions@12':symbol};aliases.update({s:v[1] for s,v in calls.items()});aliases.update(zip(('_VideoGlobal','_DisplayGlobal','_WeatherGlobal','_ColourGlobal','_WaterGlobal','_EngineGlobal','_ResourceGlobal','_PrimitiveGlobal','_LocalGlobal','_OcclusionGlobal','_ShadowGlobal','_DisplacementGlobal','_GlowGlobal','_RadialGlobal','_SurfaceGlobal'),globals))
aliases['_FormatTable']=table
(w/'video_settings_chain_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True;fixups=[]
spans=[(0xb24690,0,443),(0xb24500,448,399),(0xb50c60,848,22),(0xb5a0b0,880,32),(0xb32010,912,134),(0xb93500,1056,181),(0xb91ca0,1248,122),(0xb3b4f0,1376,136),(0x9fd0c0,1520,41),(0x9fd100,1568,19),(0x9fc160,1600,15),(0xa698f0,1616,136),(0x9e3820,1760,13),(0x9e3830,1776,50),(0xb2ad80,1840,11),(0xa6abe0,1856,62),(0xa69730,1920,99),(0xb59870,2032,321),(0xb2f740,2368,5)]
body=bytearray(2373)
for address,start,length in spans:
 body[start:start+length]=read_virtual_data(raw,address,length)
 for i in md.disasm(body[start:start+length],address):
  if i.mnemonic in ('call','jmp') and i.operands[0].type==2 and i.operands[0].imm in targets.values():fixups.append((start+i.address-address+i.imm_offset,i.operands[0].imm,1))
  else:
   for op in i.operands:
    if op.type==3 and op.mem.disp in set(globals.values())|{0x129ba40,0x129ba48}:fixups.append((start+i.address-address+i.disp_offset,op.mem.disp,0))
    if op.type==2 and op.imm==0x129ba48:fixups.append((start+i.address-address+i.imm_offset,op.imm,0))
assert len(fixups)==51,len(fixups)
(w/'video_settings_chain_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for n,p in enumerate(sources+[fixture]):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proof=check(objects[0],symbol,0xb24690,443,25,targets)
smartproof=check(objects[10],"?SmartRestoreComponentVideoMemory@CEngine@@AAE?AW4ESmartRestoreComponentVideoMemoryResult@1@XZ",0xb24500,399,5,targets)
qualityproofs=[check(objects[12],'?SetEffectQuality@'+owner+'@@QAEXABVCEngineVideoOptionsDef@@@Z',a,n,0,{}) for owner,a,n in [('CEngineWeatherRenderer',0xb50c60,22),('CEngineScreenEffectColourFilter',0xb5a0b0,32)]]
drawproof=check(objects[13],"?SetDrawDistanceQuality@CEnginePrimitiveRenderer@@QAEXW4EEngineVideoOptionQuality@@ABVCEngineVideoOptionsDef@@@Z",0xb32010,134,1,{child:0xb93500})
sceneproof=check(objects[14],child,0xb93500,181,1,{gridchild:0xb91ca0})
gridproof=check(objects[15],gridchild,0xb91ca0,122,1,{gridchild:0xb91ca0})
for (s,a),n in zip(globals.items(),(2,2,3,4,5,1,6,7,8,9,4,11,11,11,17)):assert coff_data(Coff(objects[n]),s,4)==read_virtual_data(raw,a,4)==bytes(4)
additional=[(16,'?ResizeHiresTextureMemory@CEngineResourceManager@@QAE_NJ@Z',0xb3b4f0,136,8),(18,'?ChangeScene@CGraphicDataBank@@QAEXXZ',0x9fd0c0,41,2),(18,'?SetReduceSizeFrameDelay@CGraphicDataBank@@QAEXK@Z',0x9fd100,19,0),(19,increment,0x9fc160,15,0),(20,'?CalculateTextureSize@CTextureManager@@SIKKK_NABVCPixelFormat@@@Z',0xa698f0,136,2),(21,depth,0x9e3820,13,1),(22,'?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z',0x9e3830,50,1),(24,'?ChangeSceneForTextureBank@CEngineSurfaceCompositionManager@@QAEXXZ',0xb2ad80,11,1),(25,'?ResizePoolTextureCount@CTextureManager@@QAE_NKK@Z',0xa6abe0,62,3),(26,index,0xa69730,99,0),(27,'?UpdateLoadingDistanceVideoOptions@CEngineLocalDetailGenerator@@QAEXABVCEngineVideoOptionsDef@@@Z',0xb59870,321,0),(28,'?StartNewScene@CEnginePrimitiveOcclusionManager@@QAEXXZ',0xb2f740,5,0)]
resourceproofs=[check(objects[n],sym,address,length,relocs,targets) for n,sym,address,length,relocs in additional]
assert coff_data(Coff(objects[23]),table,1692)==read_virtual_data(raw,0x129ba40,1692)
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(len(sources)):
 r=subprocess.run(cmd+[str(p) for i,p in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0;BASE=0x24000000
with (w/'input.bin').open('wb') as f:
 for initialized,status,restore,placement,geometry in itertools.product((0,255),range(27),range(3),(0x400,0xc0),range(5)):
  mode=(status+restore+geometry)%8;a=bytearray([0x95])*65536;a[0x84]=initialized;a[0xbd]=0x7f
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
  struct.pack_into('<2I',a,0x1e54,BASE+0x7000,BASE+0x7400)
  for bank in (0x7400,0x7800):struct.pack_into('<I',a,bank+0x280,BASE+0x8000)
  poolcount=(0,1,3,16)[geometry%4];struct.pack_into('<I',a,0x8008,poolcount)
  for j in range(poolcount):struct.pack_into('<I',a,0x800c+4*j,(j+1)*65536)
  a[0x4444]=(0,1,255)[restore]
  for j in range(4):a[0x800+128*j+0x71]=(0,1,127,255)[j];a[0x800+128*j+0x72]=(0,255,0,1)[j]
  f.write(struct.pack('<7I',initialized,status,restore,mode,placement,status%2,geometry%2));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as f,(w/'compiled.bin').open('rb') as g:
 while True:
  a=f.read(1<<20);b=g.read(1<<20);assert a==b,w
  if not a:break
size=(w/'compiled.bin').stat().st_size;assert size%65624==0;snapshots=size//65624
report=dict(cases=cases,snapshots=snapshots,omission_checks=len(sources),providers=[proof,smartproof,drawproof,sceneproof,gridproof]+qualityproofs+resourceproofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual video options,weather/colour/draw-distance,scene/grid invalidation,local-detail distance updates,hires resource resize,bank/composition notifications,delay/counter,pixel/size/table,pool wrapper/selection occlusion scene flag and smart restore. Full64KiB arena plus15globals,immutable table.19native spans/51fixups,29mandatory sources. Independent cell flags,bank counters,delay restoration,signed texture count,resource callback counts,occlusion flag,actual shadow retry iteration count and final result. Display,water settings,texture-manager FrameEnd,preallocation pool resize,and lower smart-restore component/effect allocations remain explicit boundaries.')
(ROOT/'rebuild/compile-gate/native-video-settings-chain.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_VIDEO_SETTINGS_CHAIN_GATE_PASS',snapshots,'snapshots')
