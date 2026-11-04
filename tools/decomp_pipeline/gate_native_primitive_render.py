"""Native primitive render routing through the original component interface."""
import hashlib,json,re,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-primitive-render-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/b3/CEnginePrimitiveRenderer_Render_00b33010.cpp','01/39/CRenderStateManager_StaticData_01396da8.cpp','01/43/NEngineInternal_EffectGlobals_01436e40.cpp','01/43/NEngineInternal_ScreenEffectGlobals_01436e30.cpp','01/43/NEngineInternal_GPCamera_01436ea0.cpp','01/43/NEngineInternal_PrimitiveRenderGlobals_01436e3c.cpp')]
fixture=ROOT/'rebuild/tests/integration/NativePrimitiveRender_test.cpp';root='?Render@CEnginePrimitiveRenderer@@UAEXABVCEngineComponentRenderParams@@@Z'
specs=[
('?FableCurrentRenderStateManager_013cb508@@3PAVCRenderStateManager@@A',0x13cb508,'_StateGlobal',1),
('?GPShadowRenderer@NEngineInternal@@3PAVCEngineShadowRenderer@@A',0x1436e60,'_ShadowGlobal',2),
('?GPScreenEffectOutlineGlow@NEngineInternal@@3PAVCEngineScreenEffectOutlineGlow@@A',0x1436e3c,'_OutlineGlobal',5),
('?GPSubPrimitiveRenderer@NEngineInternal@@3PAVCEngineSubPrimitiveRenderer@@A',0x1436e74,'_SubGlobal',5),
('?GPScreenEffectDisplacementRenderer@NEngineInternal@@3PAVCEngineScreenEffectDisplacementRenderer@@A',0x1436e38,'_DisplaceGlobal',3),
('?GPCamera@NEngineInternal@@3PAVCEngineCamera@@A',0x1436ea0,'_CameraGlobal',4),
('?PreRenderLayer@CEnginePrimitiveRenderer@@QAEXW4EEngineRenderLayer@@@Z',0xb32ad0,'@Pre@12',None),
('?RenderNonsortedPrimitives@CEnginePrimitiveRenderer@@QAEXXZ',0xb32610,'@Nonsorted@8',None),
('?PeekSceneData@CEngineShadowRenderer@@QAE?BVCShadowRendererSceneData@@XZ',0xb38890,'@Peek@12',None),
('?RenderSceneShadowBuffer@CEngineScene@@QAEXW4EEngineRenderLayer@@ABVCShadowClipVolume@@@Z',0xb94060,'@Shadow@16',None),
('?RenderType@CEngineSubPrimitiveRenderer@@QAEXW4ESubPrimitiveType@@_N@Z',0xb849f0,'@RenderType@16',None),
('?GetCount@CEngineSubPrimitiveRenderer@@QBEKW4ESubPrimitiveType@@@Z',0xb842b0,'@Count@12',None),
('?ClearType@CEngineSubPrimitiveRenderer@@QAEXW4ESubPrimitiveType@@@Z',0xb847b0,'@Clear@12',None),
('?BeginOutlineRendering@CEngineScreenEffectOutlineGlow@@QAEXXZ',0xb89a20,'@OutlineBegin@8',None),
('?FinishOutlineRendering@CEngineScreenEffectOutlineGlow@@QAEXXZ',0xb89140,'@OutlineEnd@8',None),
('?BeginDisplacementRendering@CEngineScreenEffectDisplacementRenderer@@QAE_NXZ',0xb87680,'@DisplaceBegin@8',None),
('?FinishDisplacementRendering@CEngineScreenEffectDisplacementRenderer@@QAEXXZ',0xb869a0,'@DisplaceEnd@8',None),
('?BeginDisplacementStencilRendering@CEngineScreenEffectDisplacementRenderer@@QAEXXZ',0xb871b0,'@StencilBegin@8',None),
('?FinishDisplacementStencilRendering@CEngineScreenEffectDisplacementRenderer@@QAEXXZ',0xb86bb0,'@StencilEnd@8',None),
('?RenderScene@CEngineScene@@QAEXW4EEngineRenderLayer@@ABVCEngineCamera@@@Z',0xb93f40,'@Scene@16',None),
('?RenderSceneAndCheckVisibility@CEngineScene@@QAEXW4EEngineRenderLayer@@ABVCEngineCamera@@W4EEngineOcclusionLayer@@_N@Z',0xb942a0,'@Visible@24',None),
('?PostRenderLayer@CEnginePrimitiveRenderer@@QAEXXZ',0xb32230,'@Post@8',None),
('?RestoreCaptureBlock@CRenderStateManager@@QAEXXZ',0xa05840,'@Restore@8',None)]
targets={s:a for s,a,n,o in specs};aliases={'@RenderPrimitive@12':root}
for s,a,n,o in specs:aliases[n if o is not None else s]=s if o is not None else n
(w/'primitive_render_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();body=read_virtual_data(raw,0xb33010,892);fixups=[];indices={a:i for i,(s,a,n,o) in enumerate(specs)};md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
for i in md.disasm(body,0xb33010):
 if i.mnemonic in ('call','jmp') and i.operands[0].type==2 and not 0xb33010<=i.operands[0].imm<0xb3338c:
  fixups.append((i.address-0xb33010+i.imm_offset,indices[i.operands[0].imm],1))
 else:
  for o in i.operands:
   if o.type==3 and o.mem.disp in indices:fixups.append((i.address-0xb33010+i.disp_offset,indices[o.mem.disp],0))
assert len(fixups)==63
(w/'primitive_render_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for n,p in enumerate(sources+[fixture]):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proof=check(objects[0],root,0xb33010,892,63,targets)
for s,a,n,o in specs:
 if o is not None:assert coff_data(Coff(objects[o]),s,4)==read_virtual_data(raw,a,4)==bytes(4)
link=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(6):
 r=subprocess.run(link+[str(o) for n,o in enumerate(objects) if n!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);m=re.search(r'cases=(\d+) snapshots=(\d+)',r.stdout);assert m and int(m[1])==15360
report=dict(cases=int(m[1]),snapshots=int(m[2]),providers=[proof],omission_checks=6,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual primitive Render through canonical component second-base virtualslot4 and six actual singleton globals. Full48KiB plus globals, independent event/argument/state model across16layers,scene counts0/1/3,flags,raw enable bytes,shadow technique/count/begin conditions,scene filtering,callback global swaps and parameter changes,scene truncation. Shadow by-value result and caster clip152B checked. Setup/cleanup,capture restore and concrete drawing/effect operations remain callbacks. No rendered frame.')
(ROOT/'rebuild/compile-gate/native-primitive-render.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_PRIMITIVE_RENDER_GATE_PASS',report['snapshots'],'snapshots')
