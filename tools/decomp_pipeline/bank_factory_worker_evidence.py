"""Original factory, entry size and counted lifetime in the real consumer chain."""
import struct
import bank_consumer_worker_evidence as consumer
import gate_bank_async_entry as factory
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
R=consumer.R
EXTRAS=[R/'rebuild/src/compiled'/n for n in ('00/9c/CBankFile_EntryAccessors_009cc210.cpp','00/9d/CBankFileAsyncEntry_CountedLifetime_009d69a0.cpp')]
SOURCES=consumer.SOURCES+EXTRAS
FACTORY=R/'rebuild/src/compiled/00/9d/CBankFileAsync_GetAsyncEntry_009d58d0.cpp'

def audit(objects,sources,selected):
 records=consumer.audit(objects,sources,selected)
 specs=[(FACTORY,symbol,address,size) for address,symbol,size,index in factory.SPECS]
 specs += [(EXTRAS[0],'?GetEntryDataSize@CBankFile@@QBEKK@Z',0x9cc240,37),(EXTRAS[1],'??1?$CCountedPointer@VCBankFileAsyncEntry@@@@QAE@XZ',0x9c9fc0,53)]
 addresses=dict(factory.TARGETS);addresses['??ACPackedUIntArray@@QBEKK@Z']=0xa629f0
 addresses['??_ECBankFileAsyncEntry@@UAEPAXI@Z']=0x9d6fa0
 specs.append((FACTORY,factory.VTABLE,0x129b880,4))
 raw=EXE.read_bytes()
 for source,symbol,address,size in specs:
  c=Coff(objects[sources.index(source)]);section,offset=c.symbols[symbol];assert offset==0
  body,rels=c.sections[section];assert len(body)==size,(symbol,len(body),size)
  patched=bytearray(body)
  for site,(target,kind) in rels.items():
   assert kind in (6,20)
   value=addresses[target]+struct.unpack_from('<i',body,site)[0]-(address+site+4 if kind==20 else 0)
   struct.pack_into('<I',patched,site,value&0xffffffff)
  assert bytes(patched)==read_virtual_data(raw,address,size),symbol
  record=dict(symbol=symbol,address=address,body=body.hex(),relocations=[dict(site=site,symbol=name,kind=kind) for site,(name,kind) in rels.items()])
  prior=[r for r in selected if r['symbol']==symbol]
  if prior:assert all(r['body']==record['body'] and r['relocations']==record['relocations'] for r in prior)
  else:selected.append(record)
  records.append(record)
 return records

def controls(sources):
 result=consumer.controls(sources)
 def add(name,source,old,new):
  i=sources.index(source);text=source.read_text();assert text.count(old)==1,(name,text.count(old));result.append((name,i,text.replace(old,new)))
 add('factory-wrong-entry-index',FACTORY,'Index(index)','Index(index+1)')
 add('factory-no-padding',FACTORY,'BankFile.Align(BankFile.GetEntryDataSize(index))','BankFile.GetEntryDataSize(index)')
 add('factory-no-info',FACTORY,'Info(data?new CCPPointerInfo(DeleteData,data):0)','Info(0)')
 add('factory-wrong-runtime-size',EXTRAS[0],'return RuntimeData[index].DataSize;','return RuntimeData[index].DataSize+8;')
 add('factory-skip-entry-release',EXTRAS[1],'    Release();\n    Data=0;','    ;\n    Data=0;')
 add('factory-retains-entry-pointer',EXTRAS[1],'    Data=0;','    ;')
 add('factory-retains-info-pointer',EXTRAS[1],'    Info=0;','    ;')
 add('factory-size-ignores-index',EXTRAS[0],'return RuntimeData[index].DataSize;','return RuntimeData[0].DataSize;')
 add('factory-offset-ignores-index',R/'rebuild/src/compiled/00/9c/CBankFile_GetEntryDataOffset_009cc270.cpp','return RuntimeData[index].DataOffset;','return RuntimeData[0].DataOffset;')
 return result
