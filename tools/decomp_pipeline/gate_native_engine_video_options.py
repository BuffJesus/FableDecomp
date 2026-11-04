"""Retail video-settings fallback, callback ordering and singleton reload parity."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-video-options-'));env=V.env()
names=('00/b2/CEngine_SetVideoOptions_00b24690.cpp','01/43/NEngineInternal_GPEngine_01436ea4.cpp','01/43/NEngineInternal_Globals_01436e18.cpp','01/43/NEngineInternal_GPWeatherRenderer_01436e44.cpp','01/43/NEngineInternal_EffectGlobals_01436e40.cpp','01/43/NEngineInternal_GPWaterRenderer_01436e54.cpp','01/43/NEngineInternal_GPResourceManager_01436e98.cpp','01/43/NEngineInternal_GPPrimitiveRenderer_01436e80.cpp','01/43/NEngineInternal_MapRenderers_01436ea8.cpp','01/43/NEngineInternal_GPPrimitiveOcclusionManager_01436e58.cpp')
sources=[ROOT/'rebuild/src/compiled'/p for p in names];fixture=ROOT/'rebuild/tests/integration/NativeEngineVideoOptions_test.cpp'
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
targets=dict(globals);targets.update({s:v[0] for s,v in calls.items()})
aliases={'@SetOptions@12':symbol};aliases.update({s:v[1] for s,v in calls.items()});aliases.update(zip(('_VideoGlobal','_DisplayGlobal','_WeatherGlobal','_ColourGlobal','_WaterGlobal','_EngineGlobal','_ResourceGlobal','_PrimitiveGlobal','_LocalGlobal','_OcclusionGlobal'),globals))
(w/'engine_video_options_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();body=read_virtual_data(raw,0xb24690,443);md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True;fixups=[]
for i in md.disasm(body,0xb24690):
 if i.mnemonic=='call' and i.operands[0].type==2:
  assert i.operands[0].imm in targets.values();fixups.append((i.address-0xb24690+i.imm_offset,i.operands[0].imm,1))
 else:
  for op in i.operands:
   if op.type==3 and op.mem.disp in globals.values():fixups.append((i.address-0xb24690+i.disp_offset,op.mem.disp,0))
assert len(fixups)==25
(w/'engine_video_options_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for n,p in enumerate(sources+[fixture]):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proof=check(objects[0],symbol,0xb24690,443,25,targets)
for (s,a),n in zip(globals.items(),(2,2,3,4,5,1,6,7,8,9)):assert coff_data(Coff(objects[n]),s,4)==read_virtual_data(raw,a,4)==bytes(4)
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(len(sources)):
 r=subprocess.run(cmd+[str(p) for i,p in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0;BASE=0x24000000
with (w/'input.bin').open('wb') as f:
 for initialized,status,restore,placement,geometry in itertools.product((0,255),range(27),range(3),(0x400,0xc0),range(5)):
  mode=(status+restore+geometry)%8;a=bytearray([0x95])*8192;a[0x84]=initialized;a[0xbd]=0x7f
  for engine,defs in ((0,0x800),(0x200,0xc00)):
   for j in range(4):struct.pack_into('<I',a,engine+0x120+4*j,BASE+defs+128*j);struct.pack_into('<i',a,defs+128*j+0x3c,1000+defs+7*j)
  mesh=(status+geometry)%4;texture=(restore+geometry)%4;effects=(status+restore)%4
  struct.pack_into('<4I',a,0xe0,mesh,texture,geometry%4,effects)
  x,y=((-10,-20),(640,480),(1024,768),(1920,600),(800,1440))[geometry]
  struct.pack_into('<2i11I',a,placement,x,y,0x3f800000,0x3f800000,32,60,255 if geometry%2 else 0,geometry%4,mesh,texture,geometry%4,effects,geometry)
  f.write(struct.pack('<7I',initialized,status,restore,mode,placement,status%2,geometry%2));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as f,(w/'compiled.bin').open('rb') as g:
 while True:
  a=f.read(1<<20);b=g.read(1<<20);assert a==b,w
  if not a:break
size=(w/'compiled.bin').stat().st_size;assert size%8260==0;snapshots=size//8260
report=dict(cases=cases,snapshots=snapshots,omission_checks=len(sources),providers=[proof],new_data_providers=1,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Full 8KiB arena and ten actual singleton words. All27 display-result combinations,three restore results,aliased/separate input,five signed geometries,raw initialization and VSync flags. Independently asserted callback arguments/order/counts and result. Callbacks mutate input,display/video/engine/water/local globals,quality indices and reset flag. Underlying display,quality,resource,draw-distance,occlusion and smart-restore methods remain explicit boundaries.')
(ROOT/'rebuild/compile-gate/native-engine-video-options.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_ENGINE_VIDEO_OPTIONS_GATE_PASS',snapshots,'snapshots')
