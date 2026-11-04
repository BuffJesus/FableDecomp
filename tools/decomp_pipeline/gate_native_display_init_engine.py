"""Native display engine creation/options/activation and complete path cleanup."""
import hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import check_one as V
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from variable_allocator_parity import check
from pe_oracle import EXE,pe_sections,va_to_off,read_virtual_data
import oracle_native_display_init_engine as oracle
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-display-init-engine-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/43/CDisplayEngine_InitEngine_00435190.cpp';fixture=ROOT/'rebuild/tests/integration/NativeDisplayInitEngine_test.cpp'
symbol='?InitEngine@CDisplayEngine@@MAE_NXZ'
specs=[
('??0CEngineInit@@QAE@XZ',0x42d830,'@ConstructInit@4'),
('?GFGetSystemManager@@YIPAVCSystemManager@@XZ',0x9a4ec0,'@GetSystem@0'),
('?GFAllocEngine@@YIPAVCIEngine@@XZ',0xb26340,'@AllocateEngine@0'),
('?g_FableRenderManager2D_013B8384@@3PAVCRenderManager2D@@A',0x13b8384,'_Renderer'),
('?GetGraphicBankDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ',0x41a100,'@Graphics@4'),
('?GetMeshBankDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ',0x41a1c0,'@Mesh@4'),
('?GetShaderBankDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ',0x41a1e0,'@Shader@4'),
('?GetEngineCacheDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ',0x41a200,'@Cache@4'),
('?GetWriteableEngineCacheDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ',0x41a220,'@WriteCache@4'),
('??4CWideString@@QAEAAV0@ABV0@@Z',0x99b7d0,'@AssignWide@12'),
('??1CWideString@@QAE@XZ',0x99b510,'@DestroyWide@4'),
('?GStaticMapQuality@@3JA',0x1375490,'_StaticMap'),
('?g_FableEngineMemoryInit_0137549C@@3VCEngineMemoryInit@@A',0x137549c,'_Memory'),
('?GEnableMinimumStaticMapRebuild@@3_NA',0x13b8649,'_Minimum'),
('?PMainGameComponent@CMainGameComponent@@1PAV1@A',0x13b86a0,'_MainComponent'),
('?GAllowDataGeneration@@3_NA',0x1375459,'_Allow'),
('?GConfigOptions@@3UCConfigOptions@@A',0x1375070,'_Config'),
('?GBuildRetailStaticMaps@@3_NA',0x13b864a,'_BuildRetail'),
('??0CCharString@@QAE@PBDJ@Z',0x99ebf0,'@ConstructChar@16'),
('??1CCharString@@QAE@XZ',0x99eae0,'@DestroyChar@4'),
('?DisplayProgress@NProgressDisplay@@YIXABVCCharString@@M_N1@Z',0x9e9f40,'@Progress@16'),
('?Get@?$CTBaseSingleton@VCEngineManager@@@@SIAAVCEngineManager@@XZ',0x436fb0,'@GetManager@0'),
('?AddEngine@CEngineManager@@QAEXAAVCIEngine@@@Z',0x6404d0,'@AddEngine@12'),
('??$GetDef@VCEngineDef@@@CDefinitionManager@@QBE_NABVCCharString@@JPAV?$CDefPointer@$$CBVCEngineDef@@@@@Z',0x430096,'@GetEngineDef@20'),
('?GamePrimitiveDefaultFadeRangeRatio@CDisplayEngine@@2MA',0x13b8774,'_FadeRange')]
literals=[('??_C@_0BI@IEEPJFBJ@Init?5Engine?3?5Initialise?$AA@',0x1231600,b'Init Engine: Initialise\0'),('??_C@_0BH@GDKILFOB@Init?5Engine?3?5AddEngine?$AA@',0x12315e8,b'Init Engine: AddEngine\0'),('??_C@_0BI@CDFMIJPJ@Init?5Engine?3?5OnActivate?$AA@',0x12315d0,b'Init Engine: OnActivate\0'),('??_C@_06LKDHJJCH@ENGINE?$AA@',0x1230c84,b'ENGINE\0')]
aliases={name:alias for name,_,alias in specs if alias not in ['_StaticMap','_Memory','_Minimum','_Allow','_BuildRetail']};aliases['@InitDisplayEngine@4']=symbol
(w/'display_init_engine_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")'for a,b in aliases.items())+'\n')
objects=[]
settings=ROOT/'rebuild/src/compiled/01/37/EngineInit_Defaults_01375490.cpp'
for i,path in enumerate([source,fixture,settings]):
 obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
targets={name:address for name,address,_ in specs+literals};proof=check(objects[0],symbol,0x435190,774,58,targets);c=Coff(objects[0]);raw=EXE.read_bytes();sections=pe_sections(raw)
for name,address,value in literals:
 sec,off=c.symbols[name];assert c.sections[sec][0][off:off+len(value)]==value;o=va_to_off(sections,address);assert raw[o:o+len(value)]==value
data=Coff(objects[2]);data_checks=[]
for name,address,alias in specs:
 if alias not in ['_StaticMap','_Memory','_Minimum','_Allow','_BuildRetail']:continue
 value={'_StaticMap':struct.pack('<I',256),'_Memory':struct.pack('<10I',*[0xbf800000 if i in (1,5) else 0xffffffff for i in range(10)]),'_Minimum':b'\0','_Allow':b'\1','_BuildRetail':b'\0'}[alias]
 assert coff_data(data,name,len(value))==read_virtual_data(raw,address,len(value))==value;data_checks.append(dict(address='%08x'%address,symbol=name,size=len(value)))
assert not any(name.startswith('_$E') or name.startswith('_$S')for name in data.symbols)
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omit in (0,2):
 r=subprocess.run(cmd+[str(o)for i,o in enumerate(objects)if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs,expected,report=oracle.collect();(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=120);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
actual=(w/'actual.bin').read_bytes();stride=4348
if actual!=expected:
 (w/'expected.bin').write_bytes(expected);difference=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)));raise AssertionError((w,difference,difference//stride,difference%stride,len(actual),len(expected)))
report.update(providers=[proof],omission_checks=2,data_symbols=data_checks,settings_sha256=hashlib.sha256(settings.read_bytes()).hexdigest(),source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Full4KiB object arena,100-byte engine init, seven logical globals, full config/memory settings and debug-font slot. All128 option combinations, allocation failure, initialise returns0/1/2, world null/live, editor states,16 callback mutation profiles and reference-count extremes. Engine-init construction, resource paths/strings, system/engine allocation, progress, manager registration, definition lookup and virtual methods are recorded boundaries. No masked state bytes; callback arguments use explicit local-object labels.')
(ROOT/'rebuild/compile-gate/native-display-init-engine.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip(),report['snapshots'],'snapshots')
