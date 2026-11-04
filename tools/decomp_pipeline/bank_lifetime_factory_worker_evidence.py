"""Native bank destructor and owned metadata after real factory/read completion."""
import struct
import bank_initialized_factory_worker_evidence as initialized
import bank_lifetime_host_evidence as lifetime
import gate_native_bank_shared_worker as shared
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
R=initialized.R
LIFETIME=[R/'rebuild/src/compiled'/row[1] for row in lifetime.SPECS]
ADDED=[p for p in LIFETIME if p not in shared.SOURCES and p not in initialized.SOURCES]
SOURCES=initialized.SOURCES+ADDED
EXTRAS=initialized.EXTRAS
REQUIRED_SOURCES=initialized.REQUIRED_SOURCES+[p for p in ADDED if p not in LIFETIME[12:15]]

def flags(source):
 result=initialized.flags(source)
 if source in ADDED:result+=['/Dfree=FixtureBankInitFree']
 if source==LIFETIME[0]:result+=['/DDeleteCriticalSection=FixtureDeleteCriticalSection']
 return result

def audit(objects,sources,selected):
 records=initialized.audit(objects,sources,selected);lifetime.validate_imports()
 addresses=dict(initialized.initialization.ADDRESS);addresses.update(lifetime.ADDRESS);addresses.update({row[2]:row[3] for row in lifetime.SPECS})
 addresses['_FixtureBankTreeFree']=0xbfea14
 specs=[(p,row[2],row[3],row[4]) for p,row in zip(LIFETIME,lifetime.SPECS)]
 specs.append((initialized.INITIALIZATION[-1],'??1CBaseClassNonCopyable@@UAE@XZ',0x99a300,7))
 for source,symbol,address,size in specs:
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
 result=initialized.controls(sources)
 mutations=[('dtor-skip-section-delete',0,'        DeleteCriticalSection(reinterpret_cast<_RTL_CRITICAL_SECTION*>(CriticalSection));','        ;'),('dtor-keeps-lock',0,'\n        lock.Release();\n','\n        ;\n'),('dtor-skip-list-header-free',0,'if(_M_node) free(_M_node);','if(false) free(_M_node);'),('dtor-skip-bank-close',1,'CloseBankFileImpl(this);',';'),('dtor-char-counter',5,'--g_CCharStringInstanceCount_013BD800;',';'),('dtor-wide-counter',7,'--g_CWideStringInstanceCount_013BCA20;',';')]
 for name,index,old,new in mutations:
  source=LIFETIME[index];text=source.read_text();assert text.count(old)==1,(name,text.count(old));result.append((name,sources.index(source),text.replace(old,new)))
 return result
