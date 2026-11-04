"""Typed file ownership through native bank read and ordered shutdown."""
import struct
import bank_lifetime_factory_worker_evidence as bank
import threaded_file_lifetime_oracle as file_lifetime
from native_exact_roots import import_names
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
R=bank.R
FILE_SOURCES=[R/'rebuild/src/compiled'/p for p in (
 '00/98/CThreadedFile_Constructor_0098dfd0.cpp',
 '00/98/CThreadedFile_Close_0098ec40.cpp',
 '00/98/CThreadedFileManager_CloseAllStreamsForFile_0098e7d0.cpp',
 '00/46/UlongVector_Overflow_00464aab.cpp')]
SOURCES=bank.SOURCES+FILE_SOURCES
REQUIRED_SOURCES=bank.REQUIRED_SOURCES+FILE_SOURCES
EXTRAS=bank.EXTRAS
ALIASES={'??_ECThreadedFile@@UAEPAXI@Z':'??_GCThreadedFile@@UAEPAXI@Z'}
def flags(source):return bank.flags(source)

def audit(objects,sources,selected):
 records=bank.audit(objects,sources,selected)
 specs=[(0,file_lifetime.CTORSYM,0x98dfd0,41),(1,file_lifetime.DTORSYM,0x98f090,67),
        (1,file_lifetime.DELETESYM,0x991450,88),(1,'?Close@CThreadedFile@@QAEXXZ',0x98ec40,47),
        (2,'?CloseAllStreamsForFile@CThreadedFileManager@@QAEXPAVCThreadedFile@@@Z',0x98e7d0,840),
        (3,'?_M_insert_overflow@?$vector@KV?$allocator@K@_STL@@@_STL@@QAEXPAKABKABU__true_type@2@I_N@Z',0x464ace,188),
        (0,'??_7CThreadedFile@@6B@',0x129a158,4)]
 addresses=dict(file_lifetime.ADDRESS);addresses.update({'_malloc':0xbfea0e,'_free':0xbfea14,'_memmove':0xbfeae6,'??_ECThreadedFile@@UAEPAXI@Z':0x991450})
 reverse={v:k for k,v in import_names(EXE.read_bytes()).items()}
 for index,symbol,address,size in specs:
  c=Coff(objects[sources.index(FILE_SOURCES[index])]);section,offset=c.symbols[symbol];body,rels=c.sections[section];assert offset==0 and len(body)==size
  for site,(name,kind) in rels.items():
   if name.startswith('__imp__'):addresses[name]=reverse[name[7:].split('@')[0]]
   elif '_M_insert_overflow' in name:addresses[name]=0x464ace
   elif '_M_find' in name:addresses[name]=0x98fe70
   elif name.startswith('?find@'):addresses[name]=0x9902d0
   elif '_M_lower_bound' in name:addresses[name]=0x9901d0
   elif '_M_upper_bound' in name:addresses[name]=0x98fca0
   elif '?erase@' in name:addresses[name]=0x9905d0
   elif '_Rebalance_for_erase' in name:addresses[name]=0x42a34e
  patched=bytearray(body)
  for site,(name,kind) in rels.items():
   assert kind in (6,20);value=addresses[name]+struct.unpack_from('<i',body,site)[0]-(address+site+4 if kind==20 else 0);struct.pack_into('<I',patched,site,value&0xffffffff)
  original=read_virtual_data(EXE.read_bytes(),address,size)
  if address==0x98e7d0:assert [i for i,(a,b) in enumerate(zip(patched,original)) if a!=b]==list(range(493,501))
  else:assert bytes(patched)==original,symbol
  record=dict(symbol=symbol,address=address,body=body.hex(),relocations=[dict(site=s,symbol=n,kind=k) for s,(n,k) in rels.items()]);selected.append(record);records.append(record)
 # Confirm the constructor vtable's vector-name fallback is genuine COFF weak data.
 c=Coff(objects[sources.index(FILE_SOURCES[0])]);ptr,count=struct.unpack_from('<II',c.data,8);found=False;i=0
 while i<count:
  at=ptr+i*18;storage,aux=struct.unpack_from('<BB',c.data,at+16)
  if c.indices[i] in ALIASES:
   assert storage==105 and aux==1;fallback,search=struct.unpack_from('<II',c.data,at+18)
   assert search==2 and c.indices[fallback]==ALIASES[c.indices[i]];found=True
  i+=1+aux
 assert found
 return records

def controls(sources):
 result=bank.controls(sources)
 for name,index,old,new in (
  ('file-invalid-handle',0,'reinterpret_cast<void*>(-1)','reinterpret_cast<void*>(0)'),
  ('file-skip-manager-close',1,'        g_FableThreadedFileManager_013BC870.CloseAllStreamsForFile(this);','        ;'),
  ('file-skip-handle-close',1,'        CloseHandle(fileHandle_);','        ;')):
  source=FILE_SOURCES[index];text=source.read_text();assert text.count(old)==1;result.append((name,sources.index(source),text.replace(old,new)))
 return result
