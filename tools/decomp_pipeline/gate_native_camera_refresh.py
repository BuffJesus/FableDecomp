"""Retail camera transform submission with live global reloads and callbacks."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-camera-refresh-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/b2/CEngineCamera_RefreshD3DTransforms_00b2fc50.cpp'
settings=[ROOT/'rebuild/src/compiled'/p for p in ('01/43/NEngineInternal_Globals_01436e18.cpp','01/43/NEngineInternal_GPShaderRenderManager_01436e14.cpp')]
fixture=ROOT/'rebuild/tests/integration/NativeCameraRefresh_test.cpp'
targets={'?GFABoxToVBox@@YIXABVC2DBoxF@@AAV1@@Z':0x9e2020,'?GPRenderManager@NEngineInternal@@3PAVCRenderManager@@A':0x1436e18,'?GPShaderRenderManager@NEngineInternal@@3PAVCShaderRenderManager@@A':0x1436e14,'?SetVWindow@CRenderManagerCore@@QAEXABVC2DBoxF@@@Z':0xa0b560,'?SetViewTransform@CRenderManagerCore@@QAEXABVCMatrix3x4@@@Z':0xa0b0a0,'?SetWorldTransform@CRenderManagerCore@@QAEXABVCMatrix3x4@@@Z':0xa0b140,'?SetPerspectiveTransform@CRenderManagerCore@@QAEXMMMM@Z':0xa0b320,'?SetViewTransform@CShaderRenderManager@@QAEXABVCMatrix3x4@@@Z':0x988350,'?SetProjectionTransform@CShaderRenderManager@@QAEXABVCMatrix4x4@@@Z':0x988540,'?SetCameraPos@CShaderRenderManager@@QAEXABVC3DVector@@@Z':0x988320,'?SetWorldTransform@CShaderRenderManager@@QAEXABVCMatrix3x4@@@Z':0x9881f0}
boundaries={0x9e2020:'@BoxBoundary@8',0xa0b560:'@WindowBoundary@12',0xa0b0a0:'@ViewBoundary@12',0xa0b140:'@WorldBoundary@12',0xa0b320:'@PerspectiveBoundary@24',0x988350:'@ShaderViewBoundary@12',0x988540:'@ProjectionBoundary@12',0x988320:'@PositionBoundary@12',0x9881f0:'@ShaderWorldBoundary@12'}
aliases={symbol:boundaries[address]for symbol,address in targets.items()if address in boundaries}
aliases.update({'@RefreshCamera@4':'?RefreshD3DTransforms@CEngineCamera@@QAEXXZ','_RenderGlobal':'?GPRenderManager@NEngineInternal@@3PAVCRenderManager@@A','_ShaderGlobal':'?GPShaderRenderManager@NEngineInternal@@3PAVCShaderRenderManager@@A'})
(w/'camera_refresh_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")'for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();entry=0xb2fc50;code=read_virtual_data(raw,entry,259);m=Cs(CS_ARCH_X86,CS_MODE_32);m.detail=True;fixups=[]
for ins in m.disasm(code,entry):
    if ins.mnemonic=='call':fixups.append((ins.address-entry+1,ins.operands[0].imm,1))
    for op in ins.operands:
        if op.type==3 and op.mem.disp>=0x1220000:fixups.append((ins.address-entry+ins.disp_offset,op.mem.disp,0))
assert len(fixups)==17 and all(target in targets.values()for _,target,_ in fixups)
(w/'camera_refresh_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(str(b)for b in code)+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for i,path in enumerate([source]+settings+[fixture]):
    obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proof=check(objects[0],'?RefreshD3DTransforms@CEngineCamera@@QAEXXZ',entry,259,17,targets)
for i,name in ((1,aliases['_RenderGlobal']),(2,aliases['_ShaderGlobal'])):assert coff_data(Coff(objects[i]),name,4)==read_virtual_data(raw,targets[name],4)==bytes(4)
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omitted in range(3):
    r=subprocess.run(cmd+[str(p)for i,p in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs=bytearray();cases=0;values=(0,0x80000000,1,0x3f800000,0xbf800000,0x7f7fffff,0x7f800000,0xff800000,0x7fc12345,0x7f812345)
for fill,mode in itertools.product(range(256),range(8)):
    arena=bytearray([fill])*4096
    for index in range(4):struct.pack_into('<I',arena,0x6c+4*index,values[(fill+index)%len(values)])
    inputs.extend(struct.pack('<I',mode));inputs.extend(arena);cases+=1
(w/'input.bin').write_bytes(inputs)
for kind in ('retail','compiled'):
    r=subprocess.run([str(exe),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
actual=(w/'compiled.bin').read_bytes();expected=(w/'retail.bin').read_bytes()
if actual!=expected:
    diff=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)));raise AssertionError((w,diff,len(actual),len(expected)))
report=dict(cases=cases,snapshots=cases*10,omission_checks=3,providers=[proof],source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest()for p in [source]+settings},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual retail body natively executed after17 audited relocations; full4KiB camera/boundary arena, two actual globals and every submitted box/matrix/position/scalar argument. All256fills, exceptional float bit patterns, callback changes to both globals and subsequent camera fields, and mutation of shared local world matrix between renderer/shader calls. Direct replay records all nine calls; composed replay executes actual shader world/view/projection/position and transpose, leaving five renderer/coordinate calls as boundaries. Local argument values are compared without stack addresses; no object bytes masked or normalized.')
# Repeat the parent comparison with all four actual shader setters and transpose.
extra_sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/98/CShaderRenderManager_MatrixSetters_009881f0.cpp','00/a5/CMatrix4x4_Transpose_00a5cab0.cpp','00/98/CShaderRenderManager_SetCameraPos_00988320.cpp')]
shader_aliases={0x988350:'@ShaderView@12',0x988540:'@ShaderProjection@12',0x988320:'@ShaderPosition@12',0x9881f0:'@ShaderWorld@12'}
for symbol,address in targets.items():
    if address in shader_aliases:del aliases[symbol];aliases[shader_aliases[address]]=symbol
(w/'camera_refresh_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")'for a,b in aliases.items())+'\n')
composed_objects=objects[:3]
for i,path in enumerate(extra_sources+[fixture]):
    obj=w/('composed%d.obj'%i);r=V.cl(['/nologo','/c','/O2','/MT','/DCOMPOSED_SHADER','/I'+str(w),'/Fo'+str(obj),str(path)],env,w);assert not r.returncode,r.stdout+r.stderr;composed_objects.append(obj)
for omitted in range(6):
    r=subprocess.run(cmd+[str(p)for i,p in enumerate(composed_objects)if i!=omitted],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,composed_objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
for kind in ('retail','compiled'):
    r=subprocess.run([str(exe),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),'composed',kind,flush=True)
actual=(w/'compiled.bin').read_bytes();expected=(w/'retail.bin').read_bytes();assert actual==expected,(w,'composed');assert len(actual)==cases*(6*(4096+20)+144)
report.update(composed_cases=cases,composed_snapshots=cases*6,omission_checks=9)
report['source_sha256'].update({str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest()for p in extra_sources})
# Compose the three actual renderer setters too, observing their COM submissions.
renderer_source=ROOT/'rebuild/src/compiled/00/a0/CRenderManagerCore_MatrixSetters_00a0b0a0.cpp'
renderer_aliases={0xa0b0a0:'@RendererView@12',0xa0b140:'@RendererWorld@12',0xa0b320:'@RendererPerspective@24'}
for symbol,address in targets.items():
    if address in renderer_aliases:del aliases[symbol];aliases[renderer_aliases[address]]=symbol
(w/'camera_refresh_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")'for a,b in aliases.items())+'\n')
renderer_objects=composed_objects[:-1]
for i,path in enumerate((renderer_source,fixture)):
    obj=w/('renderer%d.obj'%i);r=V.cl(['/nologo','/c','/O2','/MT','/DCOMPOSED_SHADER','/DCOMPOSED_RENDERER','/I'+str(w),'/Fo'+str(obj),str(path)],env,w);assert not r.returncode,r.stdout+r.stderr;renderer_objects.append(obj)
for omitted in range(7):
    r=subprocess.run(cmd+[str(p)for i,p in enumerate(renderer_objects)if i!=omitted],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,renderer_objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
renderer_inputs=bytearray();renderer_cases=0
for fill,mode in itertools.product((0,0x55,0xaa,0xff),range(16)):
    for value_index in range(4):
        arena=bytearray([fill])*49152
        for index in range(4):struct.pack_into('<I',arena,0x6c+4*index,values[(value_index*3+index)%len(values)])
        renderer_inputs.extend(struct.pack('<I',mode));renderer_inputs.extend(arena);renderer_cases+=1
(w/'renderer-input.bin').write_bytes(renderer_inputs)
for kind in ('retail','compiled'):
    r=subprocess.run([str(exe),str(w/'renderer-input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),'renderer composed',kind,flush=True)
actual=(w/'compiled.bin').read_bytes();expected=(w/'retail.bin').read_bytes();assert actual==expected,(w,'renderer composed');assert len(actual)==renderer_cases*(6*(49152+20)+224)
report.update(renderer_composed_cases=renderer_cases,renderer_composed_snapshots=renderer_cases*6,omission_checks=16)
report['source_sha256'][str(renderer_source.relative_to(ROOT))]=hashlib.sha256(renderer_source.read_bytes()).hexdigest()
report['boundary']+=' Additional renderer composition executes all three actual renderer setters in both replays, observing IDirect3DDevice9 SetTransform slot44: full48KiB arenas, two renderer/device and shader instances, mutating callbacks/global reloads, HRESULT success/failure and53/64-bit x87 precision. Only box/window conversion and COM remain recorded boundaries in that replay.'
(ROOT/'rebuild/compile-gate/native-camera-refresh.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_CAMERA_REFRESH_GATE_PASS',report['snapshots']+report['composed_snapshots']+report['renderer_composed_snapshots'],'snapshots')
