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
linked=verify(b,objects,{o.name for o in objects if o.name!='Fixture.obj'},b/'Fixture.cpp');assert linked['linked_symbols']==284
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
runs=[];cases=[]
for variant in range(3):
 path=b/('case%d.bin'%variant);data=bytearray((b/'synthetic.bin').read_bytes());struct.pack_into('<II',data,88,37+8*variant,0x313+0x100*variant);path.write_bytes(data);cases.append(path)
for executable in ('probe','original-header'):
 for variant,path in enumerate(cases):
  for trial in range(4):
   result=subprocess.run([str(b/(executable+'.exe')),str(path),str(b/'owned-writes.bin'),str(variant)],cwd=b,capture_output=True,text=True,timeout=20)
   record=dict(executable=executable,variant=variant,trial=trial,code=result.returncode,stdout=result.stdout,stderr=result.stderr);runs.append(record);assert result.returncode==0 and ('real_read=1 bytes=%d'%(37+8*variant)) in result.stdout and 'metadata_frees=1' in result.stdout,record
assert all(len({rec['stdout'] for rec in runs if rec['variant']==variant})==1 for variant in range(3))
print('PARSED_NATIVE_ORIGINAL_HEADER_RUNS',len(runs),flush=True)
controls=[('header_skip_entries','CBankFile_ReadFileInformation_009d0880.cpp','return ReadEntryList(stream,max_index,entry_count);','return true;'),('entry_offset','CBankFile_ReadEntryList_009cfbc0.cpp','data.DataOffset = offset;','data.DataOffset = offset+1;'),('entry_size','CBankFile_ReadEntryList_009cfbc0.cpp','data.DataSize = size;','data.DataSize = size+1;'),('header_alignment','CBankFile_ReadFileInformation_009d0880.cpp','if(ReadOnly)Alignment=alignment;','if(ReadOnly);'),('skip_thread_open','CBankFileAsync_OpenFileReadOnly_009d5820.cpp','ThreadedFile->Open(GetBankPathname(), NonCached);',';'),('completion_not_finished','CBankFileAsyncData_OnFileReadCompletion_009d54a0.cpp','Finished = true;','Finished = false;'),('wrong_request_size','CBankFileAsyncEntry_SetupRead_009d5970.cpp','desc.Size=size;','desc.Size=size+1;'),('file_handle_leak','CThreadedFile_Close_0098ec40.cpp','        CloseHandle(fileHandle_);','        ;')]
records=[];env=V.env();env['INCLUDE']=str(V.VC/'include')+';'+str(b/'include');original_command=json.loads((b/'link.json').read_text())['command']
for name,source,old,new in controls:
 original=Path(next(o for o,p in r['object_sources'].items() if Path(p).name==source));text=original.with_suffix('.cpp').read_text();assert text.count(old)==1;cpp=b/(name+'.cpp');cpp.write_text(text.replace(old,new));fault=b/(name+'.obj');flags=json.loads(original.with_suffix('.compile.json').read_text())['flags']
 result=V.cl(['/nologo','/c','/O2','/MT',*flags,'/I'+str(b/'include'),'/Fo'+str(fault),str(cpp)],env,b);assert not result.returncode,result.stdout+result.stderr
 exe=b/(name+'.exe');command=[str(fault) if arg==str(original) else '/out:'+str(exe) if arg.startswith('/out:') else arg for arg in original_command if not arg.startswith('/map:')]
 result=subprocess.run(command,cwd=b,env=env,capture_output=True,text=True);assert not result.returncode,result.stdout+result.stderr
 result=subprocess.run([str(exe),str(b/'synthetic.bin'),str(b/'owned-writes.bin')],cwd=b,capture_output=True,text=True,timeout=20);record=dict(name=name,code=result.returncode,stdout=result.stdout,stderr=result.stderr);records.append(record);print(record,flush=True);assert result.returncode==80 and 'NATIVE_WORKER_FAILURE' in result.stderr,record
# Verify current repository inputs agree with every compiled source/header snapshot.
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();inputs={EXE,Path(__file__),R/'tools/decomp_pipeline/native_bank_async_parsed_build.py',R/'rebuild/tests/integration/NativeBankAsyncParsed_test.cpp',b/'synthetic.bin',b/'owned-writes.bin'}
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
report=dict(status='PASS',runs=runs,controls=records,linked_symbols=linked['linked_symbols'],unique_bodies=linked['unique_bodies'],input_sha256={str(p):sha(p) for p in inputs},object_sha256=r['objects'],executable_sha256={name:sha(b/name) for name in ('probe.exe','original-header.exe')},boundary='Native file-open/header/entry-list/runtime metadata -> actual async GetAsyncEntry/BeginReadData/worker ReadFileEx/APC publication/wait/request removal/file and bank destruction. One generated entry/index3 across three sizes37/45/53,metadata offsets0x313/0x413/0x513,payload read offsets0x1000/0x1080/0x1100,alignment8. Twelve recreated and twelve original-header-transplant native processes;whole graph not transplanted. Schema/notification observers,installer synchronization and allocator adapters remain explicit;runtime table is parsed,not adopted,and no hand packed metadata remains. No malformed/unwind/retail-container/manager-handle integration in this worker host. Permanent bounded composition;no new root or whole-game claim.')
(b/'async-parsed-verification.json').write_text(json.dumps(report,indent=2));print('ASYNC_PARSED_VERIFICATION_PASS',len(runs),len(records),linked['linked_symbols'])
