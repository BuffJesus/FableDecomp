"""UI text frame: actual Draw2DText/Draw3DScaledText/AddText enqueue and CRenderManager2D::Render2DDrawList
flush driving the actual static font chain, state-block library, texture attach and capture-block restore
against a recording fake device; compared with the transplanted retail Render2DDrawList/Render/Prepare/
Draw/Realise graph."""
from pathlib import Path
from datetime import datetime, timezone
import sys, json, hashlib, re, subprocess, struct
import argparse
R=Path(__file__).resolve().parents[2]
parser=argparse.ArgumentParser();parser.add_argument('--output',type=Path);args=parser.parse_args()
b=args.output.resolve() if args.output else R/'rebuild/build/native-ui-text-frame'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');b.mkdir(parents=True)
sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import check_one as V
from pe_oracle import EXE, read_virtual_data
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import oracle_native_render2d_list as listoracle
sha=lambda p:hashlib.sha256(Path(p).read_bytes()).hexdigest()
helpers=[
 '00/99/CBaseClass_CBaseClass_0099a2d0.cpp','00/99/CCharString_DefaultConstructor_0099e4b0.cpp','00/99/CCharString_Destructor_0099eae0.cpp','00/99/CCharString_UnassignString_0099e9b0.cpp',
 '00/9a/CFontBank_Preload_009a0b20.cpp','00/ab/CStaticFontBank_GetFontMaxHeight_00ab7a50.cpp','00/ab/CStaticFontBank_GetStringExtents_00ab7b00.cpp',
 '00/99/CWideString_Constructor_0099aed0.cpp','00/99/CWideString_Destructor_0099b510.cpp','00/99/CCharString_operatorcharconstptr_0099af00.cpp','00/99/CCharString_operatorcharconstptr2_0099af10.cpp',
 '00/9e/CFontManager_GetFreeVertexBuffer_009e2ce0.cpp','00/a6/CVertexBuffer_Dispatch_00a63240.cpp','00/a0/CRenderManagerCore_WaitWhileResourceIsBusy_00a0aa70.cpp',
 '00/9e/CVertexBufferWin32_GCVertexBufferWin32_009e2890.cpp','00/a6/CVertexBufferWin32_Initialise_00a63150.cpp','00/a6/CVertexBufferWin32_Uninitialise_00a63200.cpp',
 '00/ab/CStaticFontBank_PrepareToRender_00ab83a0.cpp','01/39/CRenderStateManager_StaticData_01396da8.cpp','00/a0/CRenderManagerCore_SetVertexFormat_00a0aa30.cpp','01/39/CRenderStateManager_D3DAlphaFuncModeTable_01396dd8.cpp','01/39/CRenderStateManager_D3DTextureOperationModeTable_01396e70.cpp',
 '00/a0/CRenderManagerCore_DrawCurrentBuffersAsTrisTextures_00a0abe0.cpp','00/a0/CRenderStateManager_RealiseRenderState_00a058c0.cpp',
 '00/a0/CRenderStateManager_Init_00a07900.cpp','00/a0/CRenderStateManager_StateSetup_00a04580.cpp','00/a0/CRenderStateManager_SetDefaultRenderStates_00a059e0.cpp','00/a0/CRenderStateManager_ForceResendRenderStatesToHardware_00a044e0.cpp',
 '00/9d/CRenderManager2D_Draw3DScaledText_009dbff0.cpp','00/9d/CRenderManager2D_AddText_009db700.cpp','00/a0/CRenderManagerCore_GetCurrentAWindow_00a0aaa0.cpp','00/9e/Global_GFVirtualToActualScalars_009e1e10.cpp','00/9b/CDisplayManager_GetRenderTargetDimensions_009bedc0.cpp',
 '00/9d/CQuickDrawTriInfo_CopyConstructor_009de890.cpp','00/9e/STLQuickDrawInfo_InsertOverflow_009e1750.cpp','00/9f/CTexture_CopyConstructor_009f9d00.cpp','00/9f/CTexture_Uninitialise_009f9f70.cpp',
 '00/9f/CResourceList_AddResource_009fbfe0.cpp','00/9f/CResourceList_RemoveResource_009fc020.cpp','00/9f/CResource_ListInsert_009fbf90.cpp','00/9f/CResource_ListRemove_009fbfb0.cpp',
 '00/9d/CRenderManager2D_Render2DDrawList_009da9f0.cpp','00/9f/CTexture_Assignment_009fa1c0.cpp','00/9a/CRenderManagerCore_AttachTextureToStage_009a0cf0.cpp','00/9e/CQuickDrawTriInfoVector_Erase_009e15e0.cpp','00/9e/Render2DVertexVector_Erase_009e1440.cpp','00/9f/CTexture_PreallocatedStorage_009fa230.cpp',
 '00/9d/CStateBlockFunctions_Apply_009df060.cpp','01/39/CRenderManager2D_StateBlockLibrary_01396510.cpp','00/a0/CRenderStateManager_RestoreCaptureBlock_00a05840.cpp','00/a0/CRenderManagerCore_SetAWindow_00a0aa80.cpp','01/39/VirtualDimensionsData_013961e8.cpp',
 '00/99/CWideString_LiteralConstructor_0099b6b0.cpp','00/99/CWideString_CopyConstructor_0099b720.cpp','00/99/CWideString_CopyAssignment_0099b7d0.cpp','01/39/CResourceBank_ResourceAbortingEnabled_01396c5c.cpp',
 '00/99/CWideString_AllocStringData_0099b3c0.cpp','00/99/CWideString_UnassignString_0099b4d0.cpp','00/99/STL_Mthrowlengtherror_0099c550.cpp',
 '00/9d/CRenderManager2D_Draw2DTriangle_009ddff0.cpp','00/9d/CRenderManager2D_Draw2DBox_009de3f0.cpp','00/9d/CRenderManager2D_Draw2DBoxWithZ_009ddc60.cpp','00/9d/CRenderManager2D_Draw2DLine_009dd960.cpp',
 '00/9d/CRenderManager2D_Add2DTri_009dc030.cpp','00/9d/CRenderManager2D_Add2DLine_009dc2c0.cpp','00/9e/STLRender2DVertex_InsertOverflow_009e14b0.cpp',
 '01/22/Global_ShaderRenderManager_01223d00.cpp','00/98/CShaderRenderManager_Constructor_0098ab00.cpp','00/98/CShaderRenderManager_Destructor_0098abf0.cpp','00/98/CShaderRenderManager_Uninitialise_0098a7a0.cpp',
 '00/98/CVertexShaderConstantLayout_Lifetime_0098d490.cpp','00/a5/CSimpleMemoryPool_Lifetime_00a5c570.cpp','00/98/CShaderRenderManager_Initialise_0098acf0.cpp',
 '00/98/CShaderRenderManager_EnableVertexShaders_00987fe0.cpp','00/98/CShaderRenderManager_DisableVertexShaders_009880e0.cpp','00/98/CShaderRenderManager_EnablePixelShaders_00988110.cpp','00/98/CShaderRenderManager_DisablePixelShaders_00988190.cpp',
 '00/9b/CDisplayManager_GetPixelShaderVersion_009bee00.cpp','00/a0/CRenderManagerCore_EnableXBoxShaderConstants_00a0aa20.cpp','00/98/CShaderRenderManager_ApplyVertexShader_00988020.cpp',
 '00/98/CShaderRenderManager_RegisterVertexShaderConstantLayout_0098b230.cpp','00/98/CShaderRenderManager_SetVertexShaderConstantLayout_0098b5e0.cpp','00/98/CShaderConstantRange_NotEqual_00987d70.cpp','00/98/CShaderRenderManager_SetVSConstants_0098b930.cpp',
 '00/98/CShaderRenderManager_UpdateAmbient_00989760.cpp','00/98/CShaderRenderManager_UpdateCombinedProjectionTransform_00988a50.cpp','00/98/CShaderRenderManager_UpdateFogColour_009897c0.cpp','00/98/CShaderRenderManager_UpdateLightGlobals_0098a760.cpp',
 '00/98/CShaderRenderManager_UpdatePixelShaderConstants_00989bf0.cpp','00/98/CShaderRenderManager_UpdatePixelShader_00988a20.cpp','00/98/CShaderRenderManager_ResetWorldTransform_00988290.cpp',
 '01/22/Global_2DShaderConstantLayout_01224140.cpp','00/98/CVertexShaderConstantLayout_PeekName_0098d6f0.cpp',
 '01/3d/Global_GEnableSIMD_013d2880.cpp','00/98/CPixelShader_CountReferences_00987ee0.cpp','00/98/CShaderBase_GetDebugName_00987cf0.cpp',
 '00/99/CCharString_Constructor_0099ebf0.cpp',
 '00/98/STLVector4_Erase_0098cf70.cpp','00/98/STLVector4_Insert_0098d360.cpp','00/98/STLShaderLayoutPointerVector_Resize_0098d870.cpp','00/98/STLShaderLayoutPointerVector_FillInsert_0098d000.cpp',
 '00/98/IndexedCharArray_PushBack_0098db20.cpp','00/43/STL_DefStringMapErase_0043020a.cpp','00/98/CShaderRenderManager_AllocateLights_0098b420.cpp',
 '00/99/CCharString_AllocStringData_0099ea60.cpp','00/99/CCharString_CCharString_0099ec30.cpp',
 '00/43/STLStringVector_Minsertoverflow_00433530.cpp','00/43/STL_insertunique_00431606.cpp','00/43/STL_Minsert_004316cf.cpp','00/43/global_Alloc_00431773.cpp','00/41/global_global_00411570.cpp',
 '00/9a/CCharString_InitFromCStr_009a0590.cpp','00/9a/global_CBasicStringCharAssignFromData_009a0300.cpp',
 '00/a5/CSimpleMemoryPool_Allocate_00a5c910.cpp','00/a5/CSimpleMemoryPool_BlockLifetime_00a5c890.cpp','00/a5/CSimpleMemoryPool_FreeAll_00a5c720.cpp','00/42/CIVCountedPointeeBase_DeleteCIVCountedPointeeBaseUBEXXZ_0042d930.cpp',
 '00/99/CCharString_ConversionOperator_0099e4c0.cpp','00/99/CCharString_CopyAssignment_0099efb0.cpp','00/99/CDataInputStream_CDataInputStream_00993b20.cpp','00/99/CInitBaseClass_IsValid_0099a330.cpp','00/99/CMemoryDataInputStream_DestructorTyped_00993b10.cpp','00/99/CMemoryDataInputStream_ReadNullTerminatedString_00995d30.cpp','00/99/DataInputStream_ReadWithSrcChunkOverflow_00993ca0.cpp','00/9b/CDisplayManager_GetBestCompressedAlphaTextureFormat_009be870.cpp','00/9e/CPixelFormat_GetByteColourDepth_009e3800.cpp','00/9e/CPixelFormat_GetColourDepth_009e3820.cpp','00/9e/CPixelFormat_InitialiseRGBA_009e3790.cpp','00/9e/CPixelFormat_Initialise_009e3830.cpp','00/9e/CPixelFormat_IsEquivalentTyped_009e37f0.cpp','00/9e/CPixelFormat_IsSigned_009e3970.cpp','00/9f/CSurface_CanDoProperSignedConversion_009f35d0.cpp','00/9f/CSurface_CopyConstructor_009f2d60.cpp','00/9f/CSurface_CopyFromMemory_009f4390.cpp','00/9f/CSurface_InitialiseFromTextureMipmap_009f2f10.cpp','00/9f/CSurface_Uninitialise_009f2e20.cpp','00/9f/CTexture_GetSurfaceLevel_009f9e00.cpp','00/9f/CTexture_Initialise_009fa280.cpp','00/ab/CGlyph_Read_00ab96b0.cpp','00/ab/CStaticFontBank_Lifetime_00ab7a70.cpp','00/ab/CStaticFontBank_Constructor_00ab8e10.cpp','01/29/CPixelFormat_Table_0129ba40.cpp',
]
sources=[R/'rebuild/src/compiled/00/ab/CStaticFontBank_Render_00ab7c20.cpp']+[R/'rebuild/src/compiled'/p for p in helpers]+[R/'rebuild/tests/integration/NativeUITextFrame_test.cpp']
index={p.name:i for i,p in enumerate(sources)}
inc=b/'include';inc.mkdir();inputs={EXE,Path(__file__)};seen=set()
# Dependencies: the font draw composition and the Render2DDrawList retail comparison still describe these sources.
dependencies={}
path=R/'rebuild/compile-gate/native-static-font-draw.json';receipt=json.loads(path.read_text());assert receipt['status']=='PASS' and receipt['repeat'];inputs.add(path)
for p,h in receipt['input_sha256'].items():
 if Path(p).suffix in ('.h','.cpp','.py') and 'NativeStaticFontDraw_test' not in p:assert sha(p)==h,p;inputs.add(Path(p))
dependencies['native-static-font-draw.json']=receipt['build']
path=R/'rebuild/compile-gate/native-shader-combined-projection.json';receipt=json.loads(path.read_text());assert receipt['status']=='PASS' and receipt['repeat'];inputs.add(path)
for p,h in receipt['input_sha256'].items():
 if Path(p).suffix in ('.h','.cpp','.py') and 'NativeShaderCombinedProjection_test' not in p:assert sha(p)==h,p;inputs.add(Path(p))
dependencies['native-shader-combined-projection.json']=receipt['build']
path=R/'rebuild/compile-gate/native-static-font-upload.json';receipt=json.loads(path.read_text());inputs.add(path)
for p,h in receipt['input_sha256'].items():
 if Path(p).suffix in ('.cpp',) and 'NativeStaticFontUpload_test' not in p:assert sha(p)==h,p;inputs.add(Path(p))
dependencies['native-static-font-upload.json']=receipt['build']
path=R/'rebuild/compile-gate/native-render2d-list.json';receipt=json.loads(path.read_text());inputs.add(path)
assert sha(sources[index['CRenderManager2D_Render2DDrawList_009da9f0.cpp']])==receipt['source_sha256'];dependencies['native-render2d-list.json']=receipt['source_sha256']
def closure(p):
 if p in seen:return
 seen.add(p);inputs.add(p)
 for name in re.findall(r'^\s*#include\s*"([^"\n]+)"',p.read_text(),re.M):
  closure(next(q for q in (p.parent/name,R/'rebuild/include'/name) if q.is_file()))
for p in sources:closure(p)
for p in seen:
 if p.is_relative_to(R/'rebuild/include'):
  q=inc/p.relative_to(R/'rebuild/include');q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(p.read_bytes())
inputs.update(inc.rglob('*.h'))
inputs.update(p for p in (V.VC/'include').rglob('*') if p.is_file())
inputs.update(p for d in (V.VC/'bin',V.VC/'lib') for p in d.iterdir() if p.is_file() and p.suffix.lower() in ('.exe','.dll','.lib'))
for module in tuple(sys.modules.values()):
 name=getattr(module,'__file__',None)
 if name:
  p=Path(name).resolve()
  if p.suffix=='.py' and p.is_relative_to(R/'tools/decomp_pipeline'):inputs.add(p)
pins={str(p):sha(p) for p in inputs};(b/'inputs.json').write_text(json.dumps(pins,indent=2))
env=V.env();env['INCLUDE']=str(V.VC/'include')+';'+str(inc);objects=[]
def compile_unit(source,obj,cwd,name):
 x=V.cl(['/nologo','/c','/O2','/MT','/Gy']+(['/GS'] if 'Render2DDrawList' in name else [])+['/I'+str(inc),'/Fo'+str(obj),str(source)],env,cwd);assert not x.returncode,(name,x.stdout+x.stderr)
for i,p in enumerate(sources):
 snapshot=b/('unit'+str(i)+'.cpp');snapshot.write_bytes(p.read_bytes());obj=snapshot.with_suffix('.obj');compile_unit(snapshot,obj,b,p.name);objects.append(obj)
def link(exe,items,cwd):
 return subprocess.run([str(V.VC/'bin/link.exe'),'/nologo','/OPT:REF','/map:'+str(exe.with_suffix('.map')),'/out:'+str(exe),*map(str,items)],env=env,cwd=cwd,capture_output=True,text=True)
exe=b/'probe.exe';x=link(exe,objects,b);(b/'link.json').write_text(json.dumps(dict(code=x.returncode,stdout=x.stdout,stderr=x.stderr)));assert not x.returncode,x.stdout+x.stderr
def read_map(path):
 mapped={}
 for line in path.read_text().splitlines():
  m=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
  if m:mapped[m[1]]=int(m[2],16)
 return mapped
alias={}
# Retail block: Render, Prepare, Draw, Realise, Render2DDrawList; internal calls stay inside the block.
RENDER_AT,PREPARE_AT,DRAW_AT,REALISE_AT,LIST_AT,BLOCK=0,2048,4864,5120,5376,12288
# Retail shader-manager functions carried by the retail graph (address: verified extent; ApplyVertexShader and
# UpdateCombinedProjectionTransform run past their manifest lengths 176/975 to ret+int3 at 187/978).
# UpdatePixelShaderConstants (CRT static-array init) and SetVertexShaderConstantLayout (unrecreated callees)
# stay recreated in both modes.
SHADER_RETAIL={0x987fe0:61,0x988020:187,0x9880e0:44,0x988110:36,0x988190:85,0x988290:131,0x988a20:41,0x988a50:978,0x989760:85,0x9897c0:99,0x98a760:54,0x98b230:73,0x98b930:37}
SHADER_EXTERNAL={0xa0aa20:'?EnableXBoxShaderConstants@CRenderManagerCore@@QAEX_N@Z',0x9bee00:'?GetPixelShaderVersion@CDisplayManager@@QBEKXZ',0x98d870:'?resize@?$vector@PBVCVertexShaderConstantLayout@@V?$allocator@PBVCVertexShaderConstantLayout@@@_STL@@@_STL@@QAEXIABQBVCVertexShaderConstantLayout@@@Z'}
SHADER_AT={};_at=8736
for _a in sorted(SHADER_RETAIL):SHADER_AT[_a]=_at;_at=(_at+SHADER_RETAIL[_a]+15)&~15
assert _at<=BLOCK
data=EXE.read_bytes();block=bytearray(BLOCK);specs=[];md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
def place(address,size,at,tail):
 code=read_virtual_data(data,address,size);assert code[-len(tail):]==tail,hex(address);block[at:at+size]=code;return code
def absolute_operands(i):
 return [(o.imm&0xffffffff) if o.type==2 else (o.mem.disp&0xffffffff) for o in i.operands if (o.type==2 and 0x400000<=o.imm&0xffffffff<0x1600000) or (o.type==3 and o.mem.base==0 and 0x400000<=o.mem.disp&0xffffffff<0x1600000)]
code=place(0xab7c20,1910,RENDER_AT,b'\xc2\x18\0')
targets={0x9a4ec0:'?GFGetSystemManager@@YIPAVCSystemManager@@XZ',0x9e2ce0:'?GetFreeVertexBuffer@CFontManager@@QAEAAVCVertexBufferWin32@@XZ',0x99af00:'??BCWideString@@QBEPBGXZ',0x99af10:'?GetAsCharArray@CWideString@@QBEPBGXZ',0xbfeb8a:'_floor',0xac20a0:'?Lock@CVertexBufferBase@@QAEPAEKKK@Z',0xac20b0:'?Unlock@CVertexBufferBase@@QAEXXZ'}
constants={0x122f59c:'_FontHalf',0x122ded8:'_FontOne',0x122dcb4:'_FontUnsignedBias'}
for address,name in constants.items():assert read_virtual_data(data,address,4)==struct.pack('<f',{'_FontHalf':.5,'_FontOne':1.,'_FontUnsignedBias':4294967296.}[name])
# The fixture defines the vertex RHW clamp; it must hold the retail image value.
assert read_virtual_data(data,0x139652c,4)==struct.pack('<f',100.0) and 'float g_FableVertexReciprocalLimit_0139652C=100.0f;' in sources[-1].read_text()
for i in md.disasm(code,0xab7c20):
 if i.mnemonic=='call' and i.operands[0].type==2:
  t=i.operands[0].imm;site=RENDER_AT+i.address-0xab7c20+i.imm_offset
  specs.append((site,DRAW_AT,2) if t==0xa0abe0 else (site,(targets[t],0),1))
 for address,name in constants.items():
  value=struct.pack('<I',address)
  if value in i.bytes:specs.append((RENDER_AT+i.address-0xab7c20+bytes(i.bytes).index(value),(name,0),0))
code=place(0xab83a0,2672,PREPARE_AT,b'\xc2\x04\0')
ptargets={0x9a4ec0:'?GFGetSystemManager@@YIPAVCSystemManager@@XZ',0xa0aa30:'?SetVertexFormat@CRenderManagerCore@@QAEXK@Z'}
pconstants={address:('?FableRenderDefaultValue_'+format(address,'08x')+'@@3KA',0) for address in (0x1396f78,0x1396f7c,0x1396df0,0x1396fa4,0x1396fb0,0x1396e5c,0x1396dc8,0x1396fcc,0x1396e7c,0x1396ef0,0x1396eec,0x1396e70)}
pconstants[0x1396e64]=('?D3DTextureMipMapModeTable@CRenderStateManager@@0PAKA',0)
pconstants[0x1396df0]=('?D3DAlphaFuncModeTable@CRenderStateManager@@0PAKA',24)
pconstants[0x1396e7c]=('?D3DTextureOperationModeTable@CRenderStateManager@@0PAKA',12)
pconstants[0x1396e70]=('?D3DTextureOperationModeTable@CRenderStateManager@@0PAKA',0)
for i in md.disasm(code,0xab83a0):
 if i.mnemonic=='call' and i.operands[0].type==2:specs.append((PREPARE_AT+i.address-0xab83a0+i.imm_offset,(ptargets[i.operands[0].imm],0),1))
 for address,named in pconstants.items():
  value=struct.pack('<I',address)
  if value in i.bytes:specs.append((PREPARE_AT+i.address-0xab83a0+bytes(i.bytes).index(value),named,0))
code=place(0xa0abe0,127,DRAW_AT,b'\xc2\x10\0')
for i in md.disasm(code,0xa0abe0):
 if i.mnemonic=='call' and i.operands[0].type==2:assert i.operands[0].imm==0xa058c0;specs.append((DRAW_AT+i.address-0xa0abe0+i.imm_offset,REALISE_AT,2))
 elif not i.mnemonic.startswith('j'):assert not absolute_operands(i),i
code=place(0xa058c0,167,REALISE_AT,b'\xc3')
for i in md.disasm(code,0xa058c0):assert not (i.mnemonic=='call' and i.operands[0].type==2) and (i.mnemonic.startswith('j') or not absolute_operands(i)),i
# Render2DDrawList: every direct call and absolute data operand must resolve to a named oracle target.
code=place(0x9da9f0,3344,LIST_AT,b'\xc2\x04\0')
named={v:k for k,v in listoracle.targets.items()};sizes={0x1396510:28,0x13bc470:0x3e8,0x13ca8a8:0x14c,0x13cb508:4,0x1396e64:4}
data_targets=sorted(v for v in named if v>=0x1000000);calls=refs=decoded=0;restore_site=None
for i in md.disasm(code,0x9da9f0):
 decoded+=i.size;site0=LIST_AT+i.address-0x9da9f0
 if i.mnemonic=='call' and i.operands[0].type==2:
  t=i.operands[0].imm;calls+=1
  if t==0xa05840:restore_site=site0
  specs.append((site0+i.imm_offset,REALISE_AT,2) if t==0xa058c0 else (site0+i.imm_offset,SHADER_AT[t],2) if t in SHADER_AT else (site0+i.imm_offset,(named[t],0),1));continue
 if i.mnemonic.startswith('j'):continue
 for value in absolute_operands(i):
  base=max(t for t in data_targets if t<=value);offset=value-base;assert offset<sizes.get(base,4),(hex(value),hex(base))
  raw=bytes(i.bytes);k=raw.rfind(struct.pack('<I',value));assert k>=0 and raw.count(struct.pack('<I',value))==1,i
  specs.append((site0+k,(named[base],offset),0));refs+=1
assert decoded==3344 and calls==57 and refs==70 and restore_site is not None,(decoded,calls,refs)
shader_refs=0
for address,length in sorted(SHADER_RETAIL.items()):
 at=SHADER_AT[address];code=read_virtual_data(data,address,length);assert code[-1:]==b'\xc3' or code[-3:-2]==b'\xc2',hex(address)
 assert read_virtual_data(data,address+length,1)==b'\xcc',hex(address);block[at:at+length]=code
 for i in md.disasm(code,address):
  site=at+i.address-address
  if i.mnemonic=='call' and i.operands[0].type==2:
   t=i.operands[0].imm;specs.append((site+i.imm_offset,SHADER_AT[t],2) if t in SHADER_AT else (site+i.imm_offset,(SHADER_EXTERNAL[t],0),1));shader_refs+=1;continue
  if i.mnemonic.startswith('j'):continue
  for value in absolute_operands(i):
   assert value==0x13d2880,(hex(address),hex(value));raw=bytes(i.bytes);specs.append((site+raw.rindex(struct.pack('<I',value)),('?GEnableSIMD@@3_NA',0),0));shader_refs+=1
assert shader_refs==6,shader_refs  # 2 EnableXBoxShaderConstants, GetPixelShaderVersion, layout resize, 2 GEnableSIMD loads
(b/'original.bin').write_bytes(block)
def patch_map(path,mapping,items=None):
 m=read_map(mapping);patches=[(site,target if kind==2 else m[alias.get(target[0],target[0])]+target[1],kind) for site,target,kind in (items or specs)]
 path.write_bytes(struct.pack('<I',len(patches))+b''.join(struct.pack('<III',*p) for p in patches));return patches
(b/'patches.json').write_text(json.dumps(patch_map(b/'patches.bin',b/'probe.map'),indent=2))
standalone=subprocess.run([str(exe)],capture_output=True,text=True,timeout=300)
standalone_record=dict(code=standalone.returncode,stdout=standalone.stdout,stderr=standalone.stderr)
(b/'standalone.json').write_text(json.dumps(standalone_record,indent=2));assert not standalone.returncode,standalone_record
x=subprocess.run([str(exe),str(b/'original.bin'),str(b/'patches.bin')],capture_output=True,text=True,timeout=300)
positive=dict(code=x.returncode,stdout=x.stdout,stderr=x.stderr);(b/'run.json').write_text(json.dumps(positive,indent=2));print(positive,flush=True)
assert not x.returncode and 'UI_TEXT_FRAME_PASS cases=240 paths=480' in positive['stdout'],positive
# Controls: each fault is relinked with its own map and freshly bound retail patches.
out=b/'controls';out.mkdir();records=[]
kept_symbols=sorted({'/include:'+alias.get(target[0],target[0]) for site,target,kind in specs if kind!=2})
def run_fault(name,fault,block_path,patches):
 x=subprocess.run([str(fault),str(block_path),str(patches)],cwd=out,capture_output=True,text=True,timeout=300)
 record=dict(name=name,code=x.returncode,stdout=x.stdout,stderr=x.stderr[-2000:]);assert x.returncode==80 and 'UI_TEXT_FAILURE' in x.stderr,record
 records.append(record);(out/'progress.json').write_text(json.dumps(records,indent=2));print(name,'rejected',flush=True)
def reject(name,items,missing=False):
 fault=out/('fault'+str(len(records))+'.exe');x=link(fault,[*kept_symbols,*items],out)
 if missing:
  assert x.returncode,(name,x.stdout,x.stderr);records.append(dict(name=name,link_rejected=True,stdout=x.stdout,stderr=x.stderr));print(name,'rejected',flush=True);return
 assert not x.returncode,x.stdout+x.stderr
 patches=fault.with_suffix('.patches.bin');patch_map(patches,fault.with_suffix('.map'));run_fault(name,fault,b/'original.bin',patches)
mutants=[
 ('list-no-prepare','CRenderManager2D_Render2DDrawList_009da9f0.cpp','info->PFont->PrepareToRender(true);',';'),
 ('list-no-capture-restore','CRenderManager2D_Render2DDrawList_009da9f0.cpp','FableCurrentRenderStateManager_013cb508->RestoreCaptureBlock();',';'),
 ('list-stage-kept','CRenderManager2D_Render2DDrawList_009da9f0.cpp',' AttachTextureToStage(0,0);',' ;'),
 ('list-window-kept','CRenderManager2D_Render2DDrawList_009da9f0.cpp','if(memcmp(&window,&info->Window,16)!=0){SetAWindow(info->Window);window=info->Window;}',';'),
 ('list-unoffset-position','CRenderManager2D_Render2DDrawList_009da9f0.cpp','C3DVector position(info->StringX+info->Window.left,','C3DVector position(info->StringX,'),
 ('list-queue-kept','CRenderManager2D_Render2DDrawList_009da9f0.cpp','QuickDraw2DTriInfo.erase(QuickDraw2DTriInfo.begin(),QuickDraw2DTriInfo.end());',';'),
 ('list-blend-merged','CRenderManager2D_Render2DDrawList_009da9f0.cpp','currentTexture.PD3DTexture!=batchTexture.PD3DTexture || blend!=nextBlend ||','currentTexture.PD3DTexture!=batchTexture.PD3DTexture ||'),
 ('list-blend-not-applied','CRenderManager2D_Render2DDrawList_009da9f0.cpp','if(nextBlend!=blend){LibraryStateBlockFunctions[nextBlend]->Apply();blend=nextBlend;}','if(nextBlend!=blend){blend=nextBlend;}'),
 ('list-line-kept','CRenderManager2D_Render2DDrawList_009da9f0.cpp','if(changedLine){line=!line;changedLine=false;}','if(changedLine){changedLine=false;}'),
 ('addtext-offscreen-ignored','CRenderManager2D_AddText_009db700.cpp','if (DisplayManager->ViewportOffscreen)\n        return;',';'),
 ('addtext-string-dropped','CRenderManager2D_AddText_009db700.cpp','info.String = text;',';'),
 ('restore-mask-kept','CRenderStateManager_RestoreCaptureBlock_00a05840.cpp','CurrentBookmarkMask >>= 1;',';'),
 ('attach-cache-kept','CRenderManagerCore_AttachTextureToStage_009a0cf0.cpp','CurrentlySelectedTextures[stage] = deviceTexture;',';'),
]
for name,filename,old,new in mutants:
 i=index[filename];text=sources[i].read_text();assert text.count(old)==1,(name,text.count(old))
 path=out/(name+'.cpp');path.write_text(text.replace(old,new));obj=path.with_suffix('.obj');compile_unit(path,obj,out,filename)
 items=list(objects);items[i]=obj;reject(name,items)
for filename in ('CRenderManager2D_Render2DDrawList_009da9f0.cpp','CRenderManager2D_AddText_009db700.cpp','CRenderManager2D_Draw3DScaledText_009dbff0.cpp','CRenderStateManager_RestoreCaptureBlock_00a05840.cpp','CRenderManagerCore_AttachTextureToStage_009a0cf0.cpp','CStateBlockFunctions_Apply_009df060.cpp','CRenderManager2D_StateBlockLibrary_01396510.cpp','CRenderManagerCore_SetAWindow_00a0aa80.cpp','CQuickDrawTriInfoVector_Erase_009e15e0.cpp','STLQuickDrawInfo_InsertOverflow_009e1750.cpp','CRenderManager2D_Add2DTri_009dc030.cpp','CRenderManager2D_Add2DLine_009dc2c0.cpp','CRenderManager2D_Draw2DBoxWithZ_009ddc60.cpp','STLRender2DVertex_InsertOverflow_009e14b0.cpp','Global_ShaderRenderManager_01223d00.cpp','CShaderRenderManager_Initialise_0098acf0.cpp','CShaderRenderManager_EnableVertexShaders_00987fe0.cpp','CShaderRenderManager_ApplyVertexShader_00988020.cpp','CShaderRenderManager_SetVertexShaderConstantLayout_0098b5e0.cpp','CShaderRenderManager_UpdateCombinedProjectionTransform_00988a50.cpp','Global_2DShaderConstantLayout_01224140.cpp','Global_GEnableSIMD_013d2880.cpp','CStaticFontBank_Constructor_00ab8e10.cpp','CStaticFontBank_Lifetime_00ab7a70.cpp','CGlyph_Read_00ab96b0.cpp'):
 reject('missing-'+filename,[p for i,p in enumerate(objects) if i!=index[filename]],True)
# Retail-graph control: the transplanted Render2DDrawList skips its capture-block restore.
skip=bytearray(block);assert skip[restore_site]==0xe8;skip[restore_site:restore_site+5]=b'\x90'*5
(out/'original-no-restore.bin').write_bytes(skip);kept=[s for s in specs if s[0]!=restore_site+1];assert len(kept)==len(specs)-1
patch_map(out/'original-no-restore.patches.bin',b/'probe.map',kept);run_fault('original-list-without-capture-restore',exe,out/'original-no-restore.bin',out/'original-no-restore.patches.bin')
# Retail-graph control: the transplanted retail ApplyVertexShader returns at once, so the shader never binds.
skip=bytearray(block);at=SHADER_AT[0x988020];skip[at:at+3]=b'\xc2\x08\x00'
(out/'original-no-apply.bin').write_bytes(skip);patch_map(out/'original-no-apply.patches.bin',b/'probe.map');run_fault('original-retail-apply-vertex-shader-skipped',exe,out/'original-no-apply.bin',out/'original-no-apply.patches.bin')
again=subprocess.run([str(exe),str(b/'original.bin'),str(b/'patches.bin')],capture_output=True,text=True,timeout=300)
assert not again.returncode and again.stdout==positive['stdout'],again
assert all(sha(p)==h for p,h in pins.items()),'input drift'
report=dict(status='PASS',cases=240,original_paths=480,repeat=True,controls=records,run=positive,standalone=standalone_record,dependencies=dependencies,build=str(b),input_sha256=pins,objects={str(p):sha(p) for p in objects},object_sources={str(o):str(p) for o,p in zip(objects,sources)},boundary='Fonts load through the actual CStaticFontBank constructor from generated bank entries (glyph reader, DXT3 negotiation, texture creation and A8R8G8B8 surface upload through D3D/D3DX adapters) and are destroyed by the actual destructor after the frames (textures released, names freed); the font-manager assignment is a boundary. Public 2D text API enqueue (Draw2DText/Draw3DScaledText/AddText with real strings, virtual-to-actual scaling, record copies and vector growth) and Render2DDrawList flush through real state-block library, capture-block restore, texture attach, static font Prepare/Render, Win32 vertex buffers, DrawCurrentBuffersAsTris and RealiseRenderState; one state-manager Init, frames chained. Retail Render2DDrawList/Render/Prepare/Draw/Realise transplanted graph equals the recreated graph in every device/shader/viewport event, vertex payload byte and manager/shader byte (2D vector heap pointers compared by size and capacity). The actual shader-manager global (CRT-constructed), Initialise, enable/disable, vertex-shader apply, constant-layout register/select, SetVSConstants and Update* family run in both modes (each proven against retail by its own gate; here composed; thirteen retail shader-manager functions (enable/disable, apply, reset world, Update* except pixel-shader constants, register layout, SetVSConstants) are also in the retail graph, UpdatePixelShaderConstants and SetVertexShaderConstantLayout stay recreated in both modes); shader resources (device handles of the 2D vertex shader) and light pools are boundaries; CDisplayManager::SetViewport (retail inline-asm fistp) is a recorded boundary; fake COM device; borrowed font storage. Mixed text, triangle, box and line queues with solid/additive/alpha blends; untextured geometry; geometry vertices compared with the never-written specular colour masked; no graphics API, raster or game claim.')
(b/'receipt.json').write_text(json.dumps(report,indent=2))
if args.output is None:(R/'rebuild/compile-gate/native-ui-text-frame.json').write_text(json.dumps(report,indent=2))
print('NATIVE_UI_TEXT_FRAME_GATE_PASS cases=240 original_paths=480 controls='+str(len(records)),b,flush=True)
