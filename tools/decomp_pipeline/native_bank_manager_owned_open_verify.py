from pathlib import Path
import sys,json,struct,subprocess
R=Path(__file__).resolve().parents[2];sys.path[:0]=[str(R/'tools/decomp_pipeline')];import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
from near_match import compare,retail_relocations
from gate_native_bank_handle_open import SYMBOL,ADDRESS
from gate_native_bank_manager_paths import ROOTS,ADDRESS as PATH_ADDRESS
import gate_bank_info_insertion as INSERT
def run(b):
 r=json.loads((b/'receipt.json').read_text());objects=[Path(p) for p in r['object_sha256']]
 def obj(source):return Path(next(o for p,o in r['source_objects'].items() if Path(p).name==source))
 c=Coff(obj('CBankFile_OpenReadOnly_009d06f0.cpp'));sec,off=c.symbols[SYMBOL];body,rels=c.sections[sec];grade=compare(body,read_virtual_data(EXE.read_bytes(),0x9d06f0,385),tuple(rels),retail_relocations('009d06f0',385)).to_dict();assert grade['near_match'];print('HANDLE',len(body),grade)
 root=ROOTS[0];c=Coff(obj(root['source'].name));sec,off=c.symbols[root['symbol']];body,rels=c.sections[sec];fixed=bytearray(body)
 for site,(name,kind) in rels.items():struct.pack_into('<I',fixed,site,(PATH_ADDRESS[name]+struct.unpack_from('<i',body,site)[0]-(root['address']+site+4 if kind==20 else 0))&0xffffffff)
 assert bytes(fixed)==read_virtual_data(EXE.read_bytes(),root['address'],len(body));print('DEV_PATH_EXACT',len(body))
 c=Coff(obj('CBankInfoMap_InsertNode_009ab120.cpp'));symbol=INSERT.CASES[2][2];sec,off=c.symbols[symbol];body,rels=c.sections[sec];rewritten={site:('_malloc' if name=='_FixtureVectorMalloc' else name,kind) for site,(name,kind) in rels.items()};INSERT.check_call_targets(body,rewritten,read_virtual_data(EXE.read_bytes(),0x9ab120,206),'009ab120');assert len(body)==206
 controls=[('handle_missing','CBankFile_OpenReadOnly_009d06f0.cpp','BankHandle=handle;',';'),('wrong_flags','CBankFile_OpenReadOnly_009d06f0.cpp','GetDevBankFilePath(handle),flags);','GetDevBankFilePath(handle),flags^8);'),('empty_dev_path','CBankFileManager_HeaderPaths_009a7e20.cpp','return BasePath() + static_cast<CBankInfoMap::CEntryNode*>(node)->value_.second.DevBankFilePath;','return CWideString(L"");'),('wrong_node_allocator','CBankInfoMap_InsertNode_009ab120.cpp','malloc(', '::operator new(')]
 records=[];env=V.env();env['INCLUDE']=str(V.VC/'include')+';'+str(b/'include')
 for name,source,old,new in controls:
  original=obj(source);text=original.with_suffix('.cpp').read_text();assert text.count(old)==1;cpp=b/(name+'.cpp');cpp.write_text(text.replace(old,new));fault=b/(name+'.obj')
  result=V.cl(['/nologo','/c','/O2','/MT','/Gy','/Dmalloc=FixtureVectorMalloc','/Dfree=FixtureVectorFree','/I'+str(b/'include'),'/Fo'+str(fault),str(cpp)],env,b);assert not result.returncode,result.stdout+result.stderr
  exe=b/(name+'.exe');command=[str(V.VC/'bin/link.exe'),'/nologo','/OPT:REF','/out:'+str(exe),*[str(fault if p==original else p) for p in objects],'kernel32.lib'];result=subprocess.run(command,cwd=b,env=env,capture_output=True,text=True);assert not result.returncode,result.stdout+result.stderr
  result=subprocess.run([str(exe),str(b/'cases.bin'),str(b/'synthetic-bank.bin')],cwd=b,capture_output=True,text=True,timeout=30);record=dict(name=name,code=result.returncode,stdout=result.stdout,stderr=result.stderr);print(record);assert result.returncode==80,record
  if name=='wrong_node_allocator':assert 'ALLOC_PAIR got=1 wanted=0 size=40' in result.stderr
  records.append(record)
 report=dict(status='PASS',handle_grade=grade,dev_path_exact=145,node_bytes=206,node_allocator='malloc',controls=records,boundary='Actual manager ctor/map insert/find/GetDevFilePath/OpenReadOnly/dtor dev branch;retail lookup explicit fail-fast;host storage/configuration/schema/notifications controlled.230 native linked symbols exclude Manager bridge.')
 (b/'manager-proof.json').write_text(json.dumps(report,indent=2));print('MANAGER_CONTROLS_PASS',len(records));return report
