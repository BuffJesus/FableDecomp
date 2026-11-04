from pathlib import Path
import sys,json,re,struct,subprocess,hashlib
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
from pe_oracle import EXE,read_virtual_data
import pefile,capstone
b=Path(sys.argv[1]).resolve();mapping={}
for line in (b/'probe.map').read_text().splitlines():
 m=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
 if m:mapping[m[1]]=int(m[2],16)
symbol='?ReadFileInformation@CBankFile@@AAE_N_N@Z'
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

reference=json.loads((b/'run.json').read_text())['stdout'];composition=json.loads((b/'composition.json').read_text());runs=[]
for trial in range(2):
 result=subprocess.run([str(b/'original-header.exe'),composition['asset']],cwd=b,capture_output=True,text=True,timeout=30);record=dict(trial=trial,code=result.returncode,stdout=result.stdout,stderr=result.stderr);runs.append(record);assert not result.returncode and result.stdout==reference,record
combined=b/'combined-header';combined.mkdir();(combined/'probe.exe').write_bytes((b/'original-header.exe').read_bytes());(combined/'probe.map').write_bytes((b/'probe.map').read_bytes());(combined/'composition.json').write_text(json.dumps(composition,indent=2));(combined/'run.json').write_bytes((b/'run.json').read_bytes())
result=subprocess.run([sys.executable,str(R/'tools/decomp_pipeline/native_text_public_getter_oracle.py'),str(combined)],capture_output=True,text=True,timeout=60);(combined/'oracle-run.json').write_text(json.dumps(dict(code=result.returncode,stdout=result.stdout,stderr=result.stderr)));assert not result.returncode,result.stdout+result.stderr
report=dict(status='PASS',runs=runs,combined_getter_variants=4,combined_getter_runs=8,header_bytes=1635,relocations=proof,constants={hex(k):hex(v) for k,v in constants.items()},boundary='Original header only executes with recreated native callees, real concrete metadata and decoded-resource fixture; cold error/recovery paths unexercised. No whole graph or unwind claim.')
(b/'original-header-native-receipt.json').write_text(json.dumps(report,indent=2));print('TEXT_SCHEMA_ORIGINAL_HEADER_PASS runs=2')
