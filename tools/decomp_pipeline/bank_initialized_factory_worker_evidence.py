"""Actual bank construction before packed factory/read composition."""
import struct
import bank_packed_factory_worker_evidence as packed
import gate_native_bank_initialization as initialization
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
R=packed.R
INITIALIZATION=[R/'rebuild/src/compiled'/row[1] for row in initialization.SPECS]
SOURCES=packed.SOURCES+INITIALIZATION[:-1]
EXTRAS=packed.EXTRAS
REQUIRED_SOURCES=packed.REQUIRED_SOURCES+INITIALIZATION[:-1]

def flags(source):return ['/Dmalloc=FixtureBankInitMalloc'] if source in INITIALIZATION[:-1] else []

def audit(objects,sources,selected):
 records=packed.audit(objects,sources,selected)
 addresses=dict(initialization.ADDRESS);addresses['_FixtureBankInitMalloc']=addresses['_malloc']
 for source,(name,relative,symbol,address,size) in zip(INITIALIZATION,initialization.SPECS):
  c=Coff(objects[sources.index(source)]);section,offset=c.symbols[symbol];assert offset==0
  body,rels=c.sections[section];assert len(body)==size;patched=bytearray(body)
  for site,(target,kind) in rels.items():
   assert kind in (6,20);value=addresses[target]+struct.unpack_from('<i',body,site)[0]-(address+site+4 if kind==20 else 0);struct.pack_into('<I',patched,site,value&0xffffffff)
  assert bytes(patched)==read_virtual_data(EXE.read_bytes(),address,size),symbol
  record=dict(symbol=symbol,address=address,body=body.hex(),relocations=[dict(site=s,symbol=n,kind=k) for s,(n,k) in rels.items()])
  prior=[r for r in selected if r['symbol']==symbol]
  if prior:assert all(r['body']==record['body'] and r['relocations']==record['relocations'] for r in prior)
  else:selected.append(record)
  records.append(record)
 return records

def controls(sources):
 result=packed.controls(sources)
 mutations=[('init-wrong-alignment',0,'Alignment = 1;','Alignment = 2;'),('init-wrong-format',0,'BankFormatVersion = 0x73;','BankFormatVersion = 0x72;'),('init-noncached-default',1,'NonCached = false;','NonCached = true;'),('init-disabled-default',1,'DisableAsync = false;','DisableAsync = true;'),('init-stale-restart',1,'LastRestartingData = 0;','LastRestartingData = reinterpret_cast<CBankFileAsyncData*>(1);'),('init-packed-bias',2,'Bias(0)','Bias(1)'),('init-wide-counter',3,'++g_CWideStringInstanceCount_013BCA20;',';'),('init-char-counter',4,'++g_CCharStringInstanceCount_013BD800;',';'),('init-cache-buffer',5,'(requestedBufferSize + 8) & ~8','requestedBufferSize + 8')]
 for name,index,old,new in mutations:
  source=INITIALIZATION[index];text=source.read_text();assert text.count(old)==1;result.append((name,sources.index(source),text.replace(old,new)))
 source=INITIALIZATION[1];early=source.read_text().replace('CBankFileAsync::CBankFileAsync()','CBankFileAsync::CBankFileAsync() : LastRestartingData(0)').replace('    LastRestartingData = 0;\n','')
 result.append(('init-early-restart-store',sources.index(source),early))
 return result
