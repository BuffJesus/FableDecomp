"""Native ReadEntryList: rebuilt and transplanted retail root against semantic cases.

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
from file_input_stream_sources import ROOTS as STREAM_ROOTS,TARGETS as STREAM_TARGETS,SOURCES as STREAM_SOURCES

b=Path(sys.argv[1]).resolve();b.mkdir()
print(b,flush=True)
from native_bank_header_sources import SOURCES as sources,HEADER_INDEX
fixture=R/'rebuild/tests/integration/NativeBankHeaderValid_test.cpp'
inputs={Path(__file__),Path(M.__file__),Path(U.M.__file__),Path(U.P.M.__file__),Path(Q.__file__),Path(Q.M.__file__),Path(A.__file__),Path(Z.__file__),*HELPERS,EXE,R/'rebuild/tests/integration/NativeBankEntryList_model.py',R/'rebuild/tests/integration/NativeBankBlockOrder_model.py',R/'rebuild/build/symbol_index.tsv'};visited=set()
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
 result=V.cl(['/nologo','/c','/O2','/MT','/Gy',*(['/GS'] if p.name in ('CAFile_SplitPath_00997620.cpp','CBankFile_ReadFileInformation_009d0880.cpp') else []),*(['/Dmalloc=FixtureVectorMalloc','/Dfree=FixtureVectorFree'] if name!='Fixture' else []),'/I'+str(inc),'/Fo'+str(obj),str(b/(name+'.cpp'))],env,b)
 (b/(name+'.compile.json')).write_text(json.dumps(dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr)))
 assert not result.returncode,result.stdout+result.stderr
 objects.append(obj);coffs.append(Coff(obj))
command=[str(V.VC/'bin/link.exe'),'/nologo','/OPT:REF','/out:'+str(b/'probe.exe'),'/map:'+str(b/'probe.map'),*map(str,objects),'kernel32.lib']
result=subprocess.run(command,cwd=b,env=env,capture_output=True,text=True)
(b/'link.json').write_text(json.dumps(dict(command=command,returncode=result.returncode,stdout=result.stdout,stderr=result.stderr)))
assert not result.returncode,result.stdout+result.stderr
mapping={}
for line in (b/'probe.map').read_text().splitlines():
 m=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
 if m:mapping[m[1]]=int(m[2],16)
stats=M.write_cases(b/'cases.bin')
result=subprocess.run([str(b/'probe.exe'),str(b/'cases.bin'),str(b/'synthetic-bank.bin')],cwd=b,capture_output=True,text=True,timeout=30)
(b/'run.json').write_text(json.dumps(dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr)))
assert result.returncode==0 and 'cases=3840 entries=24520 updates=12432' in result.stdout,result.stdout+result.stderr
symbol='?ReadFileInformation@CBankFile@@AAE_N_N@Z';section,offset=coffs[HEADER_INDEX].symbols[symbol];payload,rels=coffs[HEADER_INDEX].sections[section]

score=compare(payload,read_virtual_data(EXE.read_bytes(),0x9d0880,1635),tuple(rels),retail_relocations('009d0880',1635)).to_dict()
# Bounded native original-header execution: valid-header paths only. Calls whose
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
assert original_run.returncode==0 and 'cases=3840 entries=24520 updates=12432' in original_run.stdout,original_run.stdout+original_run.stderr
(b/'original-header-relocations.json').write_text(json.dumps(dict(calls=proof,copied_constant_addresses={hex(k):hex(v) for k,v in constants.items()}),indent=2))
for p,h in pins.items():assert sha(Path(p))==h,('input drift',p)
report=dict(status='SCRATCH_PASS',build=str(b),host=stats,original_header_cases=3840,similarity=score,input_sha256=pins,object_sha256={str(p):sha(p) for p in objects},executable_sha256=sha(b/'probe.exe'),original_executable_sha256=sha(b/'original-header.exe'),boundary='Fresh same-TU constructor candidate verified identical to bounded canonical provider; valid-header chain: recreated/original header roots use native disk/input/parser/allocation/indexing/packing/queries and string ownership. Fixture headers/manual vector teardown, handle adoption/length/open/base lifetime and notifications controlled. Header error-only dependencies fail fast. No near-match or promotion claim.')
(b/'receipt.json').write_text(json.dumps(report,indent=2));print(result.stdout);print(score)
