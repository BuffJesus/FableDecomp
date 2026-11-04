import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
from gate_game_component_abi import Coff
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
w=Path(tempfile.mkdtemp(prefix='native-sprite-loading-init-'));env=V.env()
specs=[('00/ba/CEngineInternalPrimitive2DSprite_Update_00bad8a0.cpp','?Update@CEngineInternalPrimitive2DSprite@@UAEXABVCEnginePrimitive@@K@Z',0xbad8a0,671,768,10),('00/9f/CTexture_GetDimensions_009f9db0.cpp','?GetDimensions@CTexture@@QBE?AVC2DExtentsI@@K@Z',0x9f9db0,43,1472,0),('00/9f/CGraphicInfoFrame_BeginLoading_009fe620.cpp','?BeginLoading@CGraphicInfoFrame@@QBEXKK@Z',0x9fe620,120,0,3),('00/9f/CGraphicInfoFrame_CFrameData_Constructor_009fe380.cpp','??0CFrameData@CGraphicInfoFrame@@QAE@PBV1@@Z',0x9fe380,181,256,10),('00/9f/CResourceBank_AddResource_009fc380.cpp','?AddResource@CResourceBank@@QAEXAAVCResource@@@Z',0x9fc380,52,512,0),('00/9f/CResource_SetResourceSize_009fbf20.cpp','?SetResourceSize@CResource@@QAEXK@Z',0x9fbf20,46,576,0),('00/9f/CResourceBank_Lifetime_009fc5f0.cpp','??0CResource@@QAE@XZ',0x9fbec0,33,640,1),('00/99/CBaseClass_CBaseClass_0099a2d0.cpp','??0CBaseClass@@QAE@XZ',0x99a2d0,9,688,1)]
targets={symbol:address for rel,symbol,address,size,offset,count in specs}
targets.update({'?DELTA_NOTIONAL_ZERO@@3MB':0x129ba3c,'?GPResourceManager@NEngineInternal@@3PAVCEngineResourceManager@@A':0x1436e98,'__real@4f800000':0x122dcb4,'??_7CFrameData@CGraphicInfoFrame@@6B@':0x129c89c,'??_7CManagedTexture@@6B@':0x129c880,'??_7CResource@@6B@':0x12354a4,'??_7CBaseClass@@6B@':0x1231710,'?Sizeof@CGraphicFrameData@@QBEKXZ':0x9fcbf0,'?Sizeof@CManagedTexture@@QBEKXZ':0xa69e60,'??2@YAPAXI@Z':0xbfea1a,'?BeginLoading@CFrameData@CGraphicInfoFrame@@QAEXKK@Z':0x9fde80})
offsets={address:offset for rel,symbol,address,size,offset,count in specs};offsets.update({0x129c89c:0x80000000,0x129c880:0x80000001,0x12354a4:0x80000002,0x1231710:0x80000003,0xbfea1a:0x80000004,0x9fde80:0x80000005,0x9fcbf0:0x80000006,0xa69e60:0x80000007,0x129ba3c:0x80000008,0x1436e98:0x80000009,0x122dcb4:0x8000000a})
objects=[];sources=[];proofs=[];coffs=[];body=bytearray(1536);raw=EXE.read_bytes();cs=Cs(CS_ARCH_X86,CS_MODE_32);cs.detail=True;fixups=[];initFixups=[]
for rel,symbol,address,size,offset,count in specs:
 source=ROOT/'rebuild/src/compiled'/rel;obj=w/f'{address:x}.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr
 objects.append(obj);sources.append(source);coffs.append(Coff(obj));proofs.append(check(obj,symbol,address,size,count,targets));b=read_virtual_data(raw,address,size);body[offset:offset+size]=b
 for ins in cs.disasm(b,address):
  if ins.mnemonic in ('call','jmp') and ins.bytes[0] in (0xe8,0xe9):
   target=int(ins.op_str,16)
   if not address<=target<address+size:assert target in offsets;fixups.append((offset+ins.address-address+1,offsets[target],20))
  if ins.disp_size==4 and ins.disp in offsets:fixups.append((offset+ins.address-address+ins.disp_offset,offsets[ins.disp],6))
  if ins.mnemonic not in ('call','jmp') and ins.imm_size==4:
   for operand in ins.operands:
    if operand.type==2 and operand.imm in offsets:fixups.append((offset+ins.address-address+ins.imm_offset,offsets[operand.imm],6))
assert len(fixups)==25
(w/'sprite_loading_init_retail.h').write_text('struct Fixup{unsigned site,target,kind;};\nstatic const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const Fixup fixups[]={'+','.join('{%u,%u,%u}'%f for f in fixups)+'};\n')
aliases=['#pragma comment(linker,"/alternatename:@InternalSpriteUpdate@16='+specs[0][1]+'")','#pragma comment(linker,"/alternatename:?BeginLoading@CFrameData@CGraphicInfoFrame@@QAEXKK@Z=?Load@Boundaries@@QAEXKK@Z")']
for local,symbol in [('FrameVTable','??_7CFrameData@CGraphicInfoFrame@@6B@'),('ManagedVTable','??_7CManagedTexture@@6B@'),('ResourceVTable','??_7CResource@@6B@'),('BaseVTable','??_7CBaseClass@@6B@')]:aliases.append('#pragma comment(linker,"/alternatename:_'+local+'='+symbol+'")')
defined={name for c in coffs for name in c.symbols};undefined={name for c in coffs for data,rels in c.sections.values() for name,kind in rels.values() if name not in defined and name not in ('?DELTA_NOTIONAL_ZERO@@3MB','?GPResourceManager@NEngineInternal@@3PAVCEngineResourceManager@@A','__real@4f800000','??2@YAPAXI@Z','??3@YAXPAX@Z','__purecall','??_7type_info@@6B@','?BeginLoading@CFrameData@CGraphicInfoFrame@@QAEXKK@Z')}
for name in sorted(undefined):aliases.append('#pragma comment(linker,"/alternatename:'+name+'=_AbortBoundary")')
(w/'sprite_loading_init_aliases.h').write_text('\n'.join(aliases)+'\n')
for j,rel in enumerate(('01/29/Fable_DeltaNotionalZero_0129ba3c.cpp','01/43/NEngineInternal_GPResourceManager_01436e98.cpp')):
 source=ROOT/'rebuild/src/compiled'/rel;obj=w/f'global{j}.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj);sources.append(source)
fixture=ROOT/'rebuild/tests/integration/NativeSpriteLoadingInit_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/I'+str(ROOT/'rebuild/include'),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)]
for omit in range(10):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
assert 'cases=24576 snapshots=122112' in r.stdout,(w,r.stdout)
headers=[ROOT/'rebuild/include'/p for p in ('fable_graphic_info.h','fable_graphic_info_frame.h','engine/CGraphicDataBank.h','engine/CManagedTextureOwner.h','fable_managed_texture.h','fable_managed_texture_data.h','engine/CGraphicFrame.h','fable_graphic_frame.h','fable_graphic_frame_data.h','fable_game_text_bank.h','fable_counted_pointer.h','fable_resource_list.h','fable_iv_counted_pointer.h','fable_base_class.h')]
report=dict(cases=24576,snapshots=int(r.stdout.split('snapshots=')[1]),providers=proofs,omission_checks=10,result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual internal sprite Update, texture dimensions, frame loader, frame-data/resource/base constructors, size accounting and bank registration; ten mandatory sources including DELTA and resource-manager globals. Allocation, nested CFrameData::BeginLoading and COMGetLevelDesc remain callbacks. Independent full64KiB/global snapshots; native/compiled all bytes equal, independent model classifies only clip NaNs. Valid bank/new frame data and direct textures; source aliases, callback input/frame/global mutations, raw floats and dimensions, threshold neighbors, three x87 precisions, two placements/fills. All null-frame callback-only cases excluded. Abort-only unused size/virtual/destructor aliases must never execute.')
(ROOT/'rebuild/compile-gate/native-sprite-loading-init.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_SPRITE_LOADING_INIT_GATE_PASS')
