"""Direct original and runtime-defect evidence for native bank consumer methods."""
from pathlib import Path
import struct
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
import gate_native_bank_async_data_buffer as buffer
import bank_active_owner_controls as base
R=Path(__file__).resolve().parents[2]
SOURCES=[R/'rebuild/src/compiled'/n for n in ('00/9d/CBankFileAsyncData_WaitUntilFinished_009d5b60.cpp','00/9d/CBankFileAsync_RestartAbortedReads_009d5a90.cpp','00/9d/CBankFileAsyncData_GetData_009d5530.cpp','00/9d/CBankFileAsyncData_Accessors_009d54e0.cpp','00/a0/CHandleBasedMemoryPool_Lock_00a026f0.cpp')]
def flags(source):return []
def audit(objects,sources,selected):
 specs=[(SOURCES[0],'?WaitUntilFinished@CBankFileAsyncData@@QAE_NXZ',0x9d5b60,130),(SOURCES[1],'?RestartAbortedReads@CBankFileAsync@@AAEXXZ',0x9d5a90,200),(SOURCES[2],'?GetData@CBankFileAsyncData@@QAEPAXXZ',0x9d5530,77),(SOURCES[3],'?IsFinished@CBankFileAsyncData@@QBE_NXZ',0x9d54e0,65),(SOURCES[4],'?Lock@CHandleBasedMemoryPool@@QAEXJ@Z',0xa026f0,30)]
 addresses=dict(buffer.ADDRESS)
 addresses.update({symbol:address for source,symbol,address,size in specs})
 addresses.update({'?AddObject@CThreadedFile@@QAEKABVCThreadedFileObjectDesc@@@Z':0x98f350,'?_M_erase@?$_Rb_tree@PAVCBankFileAsyncData@@PAV1@U?$_Identity@PAVCBankFileAsyncData@@@_STL@@U?$less@PAVCBankFileAsyncData@@@3@V?$allocator@PAVCBankFileAsyncData@@@3@@_STL@@QAEXPAU?$_Rb_tree_node@PAVCBankFileAsyncData@@@2@@Z':0x9d7430})
 raw=EXE.read_bytes();records=[]
 for source,symbol,address,size in specs:
  c=Coff(objects[sources.index(source)]);section,offset=c.symbols[symbol];assert offset==0
  body,rels=c.sections[section];assert len(body)==size;b=bytearray(body)
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
 wait=SOURCES[0].name
 add('consumer-skip-wait',wait,'BankFile.ThreadedFile->WaitForObjectToFinish(ThreadedFileUID);',';')
 add('consumer-keeps-bank-lock',wait,'        lock.Release();','        ;')
 add('consumer-wrong-uid',wait,'WaitForObjectToFinish(ThreadedFileUID)','WaitForObjectToFinish(ThreadedFileUID+1)')
 add('consumer-no-buffer-check',wait,'if(!SetupBuffer(true))return false;',';')
 add('consumer-no-restart',wait,'        BankFile.RestartAbortedReads();','        ;')
 add('consumer-wrong-finished-path',wait,'if(!BankFile.DisableAsync && !Finished)','if(!BankFile.DisableAsync)')
 add('consumer-data-null',SOURCES[2].name,'return Data;','return 0;')
 add('consumer-finished-too-early',SOURCES[3].name,'if (Finished) {','if (true) {')
 add('consumer-finished-retains-restart',SOURCES[3].name,'AllowRestart = false;','AllowRestart = true;')
 if sources[0].name in ('NativeQueuedConsumerOwner_test.cpp','NativeFactoryOwner_test.cpp','NativePackedFactoryOwner_test.cpp','NativeInitializedFactoryOwner_test.cpp','NativeLifetimeFactoryOwner_test.cpp','NativeFileBankLifetimeOwner_test.cpp'):
  wait_manager='CThreadedFileManager_WaitForObjectToFinish_0098f1e0.cpp'
  add('consumer-skip-blocking-priority',wait_manager,'    SetObjectPriority(uid,THREADED_FILE_PRIORITY_BLOCKING);','    ;')
  add('consumer-wrong-blocking-priority',wait_manager,'SetObjectPriority(uid,THREADED_FILE_PRIORITY_BLOCKING)','SetObjectPriority(uid,THREADED_FILE_PRIORITY_HIGH)')
 return controls
