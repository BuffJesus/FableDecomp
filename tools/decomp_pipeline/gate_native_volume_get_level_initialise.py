import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
from gate_game_component_abi import Coff
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
w=Path(tempfile.mkdtemp(prefix='native-volume-get-level-initialise-'));env=V.env()
specs=[('00/9f/CVolume_Destructor_009fccf0.cpp','?GetVolumeLevel@CVolumeTexture@@QBE?AVCVolume@@J@Z',0xa67ea0,115,0,7),('00/9f/CVolume_Destructor_009fccf0.cpp','??1CVolume@@UAE@XZ',0x9fccf0,24,160,3),('00/a6/CVolume_CopyConstructor_00a673e0.cpp','??0CVolume@@QAE@ABV0@@Z',0xa673e0,40,208,2),('00/a6/CVolume_Uninitialise_00a67460.cpp','?Uninitialise@CVolume@@QAEX_N@Z',0xa67460,27,272,0),('00/99/CBaseClass_CBaseClass_0099a2d0.cpp','??0CBaseClass@@QAE@XZ',0x99a2d0,9,320,1),('00/99/CBaseClass_CBaseClass_0099a2d0.cpp','??1CBaseClass@@UAE@XZ',0x99a2e0,7,336,1),('00/a6/CVolume_InitialiseFromVolumeTextureMipmap_00a67770.cpp','?InitialiseFromVolumeTextureMipmap@CVolume@@QAEXPAUIDirect3DVolume9@@@Z',0xa67770,96,384,4),('00/a6/CVolume_GetByteLength_00a674e0.cpp','?GetByteLength@CVolume@@QBEKXZ',0xa674e0,77,512,2),('00/9e/CPixelFormat_Initialise_009e3830.cpp','?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z',0x9e3830,50,624,1),('00/9e/CPixelFormat_GetByteColourDepth_009e3800.cpp','?GetByteColourDepth@CPixelFormat@@QBEKXZ',0x9e3800,22,704,1)]
targets={symbol:address for rel,symbol,address,size,offset,count in specs};targets.update({'?TotalMemory@CVolume@@2KA':0x13d28dc,'?g_FableAllocatorMemoryBaseline_013CAAB0@@3KA':0x13caab0,'?GD3DFormats@@3QBVCD3DFormatInfo@@B':0x129ba40,'??_7CVolume@@6B@':0x129c840,'??_7CBaseClass@@6B@':0x1231710})
offsets={address:offset for rel,symbol,address,size,offset,count in specs};offsets.update({0x129c840:0x80000000,0x1231710:0x80000001,0xa67770:0x80000002,0x13d28dc:0x80000003,0x13caab0:0x80000004,0x129ba40:0x80000005,0x129ba48:0x80000006})

objects=[];sources=[];proofs=[];coffs=[];body=bytearray(768);raw=EXE.read_bytes();cs=Cs(CS_ARCH_X86,CS_MODE_32);cs.detail=True;fixups=[];initFixups=[];compiled={};originals={}
for rel,symbol,address,size,offset,count in specs:
 source=ROOT/'rebuild/src/compiled'/rel
 if rel=='00/9f/CVolume_Destructor_009fccf0.cpp' and 'CVolumeTexture::GetVolumeLevel' not in source.read_text():source=ROOT/'work/parity_addtext/volume_get_level_candidate_20261009.cpp'
 obj=compiled.get(source)
 if obj is None:
  obj=w/f'{address:x}.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr;originals[source]=obj
  if address==0xa67770:
   renamed=w/'initialise-observed.obj';run=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/DInitialiseFromVolumeTextureMipmap=ActualInitialiseFromVolumeTextureMipmap','/Fo'+str(renamed),str(source)],env,w);assert not run.returncode,run.stdout+run.stderr
   left,right=Coff(obj),Coff(renamed);ls,lo=left.symbols[symbol];rs,ro=right.symbols['?ActualInitialiseFromVolumeTextureMipmap@CVolume@@QAEXPAUIDirect3DVolume9@@@Z'];assert lo==ro and left.sections[ls]==right.sections[rs];obj=renamed
  compiled[source]=obj
  objects.append(obj);sources.append(source);coffs.append(Coff(obj))
 proofs.append(check(originals[source],symbol,address,size,count,targets));b=read_virtual_data(raw,address,size);body[offset:offset+size]=b
 for ins in cs.disasm(b,address):
  if ins.mnemonic in ('call','jmp') and ins.bytes[0] in (0xe8,0xe9):
   target=int(ins.op_str,16)
   if not address<=target<address+size:assert target in offsets;fixups.append((offset+ins.address-address+1,offsets[target],20))
  if ins.disp_size==4 and ins.disp in offsets:fixups.append((offset+ins.address-address+ins.disp_offset,offsets[ins.disp],6))
  if ins.mnemonic not in ('call','jmp') and ins.imm_size==4:
   for operand in ins.operands:
    if operand.type==2 and operand.imm in offsets:fixups.append((offset+ins.address-address+ins.imm_offset,offsets[operand.imm],6))
assert len(fixups)==22
from gate_render_state_static_data import coff_data
rootData=compiled[ROOT/'rebuild/src/compiled/00/a6/CVolume_InitialiseFromVolumeTextureMipmap_00a67770.cpp'];assert coff_data(Coff(rootData),'?TotalMemory@CVolume@@2KA',4)==read_virtual_data(raw,0x13d28dc,4)==bytes(4)
dataSource=ROOT/'rebuild/src/compiled/01/39/MemoryAllocatorData_01396868.cpp';dataObj=w/'data.obj';run=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(dataObj),str(dataSource)],env,w);assert not run.returncode,run.stdout+run.stderr;objects.append(dataObj);sources.append(dataSource);coffs.append(Coff(dataObj));assert coff_data(Coff(dataObj),'?g_FableAllocatorMemoryBaseline_013CAAB0@@3KA',4)==read_virtual_data(raw,0x13caab0,4)==bytes(4)
tableSource=ROOT/'rebuild/src/compiled/01/29/CPixelFormat_Table_0129ba40.cpp';tableObj=w/'table.obj';run=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(tableObj),str(tableSource)],env,w);assert not run.returncode,run.stdout+run.stderr;objects.append(tableObj);sources.append(tableSource);coffs.append(Coff(tableObj));table=read_virtual_data(raw,0x129ba40,47*36);assert coff_data(Coff(tableObj),'?GD3DFormats@@3QBVCD3DFormatInfo@@B',47*36)==table
import struct
(w/'volume_get_level_retail.h').write_text('struct Fixup{unsigned site,target,kind;};\nstatic const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const Fixup fixups[]={'+','.join('{%u,%u,%u}'%f for f in fixups)+'};\nstatic const unsigned retailFormats[]={'+','.join(map(str,struct.unpack('<423I',table)))+'};\n')
aliases=['#pragma comment(linker,"/alternatename:@ExecuteInit@12=?ActualInitialiseFromVolumeTextureMipmap@CVolume@@QAEXPAUIDirect3DVolume9@@@Z")','#pragma comment(linker,"/alternatename:?InitialiseFromVolumeTextureMipmap@CVolume@@QAEXPAUIDirect3DVolume9@@@Z=@ObserveInit@12")','#pragma comment(linker,"/alternatename:@VolumeGetLevel@16='+specs[0][1]+'")','#pragma comment(linker,"/alternatename:_VolumeVTable=??_7CVolume@@6B@")','#pragma comment(linker,"/alternatename:_BaseVTable=??_7CBaseClass@@6B@")']
defined={name for c in coffs for name in c.symbols};undefined={name for c in coffs for data,rels in c.sections.values() for name,kind in rels.values() if name not in defined and name not in ('?InitialiseFromVolumeTextureMipmap@CVolume@@QAEXPAUIDirect3DVolume9@@@Z','??2@YAPAXI@Z','??3@YAXPAX@Z','__purecall','??_7type_info@@6B@','?BeginLoading@CFrameData@CGraphicInfoFrame@@QAEXKK@Z')}
for name in sorted(undefined):aliases.append('#pragma comment(linker,"/alternatename:'+name+'=_AbortBoundary")')
(w/'volume_get_level_aliases.h').write_text('\n'.join(aliases)+'\n')
fixture=ROOT/'rebuild/tests/integration/NativeVolumeGetLevelInitialise_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/I'+str(ROOT/'rebuild/include'),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)]
for omit in range(10):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=90);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
assert 'cases=282624 snapshots=4310016' in r.stdout,(w,r.stdout)
headers=[ROOT/'rebuild/include'/p for p in ('fable_volume_types.h','fable_base_class.h','fable_d3d9.h','fable_memory_allocator.h','fable_d3d_format_info.h')]
report=dict(cases=282624,snapshots=int(r.stdout.split('snapshots=')[1]),providers=proofs,omission_checks=10,result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Ten mandatoryactualsources:115B VolumeGetLevel/shared24B VolumeDtor,40B VolumeCopy,27B Uninitialise,9B BaseCtor/7B BaseDtor,96B VolumeInitialise,77B ByteLength,50B PFInit,22B ByteDepth,1692B originaltable,existingallocatorcounterdata. Functionalboundaries onlyCOMGetLevel/AddRef/Release/GetDesc;Initialise observer capturesliveTemp thenexecutesindependent/native/compiled actual96B body,ownmethod-only rename provesidenticalcode/relocs. Independent/native/compiled full16KiB+two4B counters snapshots/hiddenoutreturn;46formats/rawdims/signedlevelbits/HRESULTignored,initAddRef/accounting4queries/copyAddRef/tempRelease,slot/output/countermutations,outaliasesTextureVT/PD,two placements/fills. No tempaddressrecord/masks orreadspastlifetime,noadditionalfunction/data/table coverage.')
(ROOT/'rebuild/compile-gate/native-volume-get-level-initialise.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_VOLUME_GET_LEVEL_INITIALISE_GATE_PASS')
