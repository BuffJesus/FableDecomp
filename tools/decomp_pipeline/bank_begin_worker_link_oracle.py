"""Retail body and final named-relocation proofs for the bank worker composition."""
import re,struct
from pathlib import Path
import pefile
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
from near_match import compare
import gate_native_bank_async_buffer_constructor as ctor
import gate_native_bank_async_data_buffer as buffer
import gate_native_bank_async_data_teardown as teardown
import gate_native_loading_restart_tree as tree
import gate_native_async_pointer_tree_key_erase as key

R=Path(__file__).resolve().parents[2]
PATHS=(
 '00/9d/CBankFileAsyncEntry_BeginReadData_009d6060.cpp','00/9d/CBankFileAsyncData_CBankFileAsyncData_009d5df0.cpp',
 '00/9d/CBankFileAsyncEntry_SetupRead_009d5970.cpp','00/9c/CBankFile_GetEntryDataOffset_009cc270.cpp',
 '00/9d/CBankFileAsync_GetAsyncEntry_009d58d0.cpp','00/9c/CBankFile_ReadData_009cbf60.cpp',
 '00/9d/CBankFileAsyncData_OnFileReadBegin_009d5440.cpp','00/9d/CBankFileAsyncData_OnFileReadCompletion_009d54a0.cpp',
 '00/9d/CBankFileAsyncData_SetupBuffer_009d52d0.cpp','00/9d/CBankFileAsyncData_CanRelocateData_009d55d0.cpp',
 '00/9d/CBankFileAsyncData_RelocateData_009d5670.cpp','00/9d/CMovableResource_Verify_009d5100.cpp',
 '00/9d/CBankFileAsyncData_Lifetime_009d5410.cpp','00/9d/STLAsyncPointerTree_InsertUnique_009d7cf0.cpp',
 '00/9d/STLAsyncBankTree_Minsert_009d7990.cpp','00/9d/STLAsyncPointerTree_KeyErase_009d7f40.cpp',
 '00/9d/STLAsyncPointerTree_EqualRange_009d7c80.cpp','00/9d/STLAsyncPointerTree_Distance_009d7550.cpp',
 '00/9d/STLAsyncPointerTree_RangeErase_009d7760.cpp','00/9d/STLAsyncBankTree_erase_009d7430.cpp',
 '00/9b/CThreadedFileCallback_OnFileReadBegin_009bdc00.cpp','00/9b/CThreadedFileCallback_OnFileReadCompletion_009bdc10.cpp')
SOURCES=[R/'rebuild/src/compiled'/p for p in PATHS]
BEGIN='?BeginReadData@CBankFileAsyncEntry@@QAE?AV?$CCountedPointer@VCBankFileAsyncData@@@@KPAXKKW4EThreadedFilePriority@@@Z'
SETUP='?SetupRead@CBankFileAsyncEntry@@AAEXKAAVCBankFileAsyncData@@KKKW4EThreadedFilePriority@@@Z'
DELETE='?DeleteData@?$CCountedPointer@VCBankFileAsyncData@@@@CIXPAX@Z'

def flags(source):
 name=source.name
 # Gate snapshots prefix the source position, so retain the original basename suffix.
 if name.endswith('STLAsyncBankTree_Minsert_009d7990.cpp'):return ['/Dmalloc=FixtureBankTreeMalloc']
 if any(name.endswith(s) for s in ('STLAsyncBankTree_erase_009d7430.cpp','STLAsyncPointerTree_RangeErase_009d7760.cpp')):return ['/Dfree=FixtureBankTreeFree']
 return []

def audit(objects,sources,add_selected):
 by_name={p.name:(i,Coff(objects[i])) for i,p in enumerate(sources)}
 roots,addresses=tree.providers()
 addresses.update(buffer.ADDRESS);addresses.update(teardown.ADDRESS)
 addresses.update({r['symbol']:r['address'] for r in roots})
 addresses.update({key.KEY:0x9d7f40,key.EQUAL:0x9d7c80,BEGIN:0x9d6060,SETUP:0x9d5970,DELETE:0x9a42b0,
  '_FixtureBankTreeMalloc':0xbfea0e,'_FixtureBankTreeFree':0xbfea14})
 # This independently accepted pointer-tree ABI is shared with its standalone gate.
 import importlib.util
 path=R/'rebuild/tests/integration/NativeAsyncPointerTreeKeyErase_test.py'
 spec=importlib.util.spec_from_file_location('_bank_worker_tree_specs',path);module=importlib.util.module_from_spec(spec);spec.loader.exec_module(module)
 addresses.update({symbol:address for symbol,index,address,size in module.SPECS})
 specs=[
  (SOURCES[0].name,BEGIN,0x9d6060,160,'MATCH'),(SOURCES[0].name,DELETE,0x9a42b0,11,'MATCH'),
  (SOURCES[1].name,ctor.ROOT,0x9d5d30,179,'MATCH'),
  (SOURCES[1].name,'??_GCBankFileAsyncData@@UAEPAXI@Z',0x9d8180,30,'MATCH'),
  (SOURCES[2].name,SETUP,0x9d5970,191,'NEAR_MATCH'),
  (SOURCES[3].name,'?GetEntryDataOffset@CBankFile@@QBEKK@Z',0x9cc270,36,'MATCH'),
  (SOURCES[6].name,'?OnFileReadBegin@CBankFileAsyncData@@UAE_NPAPAXW4EThreadedFilePriority@@@Z',0x9d5440,92,'MATCH'),
  (SOURCES[7].name,'?OnFileReadCompletion@CBankFileAsyncData@@UAEX_N@Z',0x9d54a0,63,'MATCH'),
  (SOURCES[8].name,'?SetupBuffer@CBankFileAsyncData@@AAE_N_N@Z',0x9d52d0,310,'MATCH'),
  (SOURCES[12].name,'??1CBankFileAsyncData@@UAE@XZ',0x9d61f0,348,'MATCH'),
 ]
 # Add the real pointer providers; predecessor remains behavior-only, without tuning.
 for root in roots:
  if root['address'] in (0x9d7cf0,0x9d74e0,0x9d7990):
   i,c=by_name[root['source'].name];sec,off=c.symbols[root['symbol']];body,rels=c.sections[sec]
   specs.append((root['source'].name,root['symbol'],root['address'],79 if root['address']==0x9d74e0 else len(body),'HELPER_BEHAVIOR_ONLY' if root['address']==0x9d74e0 else 'MATCH'))
 for symbol,source_index,address,size in module.SPECS[:5]:
  name=(SOURCES[15].name,SOURCES[16].name,SOURCES[17].name,SOURCES[18].name,SOURCES[19].name)[source_index]
  specs.append((name,symbol,address,size,'NEAR_MATCH' if symbol==key.KEY else 'MATCH'))
 addresses.update({symbol:address for name,symbol,address,size,grade in specs})
 addresses['?AddObject@CThreadedFile@@QAEKABVCThreadedFileObjectDesc@@@Z']=0x98f350
 addresses['?ReadData@CBankFile@@IBEXKPAXK@Z']=0x9cbf60
 addresses['??ACPackedUIntArray@@QBEKK@Z']=0xa629f0
 reports=[];symbols=[]
 for name,symbol,address,size,grade in specs:
  index,c=by_name[name];sec,off=c.symbols[symbol];assert off==0
  body,rels=c.sections[sec];patched=bytearray(body)
  for site,(target,kind) in rels.items():
   assert kind in (6,20)
   value=addresses[target]+struct.unpack_from('<i',body,site)[0]
   struct.pack_into('<I',patched,site,(value-(address+site+4 if kind==20 else 0))&0xffffffff)
  native=read_virtual_data(EXE.read_bytes(),address,size);score=compare(bytes(patched),native).to_dict()
  if grade=='MATCH':assert bytes(patched)==native,(symbol,len(body),size)
  elif grade=='NEAR_MATCH':assert score['near_match'],(symbol,score)
  add_selected(symbol,index,address,len(body));symbols.append(symbol)
  reports.append(dict(symbol=symbol,address=hex(address),bytes=len(body),original_bytes=size,grade=grade,similarity=score))
 tables=[];index,c=by_name[SOURCES[1].name]
 for name,site,address,slots,values in ctor.TABLES:
  c.table(name,slots);sec,off=c.symbols[name];body,rels=c.sections[sec]
  tables.append(dict(symbol=name,body=body.hex(),relocations=[dict(site=site,symbol=s,kind=k) for site,(s,k) in rels.items()]))
 return reports,symbols,tables

def aliases(objects,fixture):
 result=dict(re.findall(r'/alternatename:([^=\s"]+)=([^\s"]+)',fixture.read_text()))
 for obj in objects:
  c=Coff(obj);sp=struct.unpack_from('<I',c.data,8)[0]
  for index,name in c.indices.items():
   at=sp+index*18
   if c.data[at+16:at+18]==bytes([105,1]):
    default,search=struct.unpack_from('<II',c.data,at+18)
    assert search==2;result[name]=c.indices[default]
 return result

def verify_bindings(exe,selected,symbols,tables,alias):
 # LINK's map names the final selected functions, data and IAT cells. Resolve
 # weak/explicit aliases only when a public spelling is absent from that map.
 mapping={}
 for line in exe.with_suffix('.map').read_text(errors='replace').splitlines():
  m=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
  if m:mapping[m[1]]=int(m[2],16)
 def resolve(name,visited=()):
  if name in mapping:return mapping[name]
  assert name not in visited and name in alias,('unmapped linked symbol',name)
  return resolve(alias[name],visited+(name,))
 count=0
 with pefile.PE(str(exe)) as pe:
  image=pe.OPTIONAL_HEADER.ImageBase
  for record in [r for r in selected if r['symbol'] in symbols]+tables:
   va=resolve(record['symbol']);expected=bytes.fromhex(record['body']);actual=pe.get_data(va-image,len(expected))
   assert len(actual)==len(expected)
   for rel in record['relocations']:
    site,name,kind=rel['site'],rel['symbol'],rel['kind'];addend=struct.unpack_from('<i',expected,site)[0]
    got=struct.unpack_from('<I',actual,site)[0]
    if kind==20:got=(va+site+4+struct.unpack_from('<i',actual,site)[0])&0xffffffff
    else:assert kind==6
    assert got==(resolve(name)+addend)&0xffffffff,(record['symbol'],site,name,hex(got),hex(resolve(name)))
    count+=1
 return count
