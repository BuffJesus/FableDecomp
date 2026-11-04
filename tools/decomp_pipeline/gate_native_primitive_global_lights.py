"""Identity reset reaches actual shader state and COM matrix submission."""
import hashlib,json,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-primitive-global-lights-'));env=V.env()
specs=[
('00/b3/CEnginePrimitiveRenderer_PreRenderLayer_00b32ad0.cpp','?PreRenderLayer@CEnginePrimitiveRenderer@@QAEXW4EEngineRenderLayer@@@Z',0xb32ad0,947,21),
('00/b4/CEngineLightingManager_EnableFog_00b46890.cpp','?EnableFog@CEngineLightingManager@@QAEXXZ',0xb46890,109,1),
('00/98/CShaderRenderManager_ResetWorldTransform_00988290.cpp','?ResetWorldTransform@CShaderRenderManager@@QAEXXZ',0x988290,131,0),
('00/98/CShaderRenderManager_EnableVertexShaders_00987fe0.cpp','?EnableVertexShaders@CShaderRenderManager@@QAEXXZ',0x987fe0,61,1),
('00/98/CShaderRenderManager_EnablePixelShaders_00988110.cpp','?EnablePixelShaders@CShaderRenderManager@@QAEXXZ',0x988110,36,1),
('00/98/CShaderRenderManager_DisablePixelShaders_00988190.cpp','?DisablePixelShaders@CShaderRenderManager@@QAEXXZ',0x988190,85,0),
('00/9b/CDisplayManager_GetPixelShaderVersion_009bee00.cpp','?GetPixelShaderVersion@CDisplayManager@@QBEKXZ',0x9bee00,29,0),
('00/a0/CRenderManagerCore_EnableXBoxShaderConstants_00a0aa20.cpp','?EnableXBoxShaderConstants@CRenderManagerCore@@QAEX_N@Z',0xa0aa20,3,0),
('00/b3/CEnginePrimitiveRenderer_InitialiseGlobalLights_00b32300.cpp','?InitialiseGlobalLights@CEnginePrimitiveRenderer@@AAEXXZ',0xb32300,402,9),
('00/a0/CRenderManagerCore_SetMaterial_00a0aa10.cpp','?SetMaterial@CRenderManagerCore@@QAEXABVCRGBColour@@0@Z',0xa0aa10,3,0)]
sources=[ROOT/'rebuild/src/compiled'/p for p,*_ in specs]
sources += [ROOT/'rebuild/src/compiled'/p for p in ('01/43/NEngineInternal_GPShaderRenderManager_01436e14.cpp','01/43/NEngineInternal_Globals_01436e18.cpp','01/43/NEngineInternal_GPLightingManager_01436e9c.cpp','01/39/CRenderStateManager_D3DTextureOperationModeTable_01396e70.cpp','01/39/CRenderStateManager_D3DMaterialColourSourceTable_01396ff4.cpp')]
fixture=ROOT/'rebuild/tests/integration/NativePrimitiveGlobalLights_test.cpp'
sg='?GPShaderRenderManager@NEngineInternal@@3PAVCShaderRenderManager@@A';rg='?GPRenderManager@NEngineInternal@@3PAVCRenderManager@@A';lg='?GPLightingManager@NEngineInternal@@3PAVCEngineLightingManager@@A'
table='?D3DTextureOperationModeTable@CRenderStateManager@@0PAKA';material='?D3DMaterialColourSourceTable@CRenderStateManager@@0PAKA';init='?UpdateGlobalLightingStates@CEngineLightingManager@@QAEXXZ'
targets={table:0x1396e70,init:0xb46c80,material:0x1396ff4,sg:0x1436e14,rg:0x1436e18,lg:0x1436e9c};targets.update({sym:a for p,sym,a,l,c in specs})
aliases={'@PreLayer@12':specs[0][1],'_ShaderGlobal':sg,'_RenderGlobal':rg,'_LightingGlobal':lg,'_TextureTable':table,init:'@UpdateGlobalLights@8','_MaterialTable':material}
(w/'primitive_global_lights_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();body=bytearray(4096);fixups=[];md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
packed=[];offset=0
for p,sym,a,l,c in specs:
 packed.append((sym,a,l,c,offset));offset=(offset+l+15)&~15
body=body[:offset]
for sym,address,length,count,offset in packed:
 code=read_virtual_data(raw,address,length);body[offset:offset+length]=code
 for i in md.disasm(code,address):
  if i.mnemonic in ('call','jmp') and i.operands[0].type==2 and not address<=i.operands[0].imm<address+length:
   assert i.operands[0].imm in targets.values();fixups.append((offset+i.address-address+i.imm_offset,i.operands[0].imm,1))
  else:
   for o in i.operands:
    if o.type==3 and o.mem.disp in (0x1436e14,0x1436e18,0x1436e9c,0x1396e70,0x1396ff4):fixups.append((offset+i.address-address+i.disp_offset,o.mem.disp,0))
assert len(fixups)==sum(s[4] for s in specs)
(w/'primitive_global_lights_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
(w/'primitive_global_lights_retail.h').open('a').write('static const unsigned retailFunctions[][2]={'+','.join('{%d,%d}'%(a,o) for sym,a,l,c,o in packed)+'};\n')
objects=[]
for n,p in enumerate(sources+[fixture]):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proofs=[check(objects[n],sym,a,l,c,targets) for n,(sym,a,l,c,off) in enumerate(packed)]
for n,sym,address in ((10,sg,0x1436e14),(11,rg,0x1436e18),(12,lg,0x1436e9c)):assert coff_data(Coff(objects[n]),sym,4)==read_virtual_data(raw,address,4)==bytes(4)
assert coff_data(Coff(objects[13]),table,96)==read_virtual_data(raw,0x1396e70,96)
assert coff_data(Coff(objects[14]),material,12)==read_virtual_data(raw,0x1396ff4,12)
link=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(15):
 r=subprocess.run(link+[str(o) for n,o in enumerate(objects) if n!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);assert 'NATIVE_PRIMITIVE_GLOBAL_LIGHTS_PASS cases=6048 ' in r.stdout
report=dict(cases=6048,result=r.stdout.strip(),omission_checks=15,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual pre-layer shader selection, shader identity reset, inline state bookkeeping, actual fog enable, capability query, PC constants no-op, three globals and full96B texture-operation table. Full48KiB independent model; Actual InitialiseGlobalLights, PC SetMaterial no-op and material-source table; only COM pixel reset and lighting-manager UpdateGlobalLightingStates are callbacks. Shader/render global swaps during COM, mutable occlusion enable at lighting update callback, all seven fog layers, shadow bypass and unknown layers; noncanonical bool bytes. Null current pixel resource; existing resource deletion gate remains separate. No rendered frame.')
(ROOT/'rebuild/compile-gate/native-primitive-global-lights.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_PRIMITIVE_GLOBAL_LIGHTS_GATE_PASS')
