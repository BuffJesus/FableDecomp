from pathlib import Path
import sys,json,re,struct,hashlib,subprocess
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
from native_retail_readonly_verify import verify
import pefile,capstone
b=Path(sys.argv[1]).resolve();r=json.loads((b/'receipt.json').read_text());objects=[Path(p) for p in r['objects']];mapping={}
for line in (b/'probe.map').read_text().splitlines():
 m=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
 if m:mapping[m[1]]=int(m[2],16)
mapping['_ColdBoundary']=mapping['_UnusedBankBoundary'];symbol='?ReadFileInformation@CBankFile@@AAE_N_N@Z'
bridge=Path(next(o for o,p in r['object_sources'].items() if Path(p).name=='NativeBankAsyncHandleBridge.cpp'))
linked=verify(b,objects,{o.name for o in objects if o.name!='Fixture.obj' and o!=bridge},b/'Fixture.cpp');assert linked['linked_symbols']==369 and linked['unique_bodies']==341
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

(b/'original-header-relocations.json').write_text(json.dumps(dict(calls=proof,copied_constants={hex(k):hex(v) for k,v in constants.items()}),indent=2))
# Independently execute original base/async dispatch as well as original header.
from gate_native_bank_handle_open import ADDRESS as BASE_ADDRESS
import bank_begin_worker_link_oracle as banklink
from near_match import compare,retail_relocations
aliases=banklink.aliases(objects,b/'Fixture.cpp')
def resolve(name,seen=()):
 if name in mapping:return mapping[name]
 assert name not in seen and name in aliases,name
 return resolve(aliases[name],seen+(name,))
base_targets={address:name for name,address in BASE_ADDRESS.items()}
async_targets={0x9d06f0:'?OpenReadOnly@CBankFile@@UAE_NABVCCharString@@K@Z',0x99ebf0:'??0CCharString@@QAE@PBDJ@Z',0x9e9f40:'?DisplayProgress@NProgressDisplay@@YIXABVCCharString@@M_N1@Z',0x99eae0:'??1CCharString@@QAE@XZ',0x9a7f80:next(name for name in aliases if name.startswith('?FindContainedBank@')) if any(name.startswith('?FindContainedBank@') for name in aliases) else '?FindContainedBank@CBankFileManager@@QAE_NABVCCharString@@AAUCContainedBank@NBankFileManager@@AAV?$CCountedPointer@VCRetailBank@NBankFileManager@@@@@Z',0x9a9d60:'??1?$CCountedPointer@VCRetailBank@NBankFileManager@@@@QAE@XZ',0x9d6fd0:'??4?$CCountedPointer@VCThreadedFile@@@@QAEAAV0@ABV0@@Z',0x9a9c40:'??1?$CCountedPointer@VCThreadedFile@@@@QAE@XZ',0xbfea1a:'??2@YAPAXI@Z',0x98dfd0:'??0CThreadedFile@@QAE@XZ',0x9a9c80:'?Reset@?$CCountedPointer@VCThreadedFile@@@@QAEXPAVCThreadedFile@@@Z',0x9cbf10:'?GetBankPathname@CBankFile@@QBE?AVCWideString@@XZ',0x98e1e0:'?Open@CThreadedFile@@QAE_NABVCWideString@@_N@Z',0x99b510:'??1CWideString@@QAE@XZ'}
dispatch=bytearray((b/'original-header.exe').read_bytes());dispatch_proof=[]
for index,(name,source,address,length,targets) in enumerate([
 ('?OpenReadOnly@CBankFile@@UAE_NABVCCharString@@K@Z','CBankFile_OpenReadOnly_009d06f0.cpp',0x9d06f0,385,base_targets),
 ('?OpenReadOnly@CBankFileAsync@@UAE_NABVCCharString@@K@Z','CBankFileAsync_OpenReadOnly_009d56c0.cpp',0x9d56c0,348,async_targets)]):
 c=Coff(Path(next(o for o,p in r['object_sources'].items() if Path(p).name==source)));sec,off=c.symbols[name];body,rels=c.sections[sec];original=read_virtual_data(EXE.read_bytes(),address,length);grade=compare(body,original,tuple(rels),retail_relocations('%08x'%address,length)).to_dict();assert grade['near_match']
 pe=pefile.PE(data=bytes(dispatch));last=pe.sections[-1];rva=align(last.VirtualAddress+max(last.Misc_VirtualSize,last.SizeOfRawData),pe.OPTIONAL_HEADER.SectionAlignment);base=pe.OPTIONAL_HEADER.ImageBase+rva;payload=bytearray(original);constants={};calls=[]
 for ins in decoder.disasm(original,address):
  pos=ins.address-address
  if ins.mnemonic=='call' and ins.bytes[0]==0xe8:
   target=int(ins.op_str,16);target_name=targets[target];struct.pack_into('<I',payload,pos+1,(resolve(target_name)-(base+pos+5))&0xffffffff);calls.append(dict(site=hex(ins.address),target=hex(target),symbol=target_name))
  for operand in ins.operands:
   if operand.type==capstone.x86.X86_OP_MEM and not operand.mem.base and not operand.mem.index:target=operand.mem.disp;site=ins.disp_offset
   elif operand.type==capstone.x86.X86_OP_IMM and not ins.group(capstone.CS_GRP_CALL) and not ins.group(capstone.CS_GRP_JUMP):target=operand.imm;site=ins.imm_offset
   else:continue
   if not 0x1200000<=target<0x1600000:continue
   if 0x13ca79c<=target<0x13ca79c+0x38:replacement=resolve('_managerStorage')+target-0x13ca79c
   else:
    if target not in constants:constants[target]=base+len(payload);payload.extend(read_virtual_data(EXE.read_bytes(),target,512))
    replacement=constants[target]
   struct.pack_into('<I',payload,pos+site,replacement)
 header=last.get_file_offset()+40;assert header+40<=pe.OPTIONAL_HEADER.SizeOfHeaders and not any(dispatch[header:header+40]);start=align(len(dispatch),pe.OPTIONAL_HEADER.FileAlignment);size=align(len(payload),pe.OPTIONAL_HEADER.FileAlignment);dispatch.extend(bytes(start+size-len(dispatch)));dispatch[start:start+len(payload)]=payload
 struct.pack_into('<8sIIIIIIHHI',dispatch,header,('.orig%d'%index).encode(),len(payload),rva,size,start,0,0,0,0,0x60000020);struct.pack_into('<H',dispatch,pe.DOS_HEADER.e_lfanew+6,pe.FILE_HEADER.NumberOfSections+1);struct.pack_into('<I',dispatch,pe.OPTIONAL_HEADER.get_field_absolute_offset('SizeOfImage'),align(rva+len(payload),pe.OPTIONAL_HEADER.SectionAlignment));at=resolve(name);root_offset=pe.get_offset_from_rva(at-pe.OPTIONAL_HEADER.ImageBase);dispatch[root_offset:root_offset+5]=b'\xe9'+struct.pack('<I',(base-at-5)&0xffffffff)
 dispatch_proof.append(dict(symbol=name,bytes=length,grade=grade,calls=calls,constants={hex(k):hex(v) for k,v in constants.items()}))
(b/'original-routes.exe').write_bytes(dispatch)
runs=[];cases=[]
for mode in range(3):
 for variant in range(3):
  path=b/('case%d_type%d.bin'%(variant,mode));data=bytearray((b/'synthetic.bin').read_bytes())
  struct.pack_into('<II',data,88,37+8*variant,0x313+0x100*variant)
  struct.pack_into('<I',data,68,mode);struct.pack_into('<I',data,84,mode)
  info=struct.pack('<i',-5) if mode==0 else b'' if mode==1 else struct.pack('<I',3)+b'ALPHA\0\0GAMMA\0'
  info_at=64+12+24+4+len(b'native_async_entry')+8
  struct.pack_into('<I',data,info_at,len(info));data[info_at+4:info_at+4+len(info)]=info
  path.write_bytes(data);cases.append(path)
for executable in ('probe','original-header','original-routes'):
 for mode in range(3):
  for variant in range(3):
   path=cases[mode*3+variant]
   for trial in range(4):
    result=subprocess.run([str(b/(executable+'.exe')),str(path),str(b/'owned-writes.bin'),str(variant),str(mode)],cwd=b,capture_output=True,text=True,timeout=20)
    record=dict(executable=executable,variant=variant,mode=mode,trial=trial,code=result.returncode,stdout=result.stdout,stderr=result.stderr);runs.append(record)
    (b/'variant-runs-progress.json').write_text(json.dumps(runs,indent=2))
    assert result.returncode==0 and ('real_read=1 bytes=%d'%(37+8*variant)) in result.stdout and 'metadata_frees=1' in result.stdout,record
assert all(len({rec['stdout'] for rec in runs if rec['variant']==variant})==1 for variant in range(3))
print('TEXT_METADATA_NATIVE_RUNS',len(runs),flush=True)
controls=[('header_skip_entries','CBankFile_ReadFileInformation_009d0880.cpp','return ReadEntryList(stream,max_index,entry_count);','return true;'),('entry_offset','CBankFile_ReadEntryList_009cfbc0.cpp','data.DataOffset = offset;','data.DataOffset = offset+1;'),('entry_size','CBankFile_ReadEntryList_009cfbc0.cpp','data.DataSize = size;','data.DataSize = size+1;'),('header_alignment','CBankFile_ReadFileInformation_009d0880.cpp','if(ReadOnly)Alignment=alignment;','if(ReadOnly);'),('skip_thread_open','CBankFileAsync_OpenFileReadOnly_009d5820.cpp','ThreadedFile->Open(GetBankPathname(), NonCached);',';'),('completion_not_finished','CBankFileAsyncData_OnFileReadCompletion_009d54a0.cpp','Finished = true;','Finished = false;'),('wrong_request_size','CBankFileAsyncEntry_SetupRead_009d5970.cpp','desc.Size=size;','desc.Size=size+1;'),('file_handle_leak','CThreadedFile_Close_0098ec40.cpp','        CloseHandle(fileHandle_);','        ;'),('skip_outer_replacement','CBankFileAsync_OpenReadOnly_009d56c0.cpp','ThreadedFile.Reset(new CThreadedFile);\n    ThreadedFile->Open(GetBankPathname(),NonCached);',';'),('missing_handle','CBankFile_OpenReadOnly_009d06f0.cpp','BankHandle=handle;',';'),('wrong_dev_flags','CBankFile_OpenReadOnly_009d06f0.cpp','GetDevBankFilePath(handle),flags);','GetDevBankFilePath(handle),flags^8);')]
controls += [
 ('text_schema','NGameText_CDataBank_Metadata_009c79d0.cpp','return 106;','return 105;'),
 ('text_size','NGameText_CDataBank_Metadata_009c79d0.cpp','Entries.resize(size,empty);','Entries.resize(size+1,empty);'),
 ('text_narrator','NGameText_CDataBank_NotifyEntryAdded_009c8c00.cpp','entry.NarratorIndex=*reinterpret_cast<const long*>(info.GetAsCArray());','entry.NarratorIndex=0;'),
 ('text_entry','NGameText_CDataBank_NotifyEntryAdded_009c8c00.cpp','Entries[index]=&TextEntryBuffer.back();','Entries[index]=0;'),
 ('text_defaults','NGameText_CDataBank_Constructor_009c85a0.cpp','ResourceList.UnloadDelay = 0;','ResourceList.UnloadDelay = 2;'),
 ('group_entry','NGameText_CDataBank_NotifyEntryAdded_009c8c00.cpp','Entries[index]=&GroupEntryBuffer.back();','Entries[index]=0;'),
 ('narrator_count','NGameText_CDataBank_NotifyEntryAdded_009c8c00.cpp','Narrators.resize(count);','Narrators.resize(count+1);')]
records=[];env=V.env();env['INCLUDE']=str(V.VC/'include')+';'+str(b/'include');original_command=json.loads((b/'link.json').read_text())['command']
for name,source,old,new in controls:
 original=Path(next(o for o,p in r['object_sources'].items() if Path(p).name==source));text=original.with_suffix('.cpp').read_text();assert text.count(old)==1;cpp=b/(name+'.cpp');cpp.write_text(text.replace(old,new));fault=b/(name+'.obj');flags=json.loads(original.with_suffix('.compile.json').read_text())['flags']
 result=V.cl(['/nologo','/c','/O2','/MT',*flags,'/I'+str(b/'include'),'/Fo'+str(fault),str(cpp)],env,b);assert not result.returncode,result.stdout+result.stderr
 exe=b/(name+'.exe');command=[str(fault) if arg==str(original) else '/out:'+str(exe) if arg.startswith('/out:') else arg for arg in original_command if not arg.startswith('/map:')]
 result=subprocess.run(command,cwd=b,env=env,capture_output=True,text=True);assert not result.returncode,result.stdout+result.stderr
 mode=1 if name=='group_entry' else 2 if name=='narrator_count' else 0;result=subprocess.run([str(exe),str(cases[mode*3]),str(b/'owned-writes.bin'),'0',str(mode)],cwd=b,capture_output=True,text=True,timeout=20);record=dict(name=name,code=result.returncode,stdout=result.stdout,stderr=result.stderr);records.append(record);print(record,flush=True);assert result.returncode==80 and 'NATIVE_WORKER_FAILURE' in result.stderr,record
# Check allocator identity against original import bytes, including a rollback control.
from native_exact_roots import import_names
node_source=next(Path(path) for path in r['object_sources'].values() if Path(path).name=='CBankInfoMap_InsertNode_009ab120.cpp')
node_obj=Path(next(o for o,path in r['object_sources'].items() if Path(path)==node_source));node_symbol='?InsertNode@CBankInfoMap@CBankFileManager@@QAE?AViterator@12@PAUCNode@12@0ABU?$pair@$$CBVCCharString@@VCBankInfo@CBankFileManager@@@std@@I@Z'
raw=EXE.read_bytes();thunk=read_virtual_data(raw,0xbfea0e,6);assert thunk[:2]==b'\xff\x25' and import_names(raw)[struct.unpack_from('<I',thunk,2)[0]]=='malloc'
def audit_node(obj):
 c=Coff(obj);sec,off=c.symbols[node_symbol];body,rels=c.sections[sec];assert not off and len(body)==206;fixed=bytearray(body)
 for site,(name,kind) in rels.items():
  if name=='_FixtureParseMalloc':target=0xbfea0e
  elif name.startswith('??M?$CBasicString@D@@'):target=0x429950
  elif name.startswith('??0?$pair@$$CBVCCharString@@VCBankInfo@CBankFileManager@@@std@@'):target=0x9a9ad0
  elif name.startswith('?_Rebalance@'):target=0x42971d
  else:raise AssertionError(('unproved dependency',name))
  assert kind==20;struct.pack_into('<I',fixed,site,(target+struct.unpack_from('<i',body,site)[0]-(0x9ab120+site+4))&0xffffffff)
 assert bytes(fixed)==read_virtual_data(raw,0x9ab120,206)
 return dict(bytes=206,relocations=len(rels),allocator='malloc')
allocator=audit_node(node_obj);text=node_obj.with_suffix('.cpp').read_text();assert text.count('malloc(')==1;fault=b/'wrong_node_allocator.cpp';fault.write_text(text.replace('malloc(', '::operator new('));obj=fault.with_suffix('.obj');flags=json.loads(node_obj.with_suffix('.compile.json').read_text())['flags'];result=V.cl(['/nologo','/c','/O2','/MT',*flags,'/I'+str(b/'include'),'/Fo'+str(obj),str(fault)],env,b);assert not result.returncode,result.stdout+result.stderr
rejected=False
try:audit_node(obj)
except AssertionError:rejected=True
assert rejected;allocator['rollback_rejected']=True
# Verify current repository inputs agree with every compiled source/header snapshot.
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();inputs={EXE,Path(__file__),R/'tools/decomp_pipeline/native_text_bank_concrete_build.py',R/'rebuild/tests/integration/NativeTextBankConcrete_test.cpp',b/'synthetic.bin',b/'owned-writes.bin'}
assert not [p for p,h in r['input_sha256'].items() if sha(Path(p))!=h], 'build input drift'
inputs.update(Path(p) for p in r['input_sha256']);inputs.update(cases)
assert not [p for p,h in r['generated_files'].items() if sha(Path(p))!=h], 'generated case drift'
for o,p in r['object_sources'].items():
 p=Path(p);assert p.read_bytes()==Path(o).with_suffix('.cpp').read_bytes(),p;inputs.add(p)
for snapshot in (b/'include').rglob('*.h'):
 relative=snapshot.relative_to(b/'include');candidates=[R/'rebuild/include'/relative,R/'rebuild/tests/integration'/relative];source=next(p for p in candidates if p.is_file());assert source.read_bytes()==snapshot.read_bytes(),source;inputs.add(source)
inputs.update(p for p in (V.VC/'include').rglob('*') if p.is_file());inputs.update(p for d in (V.VC/'bin',V.VC/'lib') for p in d.iterdir() if p.is_file() and p.suffix.lower() in ('.exe','.dll','.lib'))
for module in tuple(sys.modules.values()):
 name=getattr(module,'__file__',None)
 if name:
  p=Path(name).resolve()
  if p.suffix=='.py' and p.is_relative_to(R/'tools/decomp_pipeline'):inputs.add(p)
report=dict(status='PASS',allocator=allocator,dispatch=dispatch_proof,runs=runs,controls=records,linked_symbols=linked['linked_symbols'],unique_bodies=linked['unique_bodies'],input_sha256={str(p):sha(p) for p in inputs},object_sha256=r['objects'],executable_sha256={name:sha(b/name) for name in ('probe.exe','original-header.exe','original-routes.exe')},boundary='Native file-open/header/entry-list/runtime metadata -> actual async GetAsyncEntry/BeginReadData/worker ReadFileEx/APC publication/wait/request removal/file and bank destruction. One generated entry/index3 across three sizes37/45/53,metadata offsets0x313/0x413/0x513,payload read offsets0x1000/0x1080/0x1100,alignment8. 36 recreated,36 original-header and36 original-dispatch+header native processes;whole graph not transplanted. Schema/notification observers forward to actual text methods; installer synchronization and allocator adapters remain explicit;runtime table is parsed,not adopted,and no hand packed metadata remains. Actual manager registration/lookup and async handle dispatch,including original double-reader replacement with first-close observation before possible handle reuse. Manager lookup retail branch explicit fail-fast;progress display null. Concrete NGameText CDataBank schema106, native metadata callbacks, text slot narrator -5 and empty resource-list defaults verified, plus type1 group and type2 three-name narrator metadata through native parser callbacks. No decoded payload/nonempty resource lifetime/malformed/unwind/retail-container claim. Permanent bounded concrete-text-bank-to-worker composition;not a whole-game/retail/unwind claim.')
(b/'text-concrete-verification.json').write_text(json.dumps(report,indent=2));print('TEXT_CONCRETE_VERIFICATION_PASS',len(runs),len(records),linked['linked_symbols'])

