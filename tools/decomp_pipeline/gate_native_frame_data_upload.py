import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
from gate_game_component_abi import Coff
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
w=Path(tempfile.mkdtemp(prefix='native-frame-data-upload-'));env=V.env()
specs=[('00/9f/CGraphicInfoFrame_CFrameData_UploadFrame_009ff220.cpp','?UploadFrame@CFrameData@CGraphicInfoFrame@@QAE_N_N@Z',0x9ff220,540,0,19),('00/9f/CResource_SetResourceSize_009fbf20.cpp','?SetResourceSize@CResource@@QAEXK@Z',0x9fbf20,46,576,0),('00/9f/CResourceBank_AddResource_009fc380.cpp','?AddResource@CResourceBank@@QAEXAAVCResource@@@Z',0x9fc380,52,640,0)]
volume='?LoadVolumeGraphic@CGraphicDataBank@@AAE_NAAVCManagedTexture@@AAVCBankFileAsyncData@@ABVCGraphicHeader@1@_NK3@Z';graphic='?LoadGraphicFrame@CGraphicDataBank@@AAE_NAAVCGraphicFrame@@AAVCManagedTexture@@AAVCBankFileAsyncData@@PAVCGraphicInfo@@_NKW4ETextureType@1@4@Z'
targets={symbol:address for rel,symbol,address,size,offset,count in specs};targets.update({'?GetEntryDataType@CBankFile@@QBEKK@Z':0x9cc210,volume:0x9fd9b0,graphic:0x9fed30,'??4CManagedTexture@@QAEABV0@ABV0@@Z':0xa69b20,'?Release@CManagedTexture@@QAE_NXZ':0xa69ad0,'?Uninitialise@CGraphicFrameData@@QAEXXZ':0x9fcba0,'??3@YAXPAX@Z':0xbfe9bc,'?Sizeof@CGraphicFrameData@@QBEKXZ':0x9fcbf0,'?Sizeof@CManagedTexture@@QBEKXZ':0xa69e60});offsets={address:offset for rel,symbol,address,size,offset,count in specs};offsets.update({address:0x80000000+i for i,address in enumerate((0x9cc210,0x9fd9b0,0x9fed30,0xa69b20,0xa69ad0,0x9fcba0,0xbfe9bc,0x9fcbf0,0xa69e60))})

objects=[];sources=[];proofs=[];coffs=[];body=bytearray(704);raw=EXE.read_bytes();cs=Cs(CS_ARCH_X86,CS_MODE_32);cs.detail=True;fixups=[];initFixups=[]
for rel,symbol,address,size,offset,count in specs:
 source=ROOT/'rebuild/src/compiled'/rel
 if not source.exists():source=ROOT/'work/parity_addtext/frame_data_upload_candidate_20261009.cpp'
 obj=w/f'{address:x}.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr
 objects.append(obj);sources.append(source);coffs.append(Coff(obj));proofs.append(check(obj,symbol,address,size,count,targets));b=read_virtual_data(raw,address,size);body[offset:offset+size]=b
 for ins in cs.disasm(b,address):
  if ins.mnemonic in ('call','jmp') and ins.bytes[0] in (0xe8,0xe9):
   target=int(ins.op_str,16)
   if not address<=target<address+size:assert target in offsets;fixups.append((offset+ins.address-address+1,offsets[target],20))
  if ins.mnemonic not in ('call','jmp') and ins.imm_size==4:
   for operand in ins.operands:
    if operand.type==2 and operand.imm in offsets:fixups.append((offset+ins.address-address+ins.imm_offset,offsets[operand.imm],6))
assert len(fixups)==19
(w/'frame_data_upload_retail.h').write_text('struct Fixup{unsigned site,target,kind;};\nstatic const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const Fixup fixups[]={'+','.join('{%u,%u,%u}'%f for f in fixups)+'};\n')
aliases=['#pragma comment(linker,"/alternatename:@UploadFrameData@12='+specs[0][1]+'")','#pragma comment(linker,"/alternatename:@VolumeBridge@32='+volume+'")','#pragma comment(linker,"/alternatename:@GraphicBridge@40='+graphic+'")']
(w/'frame_data_upload_aliases.h').write_text('\n'.join(aliases)+'\n')
fixture=ROOT/'rebuild/tests/integration/NativeFrameDataUpload_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/I'+str(ROOT/'rebuild/include'),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)]
for omit in range(3):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=120);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
assert 'cases=110592 snapshots=1258624' in r.stdout,(w,r.stdout)
headers=[ROOT/'rebuild/include'/p for p in ('fable_graphic_info.h','fable_graphic_info_frame.h','engine/CGraphicDataBank.h','engine/CManagedTextureOwner.h','fable_managed_texture.h','fable_managed_texture_data.h','engine/CGraphicFrame.h','fable_graphic_frame.h','fable_graphic_frame_data.h','fable_game_text_bank.h','fable_counted_pointer.h','fable_resource_list.h','fable_iv_counted_pointer.h','fable_base_class.h')]
report=dict(cases=110592,snapshots=int(r.stdout.split('snapshots=')[1]),providers=proofs,omission_checks=3,result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Complete540B UploadFrame with actualSetSize/AddResource,three mandatory sources. BankGetEntryType/LoadVolume/LoadGraphic/ManagedAssignment/ManagedRelease/GraphicFrameDataUninit/deferredSizes/countDelete/heap remain callbacks. Independent/native/compiled full16KiB callback/final snapshots plusbool. Volume/ordinary paths,success/failure,flat/nonflat,null/old/new/alternate frame data,rawcount/accountingwrap,signedmips,owner/info/file mutations,volume cachesowner/file/bank/level acrosssecond getter whileheader reloads,cached oldframe/source frame reload afterdeletion,temporary output FrameData mutated throughlive captured reference,localframe cleanup beforeFileData release,loadingpointer/owner/bank reload forsize/addResource,two placements/fills. Stack temporary address excluded from arena/logs;its initial/output FrameData and passed parameters compared as values,abort on nonzeroinitial;callback mutations only whiletemporary alive. No arbitrary stack normalization,actualbankdecode/textureupload not yet claimed.')
(ROOT/'rebuild/compile-gate/native-frame-data-upload.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_FRAME_DATA_UPLOAD_GATE_PASS')
