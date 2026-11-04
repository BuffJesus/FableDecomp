"""Native engine shutdown, GPU detach and ordered resource cleanup."""
import hashlib,json,subprocess,tempfile
from pathlib import Path
import check_one as V
from gate_game_component_abi import Coff
from variable_allocator_parity import check
from pe_oracle import EXE,pe_sections,va_to_off
import oracle_native_engine_uninitialise as oracle
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-engine-uninitialise-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/b2/CEngine_Uninitialise_00b25d80.cpp';fixture=ROOT/'rebuild/tests/integration/NativeEngineUninitialise_test.cpp'
symbol='?Uninitialise@CEngine@@UAEXXZ'
targets={'?GPDisplayManager@NEngineInternal@@3PAVCDisplayManager@@A':0x1436e1c,'?GPRenderManager@NEngineInternal@@3PAVCRenderManager@@A':0x1436e18,'?GPPrimitiveRendererSharedData@NEngineInternal@@3PAVCEnginePrimitiveRendererSharedData@@A':0x1436e78,'?GPVideoOptions@NEngineInternal@@3PBVCEngineVideoOptions@@B':0x1436eb0,'?RemoveDeviceResetCallback@CDisplayManager@@QAEXPAVCDeviceResetCallback@@@Z':0x9bf5a0,'?Uninitialise@CInitBaseClass@@IAEXXZ':0x99a360,'_memmove':0xbfeae6}
aliases={'@UninitialiseEngine@4':symbol,'?GPDisplayManager@NEngineInternal@@3PAVCDisplayManager@@A':'_DisplayManager','?GPRenderManager@NEngineInternal@@3PAVCRenderManager@@A':'_RenderManager','?GPPrimitiveRendererSharedData@NEngineInternal@@3PAVCEnginePrimitiveRendererSharedData@@A':'_PrimitiveShared','?GPVideoOptions@NEngineInternal@@3PBVCEngineVideoOptions@@B':'_VideoOptions','?RemoveDeviceResetCallback@CDisplayManager@@QAEXPAVCDeviceResetCallback@@@Z':'@RemoveCallbackBoundary@12','?Uninitialise@CInitBaseClass@@IAEXXZ':'@BaseUninitialiseBoundary@4'}
data_aliases={name:alias for name,alias in aliases.items()if alias in ('_DisplayManager','_RenderManager','_PrimitiveShared','_VideoOptions')}
for name,alias in data_aliases.items():del aliases[name];aliases[alias]=name
settings=ROOT/'rebuild/src/compiled/01/43/NEngineInternal_Globals_01436e18.cpp'
(w/'engine_uninitialise_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")'for a,b in aliases.items())+'\n')
objects=[]
for i,path in enumerate([source,fixture,settings]):
 obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proof=check(objects[0],symbol,0xb25d80,653,17,targets);c=Coff(objects[0]);raw=EXE.read_bytes();sections=pe_sections(raw)
from gate_render_state_static_data import coff_data
from pe_oracle import read_virtual_data
data=Coff(objects[2]);data_checks=[]
for name in data_aliases:
 assert coff_data(data,name,4)==read_virtual_data(raw,targets[name],4)==bytes(4)
 data_checks.append(dict(symbol=name,address='%08x'%targets[name],size=4))
assert not any(name.startswith('_$E')or name.startswith('_$S')for name in data.symbols)
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omit in (0,2):
 r=subprocess.run(cmd+[str(o)for i,o in enumerate(objects)if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs,expected,report=oracle.collect();(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
actual=(w/'actual.bin').read_bytes()
if actual!=expected:
 (w/'expected.bin').write_bytes(expected);difference=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)));raise AssertionError((w,difference,difference//(oracle.WIDTH+40),difference%(oracle.WIDTH+40),len(actual),len(expected)))
report.update(providers=[proof],omission_checks=2,data_symbols=data_checks,settings_sha256=hashlib.sha256(settings.read_bytes()).hexdigest(),source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Full36KiB arena includes the complete engine and two render managers, plus four internal globals. Init on/off, all16 texture masks,0/1/3/5 resource entries with nulls, changing renderer globals during callbacks, signed texture-stage high-water marks and list mutation. D3D failures are ignored as retail does; no masked or normalized state. Device-reset deregistration, GPU methods, virtual resource deletion and base Uninitialise are boundaries.')
(ROOT/'rebuild/compile-gate/native-engine-uninitialise.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip(),report['snapshots'],'snapshots')
