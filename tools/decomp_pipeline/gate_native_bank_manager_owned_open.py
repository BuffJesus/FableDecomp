"""Native owned bank open: actual path/disk/header/parser/lifetime composition.

No installed executable is modified. All native executables are build-owned.
"""
from pathlib import Path
from datetime import datetime, timezone
import sys, json, subprocess, re, struct, hashlib
R=Path(__file__).resolve().parents[2]
sys.path[:0]=[str(R/'tools/decomp_pipeline'),str(R/'rebuild/tests/integration')]
import check_one as V
from gate_game_component_abi import Coff
from near_match import compare,retail_relocations
from pe_oracle import EXE,read_virtual_data
import NativeBankEntryIndexed_model as M
import gate_native_bank_init_complete as Q
import gate_native_bank_pack_runtime as U
import NativeBankAllocate_model as A
import NativeBankVectorResize_model as Z
HELPERS=[R/'tools/decomp_pipeline'/name for name in ('native_bank_entry_allocation_verify.py','native_bank_entry_allocation_controls.py','native_bank_entry_index_verify.py','native_bank_entry_packing_verify.py')]
import pefile,capstone,csv
import native_bank_owned_open_verify
import native_bank_parent_helpers
import native_bank_manager_owned_open_verify
from file_input_stream_sources import ROOTS as STREAM_ROOTS,TARGETS as STREAM_TARGETS,SOURCES as STREAM_SOURCES

b=R/'rebuild/build/native-bank-manager-owned-open/00/9d/009d06f0'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');b.mkdir(parents=True)
print(b,flush=True)
SOURCES=['00/42/CBasicStringchar_operator_00429950.cpp', '00/40/Global_CCRC_Calc_004014a0.cpp', '00/41/Destroyrangestd_allocatorCSpeechFrame_00414e00.cpp', '00/42/BankFilenameTree_InsertNode_0042bcb5.cpp', '00/42/BankSymbolMap_Index_0042d131.cpp', '00/42/BankSymbolMap_InsertHint_0042c940.cpp', '00/42/BankSymbolMap_InsertUnique_0042bd4a.cpp', '00/42/BankSymbolMap_LowerBound_0042ad34.cpp', '00/42/STL_Rb_global_Rebalance_0042971d.cpp', '00/43/STLStringVector_Mfillinsert_004333b3.cpp', '00/43/STLStringVector_Minsertoverflow_00433530.cpp', '00/43/STLStringVector_erase_0043336a.cpp', '00/43/global_Fillnunsignedlongunsignedintunsigned_00433504.cpp', '00/46/UlongVector_Overflow_00464aab.cpp', '00/46/UlongVector_Resize_00464931.cpp', '00/46/UlongVector_Storage_0046496f.cpp', '00/49/STLCharStringVector_Resize_0049b760.cpp', '00/57/STLBankStringTree_erase_00579435.cpp', '00/99/CAFile_SplitPath_00997620.cpp', '00/99/CCharString_AllocStringData_0099ea60.cpp', '00/99/CCharString_Constructor_0099ebf0.cpp', '00/99/CCharString_ConversionOperator_0099e4c0.cpp', '00/99/CCharString_CopyAssignment_0099efb0.cpp', '00/99/CCharString_DefaultConstructor_0099e4b0.cpp', '00/99/CCharString_GetAsLowerCase_0099ed10.cpp', '00/99/CCharString_ToWideString_0099e480.cpp', '00/99/CCharString_operatorcharconstptr_0099af00.cpp', '00/99/CDataInputStream_CDataInputStream_00993b20.cpp', '00/99/CDataInputStream_ReadPresizedString_00996390.cpp', '00/99/CDataInputStream_SetPosition_00993bc0.cpp', '00/99/CDataInputStream_SourceChunk_00993c10.cpp', '00/99/CFileDataInputStream_Close_00994300.cpp', '00/99/CFileDataInputStream_Destructor_00994780.cpp', '00/99/CFileDataInputStream_InternalReadNextSourceChunk_00994360.cpp', '00/99/CFileDataInputStream_InternalReadUnbuffered_009943d0.cpp', '00/99/CFileDataInputStream_NeedsBufferedRead_009943b0.cpp', '00/99/CWideString_AppendWideString_0099b8d0.cpp', '00/99/CWideString_AssignLiteral_0099b800.cpp', '00/99/CWideString_Constructor_0099aed0.cpp', '00/99/CWideString_CopyConstructor_0099b720.cpp', '00/99/CWideString_CreateFromCharString_0099b6a0.cpp', '00/99/CWideString_Destructor_0099b510.cpp', '00/99/CWideString_FromCharConstructor_0099b440.cpp', '00/99/CWideString_MakeOwnCopyOfData_0099b560.cpp', '00/99/CWideString_ToCharString_0099b2c0.cpp', '00/99/CWideString_UnassignString_0099b4d0.cpp', '00/99/DataInputStream_ReadWithSrcChunkOverflow_00993ca0.cpp', '00/99/STLWString_append_range_0099c7e0.cpp', '00/99/STLWideString_FillConstructor_0099ca70.cpp', '00/99/STL_Mthrowlengtherror_0099c550.cpp', '00/9a/CCharString_InitFromCStr_009a0590.cpp', '00/9a/global_CBasicStringCharAssignFromData_009a0300.cpp', '00/9b/CArrayVector_SaveData_DeallocateMemory_009b4ae0.cpp', '00/9b/CVectorMap_CompressedOffset_DeallocateUnusedMemory_009b7b10.cpp', '00/9b/STL_AdjustHeap_CompressedOffset_009b3060.cpp', '00/9b/STL_InsertionSort_CompressedOffset_009b56f0.cpp', '00/9b/STL_Introsort_CompressedOffset_009b7fa0.cpp', '00/9b/STL_LinearInsert_CompressedOffset_009b46b0.cpp', '00/9b/STL_MakeHeap_CompressedOffset_009b3cf0.cpp', '00/9b/STL_PartialSort_CompressedOffset_009b6be0.cpp', '00/9b/STL_Partition_CompressedOffset_009b3900.cpp', '00/9b/STL_PushHeap_CompressedOffset_009b1d80.cpp', '00/9b/STL_SortHeap_CompressedOffset_009b5c10.cpp', '00/9b/STL_Sort_CompressedOffset_009b85a0.cpp', '00/9b/STL_UnguardedInsertionSort_CompressedOffset_009b2db0.cpp', '00/9c/CBankFile_AllocateBuffers_009ceae0.cpp', '00/9c/CBankFile_EntryExists_009cc190.cpp', '00/9c/CBankFile_GetEntryDataOffset_009cc270.cpp', '00/9c/CBankFile_GetEntryDataType_009cc210.cpp', '00/9c/CBankFile_InitEntry_009ce050.cpp', '00/9c/CBankFile_PackRuntimeData_009cd740.cpp', '00/9c/CBankFile_ReadEntryList_009cfbc0.cpp', '00/9c/CBankFile_SymbolHelpers_009cbaa0.cpp', '00/9d/BankFilenameTree_InsertEqual_009d34e0.cpp', '00/9d/CSmallVector_CCharString_AllocateCopy_009d2160.cpp', '00/9d/CSmallVector_CCharString_Clear_009d2110.cpp', '00/9d/STLBankRuntimeVector_Resize_009d4010.cpp', '00/9d/STLBankRuntimeVector_Storage_009d3a20.cpp', '00/9d/STLBankUpdateVector_Resize_009d3df0.cpp', '00/9d/STLBankUpdateVector_Storage_009d3300.cpp', '00/a6/CPackedUIntArray_CalculateBias_00a62ae0.cpp', '00/a6/CPackedUIntArray_ClearPackedBankArray_00a629d0.cpp', '00/a6/CPackedUIntArray_CountBits_00a62b70.cpp', '00/a6/CPackedUIntArray_Get_00a629f0.cpp', '00/a6/CPackedUIntArray_Initialise_00a62d80.cpp', '00/a6/CPackedUIntArray_PackArray_00a62c90.cpp', '00/a6/CPackedUIntArray_Sizeof_00a62a60.cpp', '01/29/FableCRCTable_0129a168.cpp', '01/3b/CharStringInstanceCount_013bd800.cpp']
sources=[R/'rebuild/src/compiled'/s for s in SOURCES]
sources += [R/'rebuild/src/compiled/00/9d/CBankFile_ReadFileInformation_009d0880.cpp']+[R/'rebuild/src/compiled/00/99'/name for name in ['CDiskFileWin32_Read_0099a960.cpp','CDiskFileWin32_SetPosition_0099abf0.cpp','CDiskFileWin32_GetPosition_0099abd0.cpp','CDiskFileWin32_Close_0099A920.cpp']]
sources += [R/'rebuild/src/compiled/00/9c/CBankFile_CloseBankFileImpl_009cd850.cpp',R/'rebuild/src/compiled/00/9d/CBankFile_ReleaseBankSymbols_009d3fb0.cpp']
sources += [R/'rebuild/src/compiled'/s for s in ['00/9c/CBankFile_CBankFile_009cd480.cpp', '00/a6/CPackedUIntArray_CPackedUIntArray_00a629c0.cpp', '00/a6/CChecksumCacheFile_CChecksumCacheFile_00a60c90.cpp', '00/99/CBaseClass_CBaseClass_0099a2d0.cpp', '00/9c/CBankFile_Destructor_009ce180.cpp', '00/a6/CChecksumCacheFile_Destructor_00a60ce0.cpp', '00/a6/CChecksumCacheFile_Close_00a60c30.cpp', '00/a6/CPackedUIntArray_Destructor_00a62ac0.cpp', '00/a6/STLChecksumCacheTree_erase_00a60a00.cpp']]
sources += [R/'rebuild/src/compiled/00/9d/CCountedPointer_DiskFile_Reset_009d1ee0.cpp',R/'rebuild/src/compiled/00/9d/CBankFile_OpenFileReadOnly_009d1090.cpp']
sources += [R/'rebuild/src/compiled/00/99/CDiskFileWin32_Open_0099a6a0.cpp']
sources += [R/'rebuild/src/compiled'/s for s in ['00/99/CAFile_PathExists_00999230.cpp','00/99/CAFile_ExpandPath_00998c90.cpp','00/99/CAFile_GetCurrentPath_00997f80.cpp','00/99/CWideString_LiteralConstructor_0099b6b0.cpp','00/99/CWideString_AddWideStrings_0099be70.cpp','00/99/CWideString_AddWideStringAndLiteral_0099bf30.cpp','00/99/CWideString_AppendCharacter_0099b9a0.cpp','00/99/NFile_CalcCurrentPath_009974a0.cpp','01/3b/CAFileCurrentPath_013bca10.cpp','00/9c/CBankFile_ReadData_009cbf60.cpp'] if s not in SOURCES]
# Existing operator+ family supplies the shared inline string bodies.
sources=[p for p in sources if p.name not in ('CWideString_MakeOwnCopyOfData_0099b560.cpp','CWideString_AppendWideString_0099b8d0.cpp')]
sources += [R/'rebuild/src/compiled'/p for p in ['00/40/Global_MemCmpUnsigned16_00403c60.cpp','00/51/STLWideString_UninitializedFillN_005123a0.cpp','00/99/STLWideString_Reserve_0099c990.cpp','00/99/CWideString_GetSubString_0099bdb0.cpp'] if R/'rebuild/src/compiled'/p not in sources]
sources=[p for p in sources if p.name!='CWideString_Constructor_0099aed0.cpp']
sources.append(R/'rebuild/src/compiled/00/99/CWideString_CopyAssignment_0099b7d0.cpp')
sources.append(R/'rebuild/src/compiled/00/99/CAFile_ConvertPathToFullPath_00999140.cpp')
sources += [R/'rebuild/src/compiled'/name for name in ['00/99/CAFile_GetAbsoluteFilePath_00998250.cpp','00/99/CAFile_GetAbsoluteFilePath_00998c30.cpp','00/40/CStringParser_Constructor_00404720.cpp','00/9b/CStringParser_InitTyped_009b9530.cpp','00/9b/CStringParser_SkipUntilString_009b9a20.cpp','00/9b/CStringParser_SkipPastString_009b9ba0.cpp','00/9b/CStringParser_CalcInQuotesTyped_009b9570.cpp','00/9b/CStringParser_SetStringPos_009b9940.cpp','00/9b/CParsedItem_Accessors_009b9870.cpp','00/99/CCharString_Edit_0099f2a0.cpp','00/99/CCharString_GetSubString_0099ec70.cpp','00/99/CCharString_AddLiteralAndCharString_0099f690.cpp','00/99/CCharString_AppendCharString_0099f0a0.cpp']]
sources=[p for p in sources if p.name not in ('CCharString_DefaultConstructor_0099e4b0.cpp','CCharString_AllocStringData_0099ea60.cpp','CCharString_Constructor_0099ebf0.cpp')]
sources += [R/'rebuild/src/compiled'/name for name in ['00/41/CBasicStringchar_CBasicStringCharoperatorequals_004115a0.cpp','00/9a/CBasicStringchar_Truncate_009a0080.cpp','00/9a/CBasicString_char_Append_009a04e0.cpp']]
sources += [R/'rebuild/src/compiled'/name for name in ['00/9a/CBasicStringchar_ExtendData_009a01c0.cpp']]
sources.append(R/'rebuild/src/compiled/00/9b/CStringParser_ReadAsStringUntilStringItem_009ba110.cpp')
sources.append(R/'rebuild/src/compiled/00/9a/CBasicString_char_SetRepeated_009a0390.cpp')
sources += [R/'rebuild/src/compiled'/name for name in ['00/9d/CBankFile_OpenReadOnly_009d06f0.cpp','00/9a/CBankFileManager_Constructor_009a7ac0.cpp','00/9a/CBankFileManager_Destructor_009a7b70.cpp','00/9a/CBankFileManager_Close_009a7990.cpp','00/9a/CBankFileManager_HeaderPaths_009a7e20.cpp','00/9a/STLBankMaps_Find_009ab4f0.cpp','00/9a/CBankInfoMap_Index_009ac5d0.cpp','00/9a/CBankInfoMap_LowerBound_009aadb0.cpp','00/9a/CBankInfoMap_InsertUniqueHint_009abb10.cpp','00/9a/CBankInfoMap_InsertUnique_009ab1f0.cpp','00/9a/CBankInfoMap_IteratorRetreat_009aa320.cpp','00/9a/CBankInfoMap_IteratorAdvance_009a9e20.cpp','00/9a/CBankInfoMap_Erase_009aa7c0.cpp','00/9a/CBasicRetailBankAliasMap_Erase_009aa830.cpp','00/9a/CBankManagerWidePathSet_Erase_009aa880.cpp','00/9a/CRetailBankList_Clear_009aae50.cpp']]
sources.append(R/'rebuild/tests/integration/NativeBankManagerOwnedOpenBridge.cpp')
sources.append(R/'rebuild/src/compiled/00/9a/CBankInfoMap_InsertNode_009ab120.cpp')
fixture=R/'rebuild/tests/integration/NativeBankManagerOwnedOpen_test.cpp'
inputs={Path(native_bank_parent_helpers.item_model.__file__),Path(native_bank_parent_helpers.repeated_model.__file__),R/'tools/decomp_pipeline/native_bank_header_literals.py',R/'rebuild/tests/integration/NativeBankHeaderThrow_model.py',Path(__file__),Path(M.__file__),Path(U.M.__file__),Path(U.P.M.__file__),Path(Q.__file__),Path(Q.M.__file__),Path(A.__file__),Path(Z.__file__),*HELPERS,EXE,R/'rebuild/tests/integration/NativeBankEntryList_model.py',R/'rebuild/tests/integration/NativeBankBlockOrder_model.py'};visited=set()
def closure(p):
 if p in visited:return
 visited.add(p);inputs.add(p)
 for name in re.findall(r'^\s*#include\s*"([^"\n]+)"',p.read_text(),re.M):
  closure(next(q for q in (p.parent/name,R/'rebuild/include'/name,R/'rebuild/tests/integration'/name) if q.is_file()))
for p in sources+[fixture]:closure(p)
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
inputs.update(p for p in (V.VC/'include').rglob('*') if p.is_file())
inputs.update(p for d in (V.VC/'bin',V.VC/'lib') for p in d.iterdir() if p.is_file() and p.suffix.lower() in ('.exe','.dll','.lib'))
for module in tuple(sys.modules.values()):
 name=getattr(module,'__file__',None)
 if name:
  path=Path(name).resolve()
  if path.suffix=='.py' and path.is_relative_to(R/'tools/decomp_pipeline'):inputs.add(path)
pins={str(p):sha(p) for p in inputs}
(b/'inputs.json').write_text(json.dumps(pins,indent=2))
inc=b/'include';inc.mkdir()
for p in visited:
 q=b/('root%d.cpp'%sources.index(p) if p in sources else 'Fixture.cpp') if p in sources+[fixture] else inc/p.relative_to(R/'rebuild/include') if p.is_relative_to(R/'rebuild/include') else b/p.name
 q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(p.read_bytes())
env=V.env();env['INCLUDE']=str(V.VC/'include')+';'+str(inc)
objects=[];coffs=[]
for i,p in enumerate(sources+[fixture]):
 name='root%d'%i if i<len(sources) else 'Fixture';obj=b/(name+'.obj')
 result=V.cl(['/nologo','/c','/O2','/MT','/Gy',*(['/GS'] if p.name in ('CAFile_SplitPath_00997620.cpp','CBankFile_ReadFileInformation_009d0880.cpp','NFile_CalcCurrentPath_009974a0.cpp') else []),*(['/Dmalloc=FixtureVectorMalloc','/Dfree=FixtureVectorFree'] if name!='Fixture' else []),'/I'+str(inc),'/Fo'+str(obj),str(b/(name+'.cpp'))],env,b)
 (b/(name+'.compile.json')).write_text(json.dumps(dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr)))
 assert not result.returncode,result.stdout+result.stderr
 objects.append(obj);coffs.append(Coff(obj))
command=[str(V.VC/'bin/link.exe'),'/nologo','/OPT:REF','/out:'+str(b/'probe.exe'),'/map:'+str(b/'probe.map'),*map(str,objects),'kernel32.lib']
result=subprocess.run(command,cwd=b,env=env,capture_output=True,text=True)
(b/'link.json').write_text(json.dumps(dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr)))
assert not result.returncode,result.stdout+result.stderr
mapping={}
for line in (b/'probe.map').read_text().splitlines():
 m=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
 if m:mapping[m[1]]=int(m[2],16)
stats=M.write_cases(b/'cases.bin')
result=subprocess.run([str(b/'probe.exe'),str(b/'cases.bin'),str(b/'synthetic-bank.bin')],cwd=b,capture_output=True,text=True,timeout=30)
(b/'run.json').write_text(json.dumps(dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr)))
assert result.returncode==0 and 'cases=9600 entries=' in result.stdout,result.stdout+result.stderr
symbol='?ReadFileInformation@CBankFile@@AAE_N_N@Z';section,offset=coffs[next(n for n,p in enumerate(sources) if p.name=='CBankFile_ReadFileInformation_009d0880.cpp')].symbols[symbol];payload,rels=coffs[next(n for n,p in enumerate(sources) if p.name=='CBankFile_ReadFileInformation_009d0880.cpp')].sections[section]
score=compare(payload,read_virtual_data(EXE.read_bytes(),0x9d0880,1635),tuple(rels),retail_relocations('009d0880',1635)).to_dict()
# Bounded native original-header execution: nonthrow paths only. Calls whose
# dependencies are outside this experiment explicitly point at ColdBoundary.
original=read_virtual_data(EXE.read_bytes(),0x9d0880,1635);raw=(b/'probe.exe').read_bytes();pe=pefile.PE(data=raw)
align=lambda n,a:(n+a-1)//a*a
last=pe.sections[-1];rva=align(last.VirtualAddress+max(last.Misc_VirtualSize,last.SizeOfRawData),pe.OPTIONAL_HEADER.SectionAlignment);base=pe.OPTIONAL_HEADER.ImageBase+rva
decoder=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);decoder.detail=True
targets={0x99b2c0:'?ToCharString@CWideString@@QBE?AVCCharString@@XZ',0x994700:'??0CFileDataInputStream@@QAE@PAVCAFile@@K@Z',0x993bc0:'?SetPosition@CDataInputStream@@UAEXK@Z',0x993ca0:'?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z',0x994780:'??1CFileDataInputStream@@UAE@XZ',0x99eae0:'??1CCharString@@QAE@XZ',0x99ebf0:'??0CCharString@@QAE@PBDJ@Z',0x99e4c0:'??BCCharString@@QBEPBDXZ',0x9cfbc0:'?ReadEntryList@CBankFile@@AAE_NAAVCFileDataInputStream@@KK@Z',0xbfe9f9:'@__security_check_cookie@4'}
transplant=bytearray(original);constants={};proof=[]
for ins in decoder.disasm(original,0x9d0880):
 pos=ins.address-0x9d0880
 if ins.mnemonic=='call' and ins.bytes[0]==0xe8:
  target=int(ins.op_str,16);name=targets.get(target,'_ColdBoundary');struct.pack_into('<I',transplant,pos+1,(mapping[name]-(base+pos+5))&0xffffffff)
  proof.append(dict(site=hex(ins.address),target=hex(target),symbol=name,cold=name=='_ColdBoundary'))
 for operand in ins.operands:
  if operand.type==capstone.x86.X86_OP_MEM and operand.mem.base==0 and operand.mem.index==0:
   address=operand.mem.disp;offset=ins.disp_offset
  elif operand.type==capstone.x86.X86_OP_IMM and ins.mnemonic=='push':address=operand.imm;offset=ins.imm_offset
  else:continue
  if not 0x1200000<=address<0x1600000:continue
  if address==0x139c8a8:target=mapping['___security_cookie']
  else:
   if address not in constants:
    constants[address]=base+len(transplant);transplant.extend(read_virtual_data(EXE.read_bytes(),address,512))
   target=constants[address]
  struct.pack_into('<I',transplant,pos+offset,target)
header=last.get_file_offset()+40;assert header+40<=pe.OPTIONAL_HEADER.SizeOfHeaders and not any(raw[header:header+40])
start=align(len(raw),pe.OPTIONAL_HEADER.FileAlignment);size=align(len(transplant),pe.OPTIONAL_HEADER.FileAlignment)
oracle=bytearray(raw);oracle.extend(bytes(start+size-len(oracle)));oracle[start:start+len(transplant)]=transplant
struct.pack_into('<8sIIIIIIHHI',oracle,header,b'.oracle\0',len(transplant),rva,size,start,0,0,0,0,0x60000020)
struct.pack_into('<H',oracle,pe.DOS_HEADER.e_lfanew+6,pe.FILE_HEADER.NumberOfSections+1)
struct.pack_into('<I',oracle,pe.OPTIONAL_HEADER.get_field_absolute_offset('SizeOfImage'),align(rva+len(transplant),pe.OPTIONAL_HEADER.SectionAlignment))
at=mapping[symbol];root_offset=pe.get_offset_from_rva(at-pe.OPTIONAL_HEADER.ImageBase);oracle[root_offset:root_offset+5]=b'\xe9'+struct.pack('<I',(base-at-5)&0xffffffff)
(b/'original-header.exe').write_bytes(oracle)
original_run=subprocess.run([str(b/'original-header.exe'),str(b/'cases.bin'),str(b/'synthetic-bank.bin')],cwd=b,capture_output=True,text=True,timeout=30)
(b/'original-header.run.json').write_text(json.dumps(dict(returncode=original_run.returncode,stdout=original_run.stdout,stderr=original_run.stderr)))
assert original_run.returncode==0 and 'cases=9600 entries=' in original_run.stdout,original_run.stdout+original_run.stderr
failure_runs={}
for executable in ('probe','original-header'):
 run=subprocess.run([str(b/(executable+'.exe')),str(b/'cases.bin'),str(b/'synthetic-bank.bin'),'failures'],cwd=b,capture_output=True,text=True,timeout=30)
 record=dict(returncode=run.returncode,stdout=run.stdout,stderr=run.stderr);failure_runs[executable]=record
 (b/(executable+'.failures.json')).write_text(json.dumps(record,indent=2))
 assert run.returncode==0 and run.stdout=='NATIVE_BANK_OWNED_OPEN_FAILURES_PASS paths=5 flags=64 missing=320 malformed=1600\n',record
assert failure_runs['probe']==failure_runs['original-header']
(b/'original-header-relocations.json').write_text(json.dumps(dict(calls=proof,copied_constant_addresses={hex(k):hex(v) for k,v in constants.items()}),indent=2))
for p,h in pins.items():assert sha(Path(p))==h,('input drift',p)
report=dict(status='PASS',build=str(b),generated_corpus=stats,failure_runs=failure_runs,host={key:int(value) for key,value in re.findall(r'(\w+)=(\d+)',result.stdout)},original_header_cases=9600,source_objects={str(p):str(b/('root%d.obj'%n)) for n,p in enumerate(sources)},similarity=score,input_sha256=pins,object_sha256={str(p):sha(p) for p in objects},executable_sha256=sha(b/'probe.exe'),original_executable_sha256=sha(b/'original-header.exe'),boundary='native manager construction/map registration/lookup/GetDevBankFilePath, bank-handle OpenReadOnly then absolute/rooted/plain/dot-relative/parent-relative read-only bank open and disk ownership, header/parser/packing/query/string graph, direct guarded ReadData, native bank ctor/dtor and counted peer sharing. Schema/notification vtable and generated file corpus are controlled. Manager bridge owns storage/configuration; real manager constructor/map registration/destructor and dev dispatch execute. Retail lookup is explicit fail-fast, with UseRetailBanks enabled only when flag4 forces development dispatch. Both manager/provider and bank cleanup enforce malloc/new allocation provenance. Five manager handle/path spellings. Separate direct file-open failure seam has 320 missing/1600 nonthrow malformed opens per recreated/original header; empty/magic/bank-version/schema-version/state errors return false and release file ownership before bank destruction. Throwing error-only dependencies and entry payload decode remain outside this gate. Original transplant validates header paths only; no original whole-open execution or exception unwinding claim.')
assert report['host']==dict(paths=5,flags=64,cases=9600,entries=59640,updates=30560,overflow=0,packs=9600,sorted=6000,packed=8040,runtime=1120,noop=440),report['host']
assert result.stdout==original_run.stdout
(b/'receipt.json').write_text(json.dumps(report,indent=2))
proof=native_bank_owned_open_verify.run(b)
# Manager.cpp is a host bridge, not a reconstructed production provider.
from native_retail_readonly_verify import verify as verify_native_bodies
bridge=Path(next(o for p,o in report['source_objects'].items() if Path(p).name=='NativeBankManagerOwnedOpenBridge.cpp'))
linked=verify_native_bodies(b,objects,{p.name for p in objects if p.name!='Fixture.obj' and p!=bridge},b/'Fixture.cpp')
proof['linked_symbols']=linked['linked_symbols'];proof['unique_bodies']=linked['unique_bodies']

assert proof['linked_symbols']==230 and proof['exact_roots']==8 and len(proof['controls'])==7,proof
for p,h in pins.items():assert sha(Path(p))==h,('input drift',p)
report['verification']=proof
report['parent_helpers']=native_bank_parent_helpers.run(b)
report['manager']=native_bank_manager_owned_open_verify.run(b)
assert len(report['manager']['controls'])==4
for p,h in pins.items():assert sha(Path(p))==h,('input drift',p)
for p in inputs:
 if p.suffix=='.py' and p.is_relative_to(R):
  q=b/'python-provenance'/p.relative_to(R);q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(p.read_bytes())
(b/'receipt.json').write_text(json.dumps(report,indent=2))
(R/'rebuild/compile-gate/native-bank-manager-owned-open.json').write_text(json.dumps(report,indent=2))
print('NATIVE_BANK_MANAGER_OWNED_OPEN_GATE_PASS paths=5 flags=64 cases=9600 original_header=9600 entries=59640 missing=320 malformed=1600 controls=7 linked='+str(proof['linked_symbols'])+' helper_controls=6 item_model=304 repeated_model=440 parent_exact=2 helper_near=2 manager_controls=4')

