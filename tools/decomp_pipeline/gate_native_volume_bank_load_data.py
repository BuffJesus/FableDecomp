import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
from gate_game_component_abi import Coff
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
w=Path(tempfile.mkdtemp(prefix='native-volume-bank-load-data-'));env=V.env()
specs=[
 ('00/9f/CVolume_Destructor_009fccf0.cpp','?LoadVolumeGraphic@CGraphicDataBank@@AAE_NAAVCManagedTexture@@AAVCBankFileAsyncData@@ABVCGraphicHeader@1@_NK3@Z',0x9fd9b0,462,0,21),
 ('00/9f/CVolume_Destructor_009fccf0.cpp','?GetVolumeLevel@CVolumeTexture@@QBE?AVCVolume@@J@Z',0xa67ea0,115,512,7),
 ('00/9f/CVolume_Destructor_009fccf0.cpp','??1CVolume@@UAE@XZ',0x9fccf0,24,640,3),
 ('00/a6/CVolume_CopyConstructor_00a673e0.cpp','??0CVolume@@QAE@ABV0@@Z',0xa673e0,40,688,2),
 ('00/a6/CVolume_Uninitialise_00a67460.cpp','?Uninitialise@CVolume@@QAEX_N@Z',0xa67460,27,752,0),
 ('00/99/CBaseClass_CBaseClass_0099a2d0.cpp','??0CBaseClass@@QAE@XZ',0x99a2d0,9,800,1),
 ('00/99/CBaseClass_CBaseClass_0099a2d0.cpp','??1CBaseClass@@UAE@XZ',0x99a2e0,7,816,1),
 ('00/a6/CVolume_GetByteLength_00a674e0.cpp','?GetByteLength@CVolume@@QBEKXZ',0xa674e0,77,848,2),
 ('00/a6/CVolume_GetPixelFormat_00a67480.cpp','?GetPixelFormat@CVolume@@QBE?AVCPixelFormat@@XZ',0xa67480,43,944,1),
 ('00/a6/CVolume_GetDimensions_00a674b0.cpp','?GetDimensions@CVolume@@QBE?AVC3DExtentsI@@XZ',0xa674b0,46,1008,0),
 ('00/9e/CPixelFormat_Initialise_009e3830.cpp','?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z',0x9e3830,50,1072,1),
 ('00/9e/CPixelFormat_GetByteColourDepth_009e3800.cpp','?GetByteColourDepth@CPixelFormat@@QBEKXZ',0x9e3800,22,1136,1),
 ('00/a6/CVolume_Swizzle_00a67440.cpp','?Swizzle@CVolume@@QAEXAAVCVolumeLockedBox@@@Z',0xa67440,3,1184,0),
 ('00/a6/CVolume_Unlock_00a67410.cpp','?Unlock@CVolume@@QBEXXZ',0xa67410,10,1200,0),
 ('00/9d/CBankFileAsyncData_GetData_009d5530.cpp','?GetData@CBankFileAsyncData@@QAEPAXXZ',0x9d5530,77,1248,3),
 ('00/a0/CHandleBasedMemoryPool_Lock_00a026f0.cpp','?Lock@CHandleBasedMemoryPool@@QAEXJ@Z',0xa026f0,30,1344,0),
 ('00/a6/CCriticalSectionLock_Constructor_00a62920.cpp','??0CCriticalSectionLock@@QAE@AAU_RTL_CRITICAL_SECTION@@_N@Z',0xa62920,54,1408,2),
 ('00/a6/CCriticalSectionLock_Destructor_00a629a0.cpp','??1CCriticalSectionLock@@QAE@XZ',0xa629a0,25,1472,1)]
targets={symbol:address for rel,symbol,address,size,offset,count in specs}
external={'?InitialiseFromVolumeTextureMipmap@CVolume@@QAEXPAUIDirect3DVolume9@@@Z':(0xa67770,0x80000002,'@InitVolume@12'),'?Lock@CVolume@@QAE?AVCVolumeLockedBox@@K@Z':(0xa677d0,0x80000003,'@LockVolume@16'),'??3@YAXPAX@Z':(0xbfe9bc,0x80000005,'_HeapDelete')}
external.update({'__imp__EnterCriticalSection@4':(0x143fe0c,0x80000008,'_EnterIAT'),'__imp__TryEnterCriticalSection@4':(0x143fd30,0x80000009,'_TryIAT'),'__imp__LeaveCriticalSection@4':(0x143fe10,0x8000000a,'_LeaveIAT')})
rename={'__imp__EnterCriticalSection@4':'__imp__BankEnter@4','__imp__TryEnterCriticalSection@4':'__imp__BankTry@4','__imp__LeaveCriticalSection@4':'__imp__BankLeave@4'}
targets.update({symbol:value[0] for symbol,value in external.items()});targets.update({'??_7CVolume@@6B@':0x129c840,'??_7CBaseClass@@6B@':0x1231710,'?GD3DFormats@@3QBVCD3DFormatInfo@@B':0x129ba40})
offsets={address:offset for rel,symbol,address,size,offset,count in specs};offsets.update({value[0]:value[1] for value in external.values()});offsets.update({0x129c840:0x80000000,0x1231710:0x80000001,0x129ba40:0x80000006,0x129ba48:0x80000007})

objects=[];sources=[];proofs=[];coffs=[];body=bytearray(1536);raw=EXE.read_bytes();cs=Cs(CS_ARCH_X86,CS_MODE_32);cs.detail=True;fixups=[];initFixups=[];compiled={}
for rel,symbol,address,size,offset,count in specs:
 source=ROOT/'rebuild/src/compiled'/rel
 if rel=='00/9f/CVolume_Destructor_009fccf0.cpp' and 'LoadVolumeGraphic' not in source.read_text():source=ROOT/'work/parity_addtext/volume_bank_load_candidate_20261009.cpp'
 obj=compiled.get(source)
 if obj is None:
  obj=w/f'{address:x}.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr;compiled[source]=obj
  c=Coff(obj)
  if address in (0xa62920,0xa629a0):
   replaced=w/f'{address:x}-imports.obj';run=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(replaced),'/DEnterCriticalSection=BankEnter','/DTryEnterCriticalSection=BankTry','/DLeaveCriticalSection=BankLeave',str(source)],env,w);assert not run.returncode,run.stdout+run.stderr
   renamed=Coff(replaced);a,off=c.symbols[symbol];b,other=renamed.symbols[symbol];left,lr=c.sections[a];right,rr=renamed.sections[b];assert off==other and left==right and {p:(rename.get(t,t),k) for p,(t,k) in lr.items()}==rr
   objects.append(replaced);coffs.append(renamed)
  else:objects.append(obj);coffs.append(c)
  sources.append(source)
 proofs.append(check(obj,symbol,address,size,count,targets));b=read_virtual_data(raw,address,size);body[offset:offset+size]=b
 for ins in cs.disasm(b,address):
  if ins.mnemonic in ('call','jmp') and ins.bytes[0] in (0xe8,0xe9):
   target=int(ins.op_str,16)
   if not address<=target<address+size:assert target in offsets;fixups.append((offset+ins.address-address+1,offsets[target],20))
  if ins.disp_size==4 and ins.disp in offsets:fixups.append((offset+ins.address-address+ins.disp_offset,offsets[ins.disp],6))
  if ins.mnemonic not in ('call','jmp') and ins.imm_size==4:
   for operand in ins.operands:
    if operand.type==2 and operand.imm in offsets:fixups.append((offset+ins.address-address+ins.imm_offset,offsets[operand.imm],6))
assert len(fixups)==46,fixups
# Include actual table data in the defined set, so omission cannot become an abort alias.
tableSource=ROOT/'rebuild/src/compiled/01/29/CPixelFormat_Table_0129ba40.cpp';tableObj=w/'table.obj'
r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(tableObj),str(tableSource)],env,w);assert not r.returncode,r.stdout+r.stderr
objects.append(tableObj);sources.append(tableSource);coffs.append(Coff(tableObj))
c=coffs[-1];sec,off=c.symbols['?GD3DFormats@@3QBVCD3DFormatInfo@@B'];tableBytes,tableRelocs=c.sections[sec]
originalTable=read_virtual_data(raw,0x129ba40,1692);assert off==0 and tableBytes==originalTable and not tableRelocs
import struct
tableWords=struct.unpack('<423I',originalTable)
(w/'volume_bank_load_data_retail.h').write_text('struct Fixup{unsigned site,target,kind;};\nstatic const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const Fixup fixups[]={'+','.join('{%u,%u,%u}'%f for f in fixups)+'};\nstatic const unsigned retailFormats[]={'+','.join(map(str,tableWords))+'};\n')
aliases=['#pragma comment(linker,"/alternatename:'+symbol+'='+value[2]+'")' for symbol,value in external.items()]
aliases+=['#pragma comment(linker,"/alternatename:'+new+'='+external[old][2]+'")' for old,new in rename.items()]
aliases+=['#pragma comment(linker,"/alternatename:@VolumeBankLoad@32='+specs[0][1]+'")','#pragma comment(linker,"/alternatename:_VolumeVTable=??_7CVolume@@6B@")','#pragma comment(linker,"/alternatename:_BaseVTable=??_7CBaseClass@@6B@")']
defined={name for c in coffs for name in c.symbols}
undefined={name for c in coffs for data,rels in c.sections.values() for name,kind in rels.values() if name not in defined and name not in external and name not in rename.values() and name not in ('??2@YAPAXI@Z','__purecall','??_7type_info@@6B@')}
for name in sorted(undefined):aliases.append('#pragma comment(linker,"/alternatename:'+name+'=_AbortBoundary")')
(w/'volume_bank_load_data_aliases.h').write_text('\n'.join(aliases)+'\n')
fixture=ROOT/'rebuild/tests/integration/NativeVolumeBankLoadData_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/I'+str(ROOT/'rebuild/include'),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)]
for omit in range(16):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=180);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
assert 'cases=94208 snapshots=5676032' in r.stdout,(w,r.stdout)
headers=[ROOT/'rebuild/include'/p for p in ('fable_volume_types.h','fable_base_class.h','fable_d3d9.h','fable_counted_pointer.h','fable_managed_texture.h','fable_managed_texture_data.h','fable_game_text_bank.h','engine/CGraphicDataBank.h','fable_graphic_info_frame.h','fable_critical_section_lock.h','fable_handle_memory_pool.h')]
report=dict(cases=94208,snapshots=int(r.stdout.split('snapshots=')[1]),providers=proofs,omission_checks=16,result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Sixteen actualsources compose bank volume loader plus actual77B AsyncGetData,30B PoolLock,54B/25B CriticalLock ctor/dtor. Only deferredVolumeLock, VolumeInitialise, Win32Enter/Leave, COMGetLevel/AddRef/Release/GetDesc/Unlock and counted/heapdelete boundaries. Win32 imports renamed with entire code and relocation maps identical; unusedTryEnter aborts. All46formats,bounded dynamic dimensions,row/slice copying,cached PMem before Win32Enter mutatesbox,Data cached before Leave changes asyncData,section identity cached across owner replacement,pool null/live/rawlocked bytes/valid0-7 handles or minus1/headerLocked/counterwrap,control-null/refcountwrap/deletion,twoplacements/fills;full16KiB independent/native/compiled snapshots andboolreturn,16mandatorysourceomissions. No additionalfunction/helper/table coverage.')
(ROOT/'rebuild/compile-gate/native-volume-bank-load-data.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_VOLUME_BANK_LOAD_DATA_GATE_PASS')
