import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
from gate_game_component_abi import Coff
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
w=Path(tempfile.mkdtemp(prefix='native-frame-data-free-texture-'));env=V.env()
specs=[('00/9f/CGraphicInfoFrame_CFrameData_FreeTextureMemory_009fe460.cpp','?FreeTextureMemory@CFrameData@CGraphicInfoFrame@@QAE_N_N@Z',0x9fe460,231,0,10),('00/9f/CResource_GetUnusedFrames_009fbf70.cpp','?GetUnusedFrames@CResource@@QBEKXZ',0x9fbf70,18,256,0),('00/9f/CGraphicInfoFrame_CFrameData_Sizeof_009fdbe0.cpp','?Sizeof@CFrameData@CGraphicInfoFrame@@QBEKXZ',0x9fdbe0,51,288,2),('00/9f/CResource_SetResourceSize_009fbf20.cpp','?SetResourceSize@CResource@@QAEXK@Z',0x9fbf20,46,352,0),('00/a0/CGraphicInfoFrame_ReleaseData_00a00680.cpp','?ReleaseData@CGraphicInfoFrame@@ABEXXZ',0xa00680,36,416,0),('00/9f/CGraphicInfoFrame_CFrameData_ResetTexture_009fe440.cpp','?ResetTexture@CFrameData@CGraphicInfoFrame@@QAEXXZ',0x9fe440,29,464,2)]
targets={symbol:address for rel,symbol,address,size,offset,count in specs};targets.update({'?ReduceMipmapLevel@CTextureManager@@QAE_NAAVCGraphicFrame@@AAVCManagedTexture@@K@Z':0xa69f70,'?Sizeof@CGraphicFrameData@@QBEKXZ':0x9fcbf0,'?Sizeof@CManagedTexture@@QBEKXZ':0xa69e60,'?Release@CManagedTexture@@QAE_NXZ':0xa69ad0,'?UnInitialise@CGraphicFrame@@QAEXXZ':0xa00b90});offsets={address:offset for rel,symbol,address,size,offset,count in specs};offsets.update({0xa69f70:0x80000000,0x9fcbf0:0x80000001,0xa69e60:0x80000002,0xa69ad0:0x80000003,0xa00b90:0x80000004})

objects=[];sources=[];proofs=[];coffs=[];body=bytearray(512);raw=EXE.read_bytes();cs=Cs(CS_ARCH_X86,CS_MODE_32);cs.detail=True;fixups=[];initFixups=[]
for rel,symbol,address,size,offset,count in specs:
 source=ROOT/'rebuild/src/compiled'/rel
 if not source.exists():source=ROOT/'work/parity_addtext/frame_data_free_texture_candidate_20261009.cpp'
 obj=w/f'{address:x}.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr
 objects.append(obj);sources.append(source);coffs.append(Coff(obj));proofs.append(check(obj,symbol,address,size,count,targets));b=read_virtual_data(raw,address,size);body[offset:offset+size]=b
 for ins in cs.disasm(b,address):
  if ins.mnemonic in ('call','jmp') and ins.bytes[0] in (0xe8,0xe9):
   target=int(ins.op_str,16)
   if not address<=target<address+size:assert target in offsets;fixups.append((offset+ins.address-address+1,offsets[target],20))
  if ins.mnemonic not in ('call','jmp') and ins.imm_size==4:
   for operand in ins.operands:
    if operand.type==2 and operand.imm in offsets:fixups.append((offset+ins.address-address+ins.imm_offset,offsets[operand.imm],6))
assert len(fixups)==14
(w/'frame_data_free_texture_retail.h').write_text('struct Fixup{unsigned site,target,kind;};\nstatic const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const Fixup fixups[]={'+','.join('{%u,%u,%u}'%f for f in fixups)+'};\n')
aliases=['#pragma comment(linker,"/alternatename:@FreeFrameTexture@12='+specs[0][1]+'")']
(w/'frame_data_free_texture_aliases.h').write_text('\n'.join(aliases)+'\n')
fixture=ROOT/'rebuild/tests/integration/NativeFrameDataFreeTexture_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/I'+str(ROOT/'rebuild/include'),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)]
for omit in range(6):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=120);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
assert 'cases=55296 snapshots=201304' in r.stdout,(w,r.stdout)
headers=[ROOT/'rebuild/include'/p for p in ('fable_graphic_info.h','fable_graphic_info_frame.h','engine/CGraphicDataBank.h','engine/CManagedTextureOwner.h','fable_managed_texture.h','fable_managed_texture_data.h','engine/CGraphicFrame.h','fable_graphic_frame.h','fable_graphic_frame_data.h','fable_game_text_bank.h','fable_counted_pointer.h','fable_resource_list.h','fable_iv_counted_pointer.h','fable_base_class.h','engine/CTextureManager.h','engine/CPreallocTexturePool.h')]
report=dict(cases=55296,snapshots=int(r.stdout.split('snapshots=')[1]),providers=proofs,omission_checks=6,result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Complete231B nested FreeTextureMemory with actual GetUnusedFrames, FrameDataSizeof, SetSize, FrameReleaseData and ResetTexture, six mandatory providers. ReduceMipmapLevel/GraphicFrameDataSizeof/ManagedSizeof/ManagedRelease/FrameUnInitialise/virtualDelete callbacks remain; deferred size leaves are not recovered. Independent/native/compiled full16KiB snapshots and bool;null/live frame, unsigned ages/delays/counter wrap,signed mip boundaries/unsigned compare,flat/discard branches,reduction false result ignored,cached next mip despite callback writes,postreset owner reload,owner-slot caching through deletion,loading pointer reload and resource accounting,two placements/fills. Frame-live implies valid managed texture/owner/info/bank; no unsafe-null claim.')
(ROOT/'rebuild/compile-gate/native-frame-data-free-texture.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_FRAME_DATA_FREE_TEXTURE_GATE_PASS')
