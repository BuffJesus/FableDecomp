from pathlib import Path
import sys,json,re,struct,hashlib
import pefile
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import gate_native_bank_shared_worker as G
import bank_file_lifetime_factory_worker_evidence as F
from gate_game_component_abi import Coff
b=Path(sys.argv[1]).resolve();r=json.loads((b/'receipt.json').read_text());objects=[Path(p) for p in r['objects']]
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
assert all(sha(p)==r['objects'][str(p)] for p in objects)
bank=json.loads((R/'rebuild/build/native-bank-file_lifetime_factory-owner/00/9d/009d6060/20261019T221038607481Z/receipt.json').read_text())
paths=json.loads((R/'rebuild/build/native-file-read-paths/00/98/0098e000/20261019T021303326518Z/receipt.json').read_text())
base=json.loads(Path(bank['baseline']).read_text())
aliases=G.banklink.aliases(objects,b/'fixture.cpp');aliases.update(G.banklink.aliases([],R/'rebuild/tests/integration/NativeBankLifetimeColdAliases.h'));aliases.update(F.ALIASES)
mapping={}
for line in (b/'probe.map').read_text().splitlines():
 m=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
 if m:mapping[m[1]]=int(m[2],16)
def resolve(n,seen=()):
 if n in mapping:return mapping[n]
 assert n not in seen and n in aliases,('missing',n)
 return resolve(aliases[n],seen+(n,))
coffs=[Coff(p) for p in objects];records={}
for record in base['selected_bodies']+bank['consumer_original_records']+bank['bank_tables']+paths['helpers']:
 records[record['symbol']]=record
checked=[];fixups=0
with pefile.PE(str(b/'probe.exe')) as pe:
 for symbol,local,sites in [('?Open@CThreadedFile@@QAE_NABVCWideString@@_N@Z','_s_NextReadPhysicalSortKey',(265,275)),('?OpenForWrite@CThreadedFile@@QAE_NABVCWideString@@_N@Z','_s_NextPhysicalSortKey',(107,117))]:
  if symbol not in mapping:continue
  locations=[struct.unpack('<I',pe.get_data(mapping[symbol]-pe.OPTIONAL_HEADER.ImageBase+x,4))[0] for x in sites]
  assert locations[0]==locations[1]
  rva=locations[0]-pe.OPTIONAL_HEADER.ImageBase;section=pe.get_section_by_rva(rva)
  assert section.Characteristics&0x80000000 and (rva-section.VirtualAddress>=section.SizeOfRawData or pe.get_data(rva,4)==b'\0'*4)
  mapping[local]=locations[0]
 if '_s_NextPhysicalSortKey' in mapping:assert mapping['_s_NextPhysicalSortKey']!=mapping['_s_NextReadPhysicalSortKey']
 for symbol,record in records.items():
  if symbol not in mapping:continue
  matching=[]
  for c in coffs:
   if symbol not in c.symbols:continue
   section,offset=c.symbols[symbol];body,rels=c.sections[section]
   if offset==0 and body.hex()==record['body']:
    expected_names=[(x['site'],x['symbol']) for x in record['relocations']]
    # Bank destructor observer renaming is intentionally absent from read-path COMDATs.
    names=[(site,'_FixtureBankInitFree' if name=='_free' and any(n=='_FixtureBankInitFree' for _,n in expected_names) else name) for site,(name,kind) in rels.items()]
    if sorted(names)==sorted(expected_names):matching.append((body,rels))
  assert matching,('no reproduced provider',symbol)
  address=resolve(symbol);valid=[]
  for body,rels in matching:
   expected=bytearray(body)
   for site,(name,kind) in rels.items():
    value=resolve(name)+struct.unpack_from('<i',body,site)[0]-(address+site+4 if kind==20 else 0);struct.pack_into('<I',expected,site,value&0xffffffff)
   if pe.get_data(address-pe.OPTIONAL_HEADER.ImageBase,len(expected))==bytes(expected):valid.append(rels)
  assert valid,('wrong final provider',symbol)
  checked.append(symbol);fixups+=len(valid[0])
required=['?Open@CThreadedFile@@QAE_NABVCWideString@@_N@Z','?SplitPath@CAFile@@SIXABVCWideString@@PAV2@111@Z','?ToCharString@CWideString@@QBE?AVCCharString@@XZ','?GetAsUpperCase@CCharString@@QBE?AV1@XZ','?CloseAllStreamsForFile@CThreadedFileManager@@QAEXPAVCThreadedFile@@@Z']
assert set(required)<=set(checked)
proof=dict(status='PASS',linked_bodies=len(checked),fixups=fixups,symbols=checked,exe_sha256=sha(b/'probe.exe'),boundary='Accepted original classifications reused after hash-pinned object reproduction; exact final body plus every named relocation verified. Uppercase277B and CloseAllStreams840B retain prior near-match classifications.')
(b/'linked-verification.json').write_text(json.dumps(proof,indent=2)+'\n');print(len(checked),fixups)
