"""Original instruction/relocation evidence and controls for native disposal ownership."""
from pathlib import Path
import struct
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
import gate_native_bank_async_data_teardown as teardown
import bank_active_owner_controls as base
R=Path(__file__).resolve().parents[2]
SOURCES=[R/'rebuild/src/compiled/00/9d'/n for n in ('CBankFileAsync_DiscardAsyncData_009d67e0.cpp','CBankFileAsyncData_DiscardAsyncData_009d68a0.cpp','STLAsyncDisposalList_push_back_009d7b00.cpp','CBankFileAsync_ProcessDisposalList_009d6730.cpp')]
UPDATE_SOURCES=[R/'rebuild/src/compiled/00/9d'/n for n in ('CBankFileAsync_AsyncUpdatePending_009d68b0.cpp','CBankFileAsync_RestartAbortedReads_009d5a90.cpp')]
UPDATE='?AsyncUpdatePending@CBankFileAsync@@QAEXXZ'
RESTART='?RestartAbortedReads@CBankFileAsync@@AAEXXZ'
BANK='?DiscardAsyncData@CBankFileAsync@@QAEXAAV?$CCountedPointer@VCBankFileAsyncData@@@@@Z'
STATIC='?DiscardAsyncData@CBankFileAsyncData@@SIXAAV?$CCountedPointer@VCBankFileAsyncData@@@@@Z'
PUSH='?push_back@?$list@V?$CCountedPointer@VCBankFileAsyncData@@@@V?$allocator@V?$CCountedPointer@VCBankFileAsyncData@@@@@_STL@@@_STL@@QAEXABV?$CCountedPointer@VCBankFileAsyncData@@@@@Z'
PROCESS='?ProcessDisposalList@CBankFileAsync@@QAEXXZ'
ABORT='?Abort@CBankFileAsyncData@@QAE_NXZ'
def flags(source):
 if source.name==SOURCES[2].name:return ['/Dmalloc=FixtureDisposalMalloc']
 if source.name==SOURCES[3].name:return ['/Dfree=FixtureDisposalFree']
 return []
def audit(objects,sources,selected):
 specs=[(SOURCES[0],BANK,0x9d67e0,180),(SOURCES[1],STATIC,0x9d68a0,16),(SOURCES[2],PUSH,0x9d7b00,59),(SOURCES[3],PROCESS,0x9d6730,164),(teardown.ROOTS[2]['source'],ABORT,0x9d6350,196)]
 addresses=dict(teardown.ADDRESS);addresses.update({BANK:0x9d67e0,STATIC:0x9d68a0,PUSH:0x9d7b00,PROCESS:0x9d6730,ABORT:0x9d6350,'_FixtureDisposalMalloc':0xbfea0e,'_FixtureDisposalFree':0xbfea14})
 if UPDATE_SOURCES[0] in sources:
  specs.extend([(UPDATE_SOURCES[0],UPDATE,0x9d68b0,16),(UPDATE_SOURCES[1],RESTART,0x9d5a90,200)])
  addresses.update({UPDATE:0x9d68b0,RESTART:0x9d5a90,'?AddObject@CThreadedFile@@QAEKABVCThreadedFileObjectDesc@@@Z':0x98f350,'?_M_erase@?$_Rb_tree@PAVCBankFileAsyncData@@PAV1@U?$_Identity@PAVCBankFileAsyncData@@@_STL@@U?$less@PAVCBankFileAsyncData@@@3@V?$allocator@PAVCBankFileAsyncData@@@3@@_STL@@QAEXPAU?$_Rb_tree_node@PAVCBankFileAsyncData@@@2@@Z':0x9d7430})
 raw=EXE.read_bytes();records=[]
 for source,symbol,address,size in specs:
  c=Coff(objects[sources.index(source)]);section,offset=c.symbols[symbol];assert offset==0
  body,rels=c.sections[section];assert len(body)==size
  b=bytearray(body)
  for site,(target,kind) in rels.items():
   assert kind in (6,20);value=addresses[target]+struct.unpack_from('<i',body,site)[0]-(address+site+4 if kind==20 else 0)
   struct.pack_into('<I',b,site,value&0xffffffff)
  assert bytes(b)==read_virtual_data(raw,address,size),symbol
  record=dict(symbol=symbol,address=address,body=body.hex(),relocations=[dict(site=site,symbol=name,kind=kind) for site,(name,kind) in rels.items()])
  prior=[r for r in selected if r['symbol']==symbol]
  if prior:assert all(r['body']==record['body'] and r['relocations']==record['relocations'] for r in prior)
  else:selected.append(record)
  records.append(record)
 return records
def controls(sources):
 controls=[r for r in base.controls(sources) if not r[0].startswith('active-') or r[0] in {'active-allow-reading-removal','active-callback-keeps-manager-lock','active-wrong-transfer-size'}]
 def add(name,file,old,new):
  i=next(i for i,p in enumerate(sources) if p.name==file);text=sources[i].read_text();assert text.count(old)==1,(name,text.count(old));controls.append((name,i,text.replace(old,new)))
 add('disposal-skip-transfer',SOURCES[0].name,'DisposalList.push_back(data);',';')
 add('disposal-retain-caller',SOURCES[0].name,'data.Reset(0);',';')
 add('disposal-skip-initial-process',SOURCES[0].name,'    ProcessDisposalList();','    ;')
 add('disposal-skip-drain',SOURCES[3].name,'while (DisposalList.size() && DisposalList.front()->Abort())','while (false)')
 add('disposal-no-shared-reference',SOURCES[2].name,'if(Info)++Info->RefCount;','if(Info){}')
 add('disposal-extra-shared-reference',SOURCES[2].name,'if(Info)++Info->RefCount;','if(Info)Info->RefCount+=2;')
 add('disposal-premature-abort',teardown.ROOTS[2]['source'].name,'if (Finished) return true;','if (true) return true;')
 add('disposal-never-finished',teardown.ROOTS[2]['source'].name,'if (Finished) return true;','if (Finished) return false;')
 if UPDATE_SOURCES[0] in sources:
  add('coord-skip-disposal',UPDATE_SOURCES[0].name,'    ProcessDisposalList();','    ;')
  add('coord-skip-restart',UPDATE_SOURCES[0].name,'    RestartAbortedReads();','    ;')
  add('coord-reversed-order',UPDATE_SOURCES[0].name,'    ProcessDisposalList();\n    RestartAbortedReads();','    RestartAbortedReads();\n    ProcessDisposalList();')
 return controls
