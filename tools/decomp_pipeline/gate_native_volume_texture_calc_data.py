import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
from gate_game_component_abi import Coff
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
w=Path(tempfile.mkdtemp(prefix='native-volume-texture-calc-data-'));env=V.env()
specs=[('00/a6/CVolumeTexture_CalcByteLength_00a67ff0.cpp', '?CalcByteLength@CVolumeTexture@@IAEXXZ', 10911728, 101, 0, 6), ('00/9f/CVolume_Destructor_009fccf0.cpp', '?GetVolumeLevel@CVolumeTexture@@QBE?AVCVolume@@J@Z', 10911392, 115, 128, 7), ('00/9f/CVolume_Destructor_009fccf0.cpp', '??1CVolume@@UAE@XZ', 10472688, 24, 256, 3), ('00/a6/CVolume_CopyConstructor_00a673e0.cpp', '??0CVolume@@QAE@ABV0@@Z', 10908640, 40, 304, 2), ('00/a6/CVolume_Uninitialise_00a67460.cpp', '?Uninitialise@CVolume@@QAEX_N@Z', 10908768, 27, 368, 0), ('00/99/CBaseClass_CBaseClass_0099a2d0.cpp', '??0CBaseClass@@QAE@XZ', 10068688, 9, 416, 1), ('00/99/CBaseClass_CBaseClass_0099a2d0.cpp', '??1CBaseClass@@UAE@XZ', 10068704, 7, 432, 1), ('00/a6/CVolume_InitialiseFromVolumeTextureMipmap_00a67770.cpp', '?InitialiseFromVolumeTextureMipmap@CVolume@@QAEXPAUIDirect3DVolume9@@@Z', 10909552, 96, 480, 4), ('00/a6/CVolume_GetByteLength_00a674e0.cpp', '?GetByteLength@CVolume@@QBEKXZ', 10908896, 77, 608, 2), ('00/9e/CPixelFormat_Initialise_009e3830.cpp', '?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z', 10369072, 50, 704, 1), ('00/9e/CPixelFormat_GetByteColourDepth_009e3800.cpp', '?GetByteColourDepth@CPixelFormat@@QBEKXZ', 10369024, 22, 768, 1), ('00/a6/CVolumeTexture_GetNoLevels_00a67e90.cpp', '?GetNoLevels@CVolumeTexture@@QBEIXZ', 10911376, 10, 800, 0)]
targets={symbol:address for rel,symbol,address,size,offset,count in specs};targets.update({'?TotalMemory@CVolume@@2KA':0x13d28dc,'?g_FableAllocatorMemoryBaseline_013CAAB0@@3KA':0x13caab0,'?GD3DFormats@@3QBVCD3DFormatInfo@@B':0x129ba40,'??_7CVolume@@6B@':0x129c840,'??_7CBaseClass@@6B@':0x1231710})
offsets={address:offset for rel,symbol,address,size,offset,count in specs};offsets.update({0x129c840:0x80000000,0x1231710:0x80000001,0xa67770:0x80000002,0x13d28dc:0x80000003,0x13caab0:0x80000004,0x129ba40:0x80000005,0x129ba48:0x80000006,0xa67ea0:0x80000007})

objects=[];sources=[];proofs=[];coffs=[];body=bytearray(832);raw=EXE.read_bytes();cs=Cs(CS_ARCH_X86,CS_MODE_32);cs.detail=True;fixups=[];initFixups=[];compiled={};originals={}
for rel,symbol,address,size,offset,count in specs:
 source=ROOT/'rebuild/src/compiled'/rel
 if rel=='00/9f/CVolume_Destructor_009fccf0.cpp' and 'CVolumeTexture::GetVolumeLevel' not in source.read_text():source=ROOT/'work/parity_addtext/volume_get_level_candidate_20261009.cpp'
 obj=compiled.get(source)
 if obj is None:
  obj=w/f'{address:x}.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr;originals[source]=obj
  if address==0xa67770:
   renamed=w/'initialise-observed.obj';run=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/DInitialiseFromVolumeTextureMipmap=ActualInitialiseFromVolumeTextureMipmap','/Fo'+str(renamed),str(source)],env,w);assert not run.returncode,run.stdout+run.stderr
   left,right=Coff(obj),Coff(renamed);ls,lo=left.symbols[symbol];rs,ro=right.symbols['?ActualInitialiseFromVolumeTextureMipmap@CVolume@@QAEXPAUIDirect3DVolume9@@@Z'];assert lo==ro and left.sections[ls]==right.sections[rs];obj=renamed
  if address==0xa67ea0:
   renamed=w/'getlevel-observed.obj';run=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/DGetVolumeLevel=ActualGetVolumeLevel','/Fo'+str(renamed),str(source)],env,w);assert not run.returncode,run.stdout+run.stderr
   left,right=Coff(obj),Coff(renamed);ls,lo=left.symbols[symbol];rs,ro=right.symbols['?ActualGetVolumeLevel@CVolumeTexture@@QBE?AVCVolume@@J@Z'];assert lo==ro and left.sections[ls]==right.sections[rs];obj=renamed
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
assert len(fixups)==27
from gate_render_state_static_data import coff_data
rootData=compiled[ROOT/'rebuild/src/compiled/00/a6/CVolume_InitialiseFromVolumeTextureMipmap_00a67770.cpp'];assert coff_data(Coff(rootData),'?TotalMemory@CVolume@@2KA',4)==read_virtual_data(raw,0x13d28dc,4)==bytes(4)
dataSource=ROOT/'rebuild/src/compiled/01/39/MemoryAllocatorData_01396868.cpp';dataObj=w/'data.obj';run=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(dataObj),str(dataSource)],env,w);assert not run.returncode,run.stdout+run.stderr;objects.append(dataObj);sources.append(dataSource);coffs.append(Coff(dataObj));assert coff_data(Coff(dataObj),'?g_FableAllocatorMemoryBaseline_013CAAB0@@3KA',4)==read_virtual_data(raw,0x13caab0,4)==bytes(4)
tableSource=ROOT/'rebuild/src/compiled/01/29/CPixelFormat_Table_0129ba40.cpp';tableObj=w/'table.obj';run=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(tableObj),str(tableSource)],env,w);assert not run.returncode,run.stdout+run.stderr;objects.append(tableObj);sources.append(tableSource);coffs.append(Coff(tableObj));table=read_virtual_data(raw,0x129ba40,47*36);assert coff_data(Coff(tableObj),'?GD3DFormats@@3QBVCD3DFormatInfo@@B',47*36)==table
import struct
(w/'volume_texture_calc_data_retail.h').write_text('struct Fixup{unsigned site,target,kind;};\nstatic const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const Fixup fixups[]={'+','.join('{%u,%u,%u}'%f for f in fixups)+'};\nstatic const unsigned retailFormats[]={'+','.join(map(str,struct.unpack('<423I',table)))+'};\n')
aliases=['#pragma comment(linker,"/alternatename:@ExecuteInit@12=?ActualInitialiseFromVolumeTextureMipmap@CVolume@@QAEXPAUIDirect3DVolume9@@@Z")','#pragma comment(linker,"/alternatename:?InitialiseFromVolumeTextureMipmap@CVolume@@QAEXPAUIDirect3DVolume9@@@Z=@ObserveInit@12")','#pragma comment(linker,"/alternatename:@TextureCalcBytes@8='+specs[0][1]+'")','#pragma comment(linker,"/alternatename:@ExecuteGetLevel@16=?ActualGetVolumeLevel@CVolumeTexture@@QBE?AVCVolume@@J@Z")','#pragma comment(linker,"/alternatename:?GetVolumeLevel@CVolumeTexture@@QBE?AVCVolume@@J@Z=@ObserveGetLevel@16")','#pragma comment(linker,"/alternatename:_VolumeVTable=??_7CVolume@@6B@")','#pragma comment(linker,"/alternatename:_BaseVTable=??_7CBaseClass@@6B@")']
defined={name for c in coffs for name in c.symbols};undefined={name for c in coffs for data,rels in c.sections.values() for name,kind in rels.values() if name not in defined and name not in ('?InitialiseFromVolumeTextureMipmap@CVolume@@QAEXPAUIDirect3DVolume9@@@Z','?GetVolumeLevel@CVolumeTexture@@QBE?AVCVolume@@J@Z','??2@YAPAXI@Z','??3@YAXPAX@Z','__purecall','??_7type_info@@6B@','?BeginLoading@CFrameData@CGraphicInfoFrame@@QAEXKK@Z')}
for name in sorted(undefined):aliases.append('#pragma comment(linker,"/alternatename:'+name+'=_AbortBoundary")')
(w/'volume_texture_calc_data_aliases.h').write_text('\n'.join(aliases)+'\n')
fixture=ROOT/'rebuild/tests/integration/NativeVolumeTextureCalcData_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/I'+str(ROOT/'rebuild/include'),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)]
for omit in range(12):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=90);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
assert 'cases=88320 snapshots=' in r.stdout,(w,r.stdout)
headers=[ROOT/'rebuild/include'/p for p in ('fable_volume_types.h','fable_base_class.h','fable_d3d9.h','fable_memory_allocator.h','fable_d3d_format_info.h','fable_surface_types.h','fable_extents_i.h')]
report=dict(cases=88320,snapshots=int(r.stdout.split('snapshots=')[1]),providers=proofs,omission_checks=12,result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Twelve mandatory actual sources: CalcByteLength101B native/102B near compiled,LevelCount10B,GetLevel115B,VolumeCopy40B,Uninitialise27B,BaseCtor9B/Dtor7B,Initialise96B,ByteLength77B,PFInit50B/Depth22B,original1692B format table andcounterdata. Only functional boundaries COMLevelCount/GetLevel/AddRef/Release/GetDesc. GetLevel/Initialise own-method symbol renames prove full code-relocation equality, observers capture only live caller/inner temporaries then execute full actual bodies in each mode. All46 formats,signedcounts,callback owner/temporary PD/VT/counter mutation, independent accounting/counterwrap,full16KiB+8Bglobals snapshots. Temporary addresses neverlogged/masked or read afterlife; two placements-fills. No additional helper/function/table/data coverage.')
(ROOT/'rebuild/compile-gate/native-volume-texture-calc-data.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_VOLUME_TEXTURE_CALC_DATA_GATE_PASS')
