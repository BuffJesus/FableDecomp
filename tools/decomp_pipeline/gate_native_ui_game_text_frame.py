"""UI game-text frame: strings loaded from the actual NGameText::CDataBank (generated RESOURCE_TEST retail bank,
entries held through the frames) queued beside fixed strings, over the accepted UI bank frame: the accepted UI text frame (actual 2D enqueue, Render2DDrawList flush, static font chain,
state-block library, texture attach, capture-block restore; compared with the transplanted retail
Render2DDrawList/Render/Prepare/Draw/Realise + shader graph) over fonts loaded from a generated BIGB font bank
by the actual retail bank manager and the actual CFontManager (symbol map, eight Win32 vertex buffers,
GetStaticFont -> AcquireFont -> CStaticFontBank constructor), torn down by the manager destructor."""
from pathlib import Path
from datetime import datetime, timezone
import sys, json, hashlib, re, subprocess, struct
import argparse
R=Path(__file__).resolve().parents[2]
parser=argparse.ArgumentParser();parser.add_argument('--output',type=Path);args=parser.parse_args()
b=args.output.resolve() if args.output else R/'rebuild/build/native-ui-game-text-frame'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');b.mkdir(parents=True)
sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import check_one as V
from pe_oracle import EXE, read_virtual_data
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import oracle_native_render2d_list as listoracle
import gate_native_bank_shared_worker as W
from native_ui_bank_frame_sources import SOURCES as BANK_SOURCES
from native_font_bank_archive import create_font_archive
from native_text_retail_schema_fixture import create_archive
sha=lambda p:hashlib.sha256(Path(p).read_bytes()).hexdigest()
FIXTURE=R/'rebuild/tests/integration/NativeUIGameTextFrame_test.cpp'
FIXTURES=[R/'rebuild/tests/integration/NativeTextBeginLoadingGlobals_test.cpp',FIXTURE,R/'rebuild/tests/integration/NativeUIBankFrameBankOpen.cpp',R/'rebuild/tests/integration/NativeUIBankFrameData.cpp']
sources=[R/'rebuild/src/compiled'/p for p,f in BANK_SOURCES]+FIXTURES
flags_of={str(R/'rebuild/src/compiled'/p):list(f) for p,f in BANK_SOURCES}
assert len(set(sources))==len(sources)
index={}
for i,p in enumerate(sources):assert p.name not in index,p.name;index[p.name]=i
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
path=R/'rebuild/compile-gate/native-ui-bank-frame.json';receipt=json.loads(path.read_text());assert receipt['status']=='PASS' and receipt['repeat'];inputs.add(path)
for p,h in receipt['input_sha256'].items():
 if Path(p).suffix in ('.h','.cpp','.py') and 'NativeUIBankFrame_test' not in p and 'gate_native_ui_bank_frame' not in p:assert sha(p)==h,p
dependencies['native-ui-bank-frame.json']=receipt['build']
for report_name in ('native-streaming-font-entry.json','native-font-vertex-buffer-fill-insert.json','native-shader-layout-fill-insert.json','native-font-bank-entry-added.json','native-font-entry-unload.json'):
 path=R/'rebuild/compile-gate'/report_name;receipt=json.loads(path.read_text());assert receipt['status']=='PASS',report_name;inputs.add(path)
 for p,h in receipt['input_sha256'].items():
  if Path(p).suffix=='.cpp':assert sha(R/p)==h,(report_name,p);inputs.add(R/p)
 dependencies[report_name]=receipt['input_sha256']
def closure(p):
 if p in seen:return
 seen.add(p);inputs.add(p)
 for name in re.findall(r'^\s*#include\s*"([^"\n]+)"',p.read_text(),re.M):
  closure(next(q for q in (p.parent/name,R/'rebuild/include'/name,R/'rebuild/tests/integration'/name) if q.is_file()))
for p in sources:closure(p)
for p in seen:
 if p.is_relative_to(R/'rebuild/include'):q=inc/p.relative_to(R/'rebuild/include')
 elif p.is_relative_to(R/'rebuild/tests/integration') and p not in FIXTURES:q=inc/p.relative_to(R/'rebuild/tests/integration')
 else:continue
 q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(p.read_bytes())
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
def compile_unit(source,obj,cwd,name,flags=()):
 x=V.cl(['/nologo','/c','/O2','/MT','/Gy']+(['/GS'] if 'Render2DDrawList' in name else [])+list(flags)+['/I'+str(inc),'/Fo'+str(obj),str(source)],env,cwd);assert not x.returncode,(name,x.stdout+x.stderr)
for i,p in enumerate(sources):
 snapshot=b/('unit'+str(i)+'.cpp');snapshot.write_bytes(p.read_bytes());obj=snapshot.with_suffix('.obj');compile_unit(snapshot,obj,b,p.name,flags_of.get(str(p),()));objects.append(obj)
def link(exe,items,cwd):
 rsp=exe.with_suffix('.rsp');rsp.write_text(chr(10).join(chr(34)+x+chr(34) for x in ['/nologo','/OPT:REF','/map:'+str(exe.with_suffix('.map')),'/out:'+str(exe),*map(str,items),'kernel32.lib',str(W.WINMM)]))
 return subprocess.run([str(V.VC/'bin/link.exe'),'@'+str(rsp)],env=env,cwd=cwd,capture_output=True,text=True)
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
assert read_virtual_data(data,0x139652c,4)==struct.pack('<f',100.0) and 'float g_FableVertexReciprocalLimit_0139652C=100.0f;' in FIXTURE.read_text()
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
archive_info=create_font_archive(b/'fonts.big');text_archive_info=create_archive(b/'resource.big');ARCHIVE=str(b/'fonts.big');TEXT_ARCHIVE=str(b/'resource.big')
def patch_map(path,mapping,items=None):
 m=read_map(mapping);patches=[(site,target if kind==2 else m[alias.get(target[0],target[0])]+target[1],kind) for site,target,kind in (items or specs)]
 path.write_bytes(struct.pack('<I',len(patches))+b''.join(struct.pack('<III',*p) for p in patches));return patches
(b/'patches.json').write_text(json.dumps(patch_map(b/'patches.bin',b/'probe.map'),indent=2))
standalone=subprocess.run([str(exe),ARCHIVE,TEXT_ARCHIVE],capture_output=True,text=True,timeout=600)
standalone_record=dict(code=standalone.returncode,stdout=standalone.stdout,stderr=standalone.stderr)
(b/'standalone.json').write_text(json.dumps(standalone_record,indent=2));assert not standalone.returncode,standalone_record
x=subprocess.run([str(exe),ARCHIVE,TEXT_ARCHIVE,str(b/'original.bin'),str(b/'patches.bin')],capture_output=True,text=True,timeout=600)
positive=dict(code=x.returncode,stdout=x.stdout,stderr=x.stderr);(b/'run.json').write_text(json.dumps(positive,indent=2));print(positive,flush=True)
assert not standalone.returncode and 'UI_GAME_TEXT_FRAME_PASS cases=288 paths=288' in standalone.stdout
assert not x.returncode and 'UI_GAME_TEXT_FRAME_PASS cases=288 paths=576' in positive['stdout'],positive
# Controls: each fault is relinked with its own map and freshly bound retail patches.
out=b/'controls';out.mkdir();records=[]
kept_symbols=sorted({'/include:'+alias.get(target[0],target[0]) for site,target,kind in specs if kind!=2})
def run_fault(name,fault,block_path,patches):
 x=subprocess.run([str(fault),ARCHIVE,TEXT_ARCHIVE,str(block_path),str(patches)],cwd=out,capture_output=True,text=True,timeout=600)
 record=dict(name=name,code=x.returncode,stdout=x.stdout,stderr=x.stderr[-2000:]);assert x.returncode==80 and ('UI_TEXT_FAILURE' in x.stderr or 'UI_BANK_COLD' in x.stderr),record
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
 # Bank layer: the manager's buffer pool, font-entry creation, entry caching and the static handle open.
 ('manager-seven-buffers','CFontManager_CFontManager_009e2e30.cpp','for (unsigned i = 0; i < 8; ++i)','for (unsigned i = 0; i < 7; ++i)'),
 ('entry-added-index-swapped','CFontDataBank_NotifyEntryAdded_00a636c0.cpp','new CFontDataBankEntry(type, index, this)','new CFontDataBankEntry(type, index ^ 3, this)'),
 ('static-font-manager-unset','CFontManager_GetStaticFont_009e2ba0.cpp','font->PManager = this;',';'),
 # Text bank: the entry's string must survive the read and reach the queue.
 ('text-entry-string-dropped','NGameText_CTextEntry_ReadFromDataStream_009c8de0.cpp','Text=stream.ReadNullTerminatedWString();',';'),
]
for name,filename,old,new in mutants:
 i=index[filename];text=sources[i].read_text();assert text.count(old)==1,(name,text.count(old))
 path=out/(name+'.cpp');path.write_text(text.replace(old,new));obj=path.with_suffix('.obj');compile_unit(path,obj,out,filename)
 items=list(objects);items[i]=obj;reject(name,items)
for filename in ('NGameText_CDataBank_GetPTextEntry_009c9070.cpp','NGameText_CTextEntry_ReadFromDataStream_009c8de0.cpp','CFontManager_CFontManager_009e2e30.cpp','CFontManager_1CFontManager_009e2d00.cpp','CFontManager_GetStaticFont_009e2ba0.cpp','CFontDataBankEntry_AcquireFont_00a634f0.cpp','CFontDataBank_NotifyEntryAdded_00a636c0.cpp','CStreamingFontDataBank_NotifyEntryAdded_00a5f430.cpp','STLFontVertexBufferVector_FillInsert_009e3430.cpp','CBankFile_CreateSymbolMap_009cc530.cpp','CRenderManager2D_Render2DDrawList_009da9f0.cpp','CRenderManager2D_AddText_009db700.cpp','CRenderManager2D_Draw3DScaledText_009dbff0.cpp','CRenderStateManager_RestoreCaptureBlock_00a05840.cpp','CRenderManagerCore_AttachTextureToStage_009a0cf0.cpp','CStateBlockFunctions_Apply_009df060.cpp','CRenderManager2D_StateBlockLibrary_01396510.cpp','CRenderManagerCore_SetAWindow_00a0aa80.cpp','CQuickDrawTriInfoVector_Erase_009e15e0.cpp','STLQuickDrawInfo_InsertOverflow_009e1750.cpp','CRenderManager2D_Add2DTri_009dc030.cpp','CRenderManager2D_Add2DLine_009dc2c0.cpp','CRenderManager2D_Draw2DBoxWithZ_009ddc60.cpp','STLRender2DVertex_InsertOverflow_009e14b0.cpp','Global_ShaderRenderManager_01223d00.cpp','CShaderRenderManager_Initialise_0098acf0.cpp','CShaderRenderManager_EnableVertexShaders_00987fe0.cpp','CShaderRenderManager_ApplyVertexShader_00988020.cpp','CShaderRenderManager_SetVertexShaderConstantLayout_0098b5e0.cpp','CShaderRenderManager_UpdateCombinedProjectionTransform_00988a50.cpp','Global_2DShaderConstantLayout_01224140.cpp','Global_GEnableSIMD_013d2880.cpp','CStaticFontBank_Constructor_00ab8e10.cpp','CStaticFontBank_Lifetime_00ab7a70.cpp','CGlyph_Read_00ab96b0.cpp'):
 reject('missing-'+filename,[p for i,p in enumerate(objects) if i!=index[filename]],True)
# Base bank virtuals the font bank does not override are hot; NativeBankLifetimeColdAliases.h gives them a weak
# cold fallback, so their absence links and must fail at run time instead.
for filename in ('CBankFile_NotifyBankSize_009cbbf0.cpp','CBankFile_GetEnumerationType_009cbd10.cpp'):
 reject('runtime-missing-'+filename,[p for i,p in enumerate(objects) if i!=index[filename]])
# Retail-graph control: the transplanted Render2DDrawList skips its capture-block restore.
skip=bytearray(block);assert skip[restore_site]==0xe8;skip[restore_site:restore_site+5]=b'\x90'*5
(out/'original-no-restore.bin').write_bytes(skip);kept=[s for s in specs if s[0]!=restore_site+1];assert len(kept)==len(specs)-1
patch_map(out/'original-no-restore.patches.bin',b/'probe.map',kept);run_fault('original-list-without-capture-restore',exe,out/'original-no-restore.bin',out/'original-no-restore.patches.bin')
# Retail-graph control: the transplanted retail ApplyVertexShader returns at once, so the shader never binds.
skip=bytearray(block);at=SHADER_AT[0x988020];skip[at:at+3]=b'\xc2\x08\x00'
(out/'original-no-apply.bin').write_bytes(skip);patch_map(out/'original-no-apply.patches.bin',b/'probe.map');run_fault('original-retail-apply-vertex-shader-skipped',exe,out/'original-no-apply.bin',out/'original-no-apply.patches.bin')
again=subprocess.run([str(exe),ARCHIVE,TEXT_ARCHIVE,str(b/'original.bin'),str(b/'patches.bin')],capture_output=True,text=True,timeout=600)
assert not again.returncode and again.stdout==positive['stdout'],again
assert all(sha(p)==h for p,h in pins.items()),'input drift'
report=dict(status='PASS',cases=288,original_paths=576,text_archive=text_archive_info,repeat=True,controls=records,run=positive,standalone=standalone_record,dependencies=dependencies,build=str(b),input_sha256=pins,objects={str(p):sha(p) for p in objects},object_sources={str(o):str(p) for o,p in zip(objects,sources)},archive=archive_info,boundary='Game text: the actual NGameText::CDataBank opens a generated RESOURCE_TEST retail bank (text-lane schema fixture; not retail text) and loads eight text entries through GetPTextEntry/CTextEntry::ReadFromDataStream; their wide strings are queued by the public 2D API beside fixed strings and drawn with bank-loaded fonts (two extra frame shapes), then released and the bank closed with exact string balance. Fonts load from a generated BIGB font bank (FONT_TEST, schema 105, two type-0 entries; not a retail font asset) opened by the actual retail bank manager (OpenRetailBank) through the actual CFontManager static-handle path: CFontDataBank OpenReadOnly/ReadEntryList/NotifyBankSize/NotifyEntryAdded, CreateSymbolMap with the actual CSymbolMap, eight Win32 vertex buffers through the actual vector growth, GetStaticFont -> CFontDataBankEntry::AcquireFont -> actual CStaticFontBank constructor (glyph reader, DXT3 negotiation, texture creation and A8R8G8B8 upload through D3D/D3DX adapters); a repeat acquisition returns the cached font; the manager destructor releases every font, texture, buffer, bank and symbol map (char/wide string counts return exactly, except the shader-manager global constant names). Cold paths (disk files, string parser, dates, CRC, path helpers, error-message formatting) are fail-fast fixture boundaries. Frames as in the accepted UI text frame gate: public 2D text API enqueue and Render2DDrawList flush through real state blocks, capture restore, texture attach, static font Prepare/Render, the manager-owned Win32 vertex buffers, DrawCurrentBuffersAsTris and RealiseRenderState; retail Render2DDrawList/Render/Prepare/Draw/Realise + thirteen shader-manager functions transplanted and equal to the recreated graph in every event, payload byte and manager/shader byte. Shader resources, light pools and CDisplayManager::SetViewport stay boundaries; fake COM device; no graphics API, raster or game claim.')
(b/'receipt.json').write_text(json.dumps(report,indent=2))
if args.output is None:(R/'rebuild/compile-gate/native-ui-game-text-frame.json').write_text(json.dumps(report,indent=2))
print('NATIVE_UI_GAME_TEXT_FRAME_GATE_PASS cases=288 original_paths=576 controls='+str(len(records)),b,flush=True)
