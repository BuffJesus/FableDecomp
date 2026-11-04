"""Font draw composition: actual render-state Init, PrepareToRender, Render, Win32 vertex buffers,
DrawCurrentBuffersAsTris and RealiseRenderState against a recording fake device, in the retail
Render2DDrawList order, compared with the transplanted retail Render/Prepare/Draw/Realise graph."""
from pathlib import Path
from datetime import datetime, timezone
import sys, json, hashlib, re, subprocess, struct
import argparse
R=Path(__file__).resolve().parents[2]
parser=argparse.ArgumentParser();parser.add_argument('--output',type=Path);args=parser.parse_args()
b=args.output.resolve() if args.output else R/'rebuild/build/native-static-font-draw'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');b.mkdir(parents=True)
sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import check_one as V
from pe_oracle import EXE, read_virtual_data
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from gate_game_component_abi import Coff
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
]
sources=[R/'rebuild/src/compiled/00/ab/CStaticFontBank_Render_00ab7c20.cpp']+[R/'rebuild/src/compiled'/p for p in helpers]+[R/'rebuild/tests/integration/NativeStaticFontDraw_test.cpp']
index={p.name:i for i,p in enumerate(sources)}
inc=b/'include';inc.mkdir();inputs={EXE,Path(__file__)};seen=set()
# Dependencies: every composed root keeps the source its own accepted gate qualified.
dependencies={}
for name in ('native-static-font-render-buffer.json','native-static-font-prepare.json'):
 path=R/'rebuild/compile-gate'/name;receipt=json.loads(path.read_text());assert receipt['status']=='PASS' and receipt['repeat'];inputs.add(path)
 for p,h in receipt['input_sha256'].items():
  if Path(p).suffix in ('.h','.cpp','.py'):assert sha(p)==h,p;inputs.add(Path(p))
 dependencies[name]=receipt['build']
for name,source in (('native-textured-buffers.json',None),('render-state-init.json','CRenderStateManager_Init_00a07900.cpp'),('render-state-realise.json','CRenderStateManager_RealiseRenderState_00a058c0.cpp'),('render-state-setup.json','CRenderStateManager_StateSetup_00a04580.cpp'),('render-state-defaults.json','CRenderStateManager_SetDefaultRenderStates_00a059e0.cpp'),('render-state-resend.json','CRenderStateManager_ForceResendRenderStatesToHardware_00a044e0.cpp')):
 path=R/'rebuild/compile-gate'/name;receipt=json.loads(path.read_text());inputs.add(path);pinned=receipt['source_sha256']
 for p,h in (pinned.items() if isinstance(pinned,dict) else [(sources[index[source]],pinned)]):
  p=R/p;assert sha(p)==h,p
 dependencies[name]=pinned
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
def compile_unit(source,obj,cwd):
 x=V.cl(['/nologo','/c','/O2','/MT','/Gy','/I'+str(inc),'/Fo'+str(obj),str(source)],env,cwd);assert not x.returncode,(source.name,x.stdout+x.stderr);return x
for i,p in enumerate(sources):
 snapshot=b/('unit'+str(i)+'.cpp');snapshot.write_bytes(p.read_bytes());obj=snapshot.with_suffix('.obj');compile_unit(snapshot,obj,b);objects.append(obj)
def link(exe,items,cwd):
 return subprocess.run([str(V.VC/'bin/link.exe'),'/nologo','/OPT:REF','/map:'+str(exe.with_suffix('.map')),'/out:'+str(exe),*map(str,items)],env=env,cwd=cwd,capture_output=True,text=True)
exe=b/'probe.exe';x=link(exe,objects,b);(b/'link.json').write_text(json.dumps(dict(code=x.returncode,stdout=x.stdout,stderr=x.stderr)));assert not x.returncode,x.stdout+x.stderr
def read_map(path):
 mapped={}
 for line in path.read_text().splitlines():
  m=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
  if m:mapped[m[1]]=int(m[2],16)
 return mapped
# Fresh composed roots equal the objects their accepted gates qualified.
def same_symbol(receipt_name,filename,symbol):
 receipt=json.loads((R/'rebuild/compile-gate'/receipt_name).read_text())
 reference=next(Path(p) for p,s in receipt['object_sources'].items() if Path(s).name==filename);assert sha(reference)==receipt['objects'][str(reference)]
 fresh=Coff(objects[index[filename]]);accepted=Coff(reference);fs,fo=fresh.symbols[symbol];rs,ro=accepted.symbols[symbol];assert fo==ro==0 and fresh.sections[fs]==accepted.sections[rs],symbol
same_symbol('native-static-font-render-buffer.json','CStaticFontBank_Render_00ab7c20.cpp','?Render@CStaticFontBank@@UBEXABVCWideString@@ABVC3DVector@@MABVCRGBColour@@KPBVCTexture@@@Z')
same_symbol('native-static-font-prepare.json','CStaticFontBank_PrepareToRender_00ab83a0.cpp','?PrepareToRender@CStaticFontBank@@UBEX_N@Z')
# One transplanted retail block: Render, Prepare, Draw, Realise; internal calls stay inside the block.
RENDER_AT,PREPARE_AT,DRAW_AT,REALISE_AT,BLOCK=0,2048,4864,5120,5287
data=EXE.read_bytes();block=bytearray(BLOCK);specs=[];md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
def place(address,size,at,tail):
 code=read_virtual_data(data,address,size);assert code[-len(tail):]==tail,hex(address);block[at:at+size]=code;return code
def absolute(i):
 return any((o.type==2 and 0x400000<=o.imm&0xffffffff<0x1600000) or (o.type==3 and o.mem.base==0 and o.mem.index==0 and 0x400000<=o.mem.disp&0xffffffff<0x1600000) for o in i.operands)
code=place(0xab7c20,1910,RENDER_AT,b'\xc2\x18\0')
targets={0x9a4ec0:'?GFGetSystemManager@@YIPAVCSystemManager@@XZ',0x9e2ce0:'?GetFreeVertexBuffer@CFontManager@@QAEAAVCVertexBufferWin32@@XZ',0x99af00:'??BCWideString@@QBEPBGXZ',0x99af10:'?GetAsCharArray@CWideString@@QBEPBGXZ',0xbfeb8a:'_floor',0xac20a0:'?Lock@CVertexBufferBase@@QAEPAEKKK@Z',0xac20b0:'?Unlock@CVertexBufferBase@@QAEXXZ'}
constants={0x122f59c:'_FontHalf',0x122ded8:'_FontOne',0x122dcb4:'_FontUnsignedBias'}
for address,name in constants.items():assert read_virtual_data(data,address,4)==struct.pack('<f',{'_FontHalf':.5,'_FontOne':1.,'_FontUnsignedBias':4294967296.}[name])
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
code=place(0xa0abe0,127,DRAW_AT,b'\xc2\x10\0');calls=0
for i in md.disasm(code,0xa0abe0):
 if i.mnemonic=='call' and i.operands[0].type==2:assert i.operands[0].imm==0xa058c0;specs.append((DRAW_AT+i.address-0xa0abe0+i.imm_offset,REALISE_AT,2));calls+=1
 elif not i.mnemonic.startswith('j'):assert not absolute(i),i
assert calls==1
code=place(0xa058c0,167,REALISE_AT,b'\xc3')
for i in md.disasm(code,0xa058c0):
 assert not (i.mnemonic=='call' and i.operands[0].type==2),i
 assert i.mnemonic.startswith('j') or not absolute(i),i
(b/'original.bin').write_bytes(block)
def patch_map(path,mapping):
 m=read_map(mapping);patches=[(site,target if kind==2 else m[target[0]]+target[1],kind) for site,target,kind in specs]
 path.write_bytes(struct.pack('<I',len(patches))+b''.join(struct.pack('<III',*p) for p in patches));return patches
(b/'patches.json').write_text(json.dumps(patch_map(b/'patches.bin',b/'probe.map'),indent=2))
standalone=subprocess.run([str(exe)],capture_output=True,text=True,timeout=300)
standalone_record=dict(code=standalone.returncode,stdout=standalone.stdout,stderr=standalone.stderr)
(b/'standalone.json').write_text(json.dumps(standalone_record,indent=2));assert not standalone.returncode,standalone_record
x=subprocess.run([str(exe),str(b/'original.bin'),str(b/'patches.bin')],capture_output=True,text=True,timeout=300)
positive=dict(code=x.returncode,stdout=x.stdout,stderr=x.stderr);(b/'run.json').write_text(json.dumps(positive,indent=2));print(positive,flush=True)
assert not x.returncode,positive
m=re.search(r'STATIC_FONT_DRAW_PASS cases=(\d+) paths=(\d+) init_states=(\d+),(\d+)',positive['stdout']);assert m and m[1]=='21600' and m[2]=='43200',positive
# Controls: each fault is relinked with its own map and freshly bound retail patches. Init, setup and
# defaults run recreated in both modes, so only their device-visible effects are controls here; their
# stored values (bookmark mask, default values) are compared with retail by their own gates.
out=b/'controls';out.mkdir();records=[]
kept_symbols=sorted({'/include:'+target[0] for site,target,kind in specs if kind!=2})
def reject(name,items,missing=False):
 fault=out/('fault'+str(len(records))+'.exe');x=link(fault,[*kept_symbols,*items],out)
 if missing:
  assert x.returncode,(name,x.stdout,x.stderr);record=dict(name=name,link_rejected=True,stdout=x.stdout,stderr=x.stderr)
 else:
  assert not x.returncode,x.stdout+x.stderr
  patches=fault.with_suffix('.patches.bin');patch_map(patches,fault.with_suffix('.map'))
  x=subprocess.run([str(fault),str(b/'original.bin'),str(patches)],cwd=out,capture_output=True,text=True,timeout=300)
  record=dict(name=name,code=x.returncode,stdout=x.stdout,stderr=x.stderr)
  assert x.returncode==80 and 'FONT_DRAW_FAILURE' in x.stderr,record
 records.append(record);(out/'progress.json').write_text(json.dumps(records,indent=2));print(name,'rejected',flush=True)
mutants=[
 ('draw-texture-cache','CRenderManagerCore_DrawCurrentBuffersAsTrisTextures_00a0abe0.cpp','if(CurrentlySelectedTextures[stage]!=texture)','if(true)'),
 ('draw-cache-update','CRenderManagerCore_DrawCurrentBuffersAsTrisTextures_00a0abe0.cpp','CurrentlySelectedTextures[stage]=texture;',';'),
 ('draw-no-realise','CRenderManagerCore_DrawCurrentBuffersAsTrisTextures_00a0abe0.cpp','RealiseRenderState();',';'),
 ('draw-stage-count','CRenderManagerCore_DrawCurrentBuffersAsTrisTextures_00a0abe0.cpp','TextureStagesInUse=stage+1;','TextureStagesInUse=stage;'),
 ('draw-primitive-type','CRenderManagerCore_DrawCurrentBuffersAsTrisTextures_00a0abe0.cpp','DrawPrimitive(PT_TRIANGLE_LIST,','DrawPrimitive((EPrimitiveType)5,'),
 ('realise-unchanged-states','CRenderStateManager_RealiseRenderState_00a058c0.cpp','if (desired != state->CurrentState)','if (true)'),
 ('realise-current-kept','CRenderStateManager_RealiseRenderState_00a058c0.cpp','state->CurrentState = state->DesiredState;',';'),
 ('realise-dirty-flag-kept','CRenderStateManager_RealiseRenderState_00a058c0.cpp','state->DirtyListFlag = 0;',';'),
 ('realise-list-kept','CRenderStateManager_RealiseRenderState_00a058c0.cpp','StateUpdateListSize = 0;',';'),
 ('init-no-resend','CRenderStateManager_Init_00a07900.cpp','ForceResendRenderStatesToHardware();',';'),
 ('resend-current-kept','CRenderStateManager_ForceResendRenderStatesToHardware_00a044e0.cpp','state->CurrentState=desired;',';'),
 ('setup-lod-caps','CRenderStateManager_StateSetup_00a04580.cpp','if(caps.RasterCaps & 0x2000)','if(caps.RasterCaps & 0x4000)'),
 ('prepare-blend-disabled','CStaticFontBank_PrepareToRender_00ab83a0.cpp','SetRenderState(1,render->DeviceState.AlphaBlendEnable)','SetRenderState(0,render->DeviceState.AlphaBlendEnable)'),
 ('prepare-alpha-reference','CStaticFontBank_PrepareToRender_00ab83a0.cpp','SetRenderState(8,render->DeviceState.AlphaRef)','SetRenderState(7,render->DeviceState.AlphaRef)'),
 ('prepare-z-enable','CStaticFontBank_PrepareToRender_00ab83a0.cpp','SetRenderState(1,render->DeviceState.ZBufferEnable)','SetRenderState(0,render->DeviceState.ZBufferEnable)'),
 ('prepare-fog-enable','CStaticFontBank_PrepareToRender_00ab83a0.cpp','SetRenderState(0,render->DeviceState.FogEnable)','SetRenderState(1,render->DeviceState.FogEnable)'),
 ('prepare-never-format','CStaticFontBank_PrepareToRender_00ab83a0.cpp','if(setupVertexFormat){','if(false){'),
 ('render-zero-textures','CStaticFontBank_Render_00ab7c20.cpp','unsigned long textureCount=texture?2:1;','unsigned long textureCount=1;'),
 ('render-wrong-owned-texture','CStaticFontBank_Render_00ab7c20.cpp','const CTexture* textures[2]={PTexture,texture};','const CTexture* textures[2]={0,texture};'),
 ('render-final-empty-draw','CStaticFontBank_Render_00ab7c20.cpp','    if(triangles){','    if(true){'),
]
for name,filename,old,new in mutants:
 i=index[filename];text=sources[i].read_text();assert text.count(old)==1,(name,text.count(old))
 path=out/(name+'.cpp');path.write_text(text.replace(old,new));obj=path.with_suffix('.obj');compile_unit(path,obj,out)
 items=list(objects);items[i]=obj;reject(name,items)
for filename in ('CStaticFontBank_PrepareToRender_00ab83a0.cpp','CRenderManagerCore_DrawCurrentBuffersAsTrisTextures_00a0abe0.cpp','CRenderStateManager_RealiseRenderState_00a058c0.cpp','CRenderStateManager_Init_00a07900.cpp','CRenderStateManager_StateSetup_00a04580.cpp','CRenderStateManager_SetDefaultRenderStates_00a059e0.cpp','CRenderStateManager_ForceResendRenderStatesToHardware_00a044e0.cpp','CRenderManagerCore_SetVertexFormat_00a0aa30.cpp'):
 reject('missing-'+filename,[p for i,p in enumerate(objects) if i!=index[filename]],True)
# Retail-graph control: the transplanted draw skips its realise call, so queued states never reach the device.
skip=bytearray(block);site=next(s for s,t,k in specs if k==2 and t==REALISE_AT);assert skip[site-1]==0xe8;skip[site-1:site+4]=b'\x90'*5
(out/'original-no-realise.bin').write_bytes(skip);nospecs=out/'original-no-realise.patches.bin';kept=[p for p in json.loads((b/'patches.json').read_text()) if p[0]!=site]
nospecs.write_bytes(struct.pack('<I',len(kept))+b''.join(struct.pack('<III',*p) for p in kept))
x=subprocess.run([str(exe),str(out/'original-no-realise.bin'),str(nospecs)],cwd=out,capture_output=True,text=True,timeout=300)
assert x.returncode==80 and 'FONT_DRAW_FAILURE' in x.stderr,x;records.append(dict(name='original-draw-without-realise',code=x.returncode,stdout=x.stdout,stderr=x.stderr))
again=subprocess.run([str(exe),str(b/'original.bin'),str(b/'patches.bin')],capture_output=True,text=True,timeout=300)
assert not again.returncode and again.stdout==positive['stdout'],again
assert all(sha(p)==h for p,h in pins.items()),'input drift'
report=dict(status='PASS',cases=21600,original_paths=43200,init_states=[int(m[3]),int(m[4])],repeat=True,controls=records,run=positive,standalone=standalone_record,dependencies=dependencies,build=str(b),input_sha256=pins,objects={str(p):sha(p) for p in objects},object_sources={str(o):str(p) for o,p in zip(objects,sources)},boundary='Retail Render2DDrawList order: actual CRenderStateManager::Init (setup/defaults/resend under two RasterCaps profiles), PrepareToRender, Render, Win32 vertex buffers, DrawCurrentBuffersAsTris with texture cache and RealiseRenderState feed a recording fake device; candidate graph equals the transplanted retail Render/Prepare/Draw/Realise graph in every device call, vertex payload and manager byte. CRenderManagerCore::Init (system font, caps copy, back-buffer format) is replaced by explicit device/stage/texture-cache setup. Fake COM device/resources, borrowed font storage. No graphics API, rasterization or game certification.')
(b/'receipt.json').write_text(json.dumps(report,indent=2))
if args.output is None:(R/'rebuild/compile-gate/native-static-font-draw.json').write_text(json.dumps(report,indent=2))
print('NATIVE_STATIC_FONT_DRAW_GATE_PASS cases=21600 original_paths=43200 controls='+str(len(records)),b,flush=True)
