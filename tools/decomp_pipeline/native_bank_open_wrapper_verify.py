from pathlib import Path
import sys,json,re,struct,hashlib
import pefile
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
from native_exact_roots import import_names
import gate_native_bank_shared_worker as G
import bank_file_lifetime_factory_worker_evidence as F
import disk_open_original_evidence as D
from gate_native_bank_async_open import ADDRESS as ASYNC
sys.path.insert(0,str(R/'rebuild/tests/integration'))
from NativeBankBaseOpenRead_model import ADDRESS as BASE
b=Path(sys.argv[1]).resolve();r=json.loads((b/'receipt.json').read_text());objects=[Path(p) for p in r['objects']];coffs=[Coff(p) for p in objects]
assert all(hashlib.sha256(p.read_bytes()).hexdigest()==r['objects'][str(p)] for p in objects)
specs=[(1,'?OpenFileReadOnly@CBankFile@@UAE_NABVCWideString@@K@Z',0x9d1090,204),(2,'?OpenFileReadOnly@CBankFileAsync@@UAE_NABVCWideString@@K@Z',0x9d5820,122),
 (3,'?Reset@?$CCountedPointer@VCDiskFileWin32@@@@QAEXPAVCDiskFileWin32@@@Z',0x9d1ee0,103),(3,'?DeleteData@?$CCountedPointer@VCDiskFileWin32@@@@CIXPAX@Z',0x9a9030,11),
 (4,'?GetBankPathname@CBankFile@@QBE?AVCWideString@@XZ',0x9cbf10,20),(5,'?NotifyBankOpened@CBankFile@@MAEXXZ',0x9cbc00,1),
 (7,'?Close@CDiskFileWin32@@UAEXXZ',0x99a920,52),(8,'?Reset@?$CCountedPointer@VCThreadedFile@@@@QAEXPAVCThreadedFile@@@Z',0x9a9c80,103),(9,'?DeleteData@?$CCountedPointer@VCThreadedFile@@@@SIXPAX@Z',0x9a9040,11),
 (10,'?ConvertPathToFullPath@CAFile@@SI_NABVCWideString@@AAV2@@Z',0x999140,64),(11,'??BCCharString@@QBEPBDXZ',0x99e4c0,15),
 (1,'??_GCDiskFileWin32@@UAEPAXI@Z',0x4016b0,51),(1,'?GetLength@CDiskFileWin32@@UBEJXZ',0x401670,4),(1,'?IsOpen@CDiskFileWin32@@UBE_NXZ',0x401680,4),
 (1,'?GetPathname@CDiskFileWin32@@UBE?AVCWideString@@XZ',0x401690,22),(1,'?IsIOFinished@CAFile@@UBE_NXZ',0x401650,3),(1,'?GetAlignment@CAFile@@UBEJXZ',0x401660,6)]
address={**ASYNC,**BASE,**{symbol:entry for _,symbol,entry,_ in specs},'??0CCharString@@QAE@ABV0@@Z':0x99ec30,'??0CWideString@@QAE@ABV0@@Z':0x99b720,'?GetCurrentPath@CAFile@@SI?AVCWideString@@XZ':0x997f80,'?ExpandPath@CAFile@@SI?AVCWideString@@ABV2@0@Z':0x998c90,'??1CBaseClassNonCopyable@@UAE@XZ':0x99a300,'??3@YAXPAX@Z':0xbfe9bc,'?g_FableEmptyNarrowString_0129AAF4@@3PADA':0x129aaf4}
imports={v:k for k,v in import_names(EXE.read_bytes()).items()};records=[]
for index,symbol,entry,size in specs:
 c=Coff(b/('extra%d.obj'%index));sec,off=c.symbols[symbol];body,rels=c.sections[sec];assert not off and len(body)==size,(symbol,len(body),size)
 expected=bytearray(body)
 for site,(name,kind) in rels.items():
  target=imports[name[7:].split('@')[0]] if name.startswith('__imp__') else address[name];value=target+struct.unpack_from('<i',body,site)[0]-(entry+site+4 if kind==20 else 0);struct.pack_into('<I',expected,site,value&0xffffffff)
 assert bytes(expected)==read_virtual_data(EXE.read_bytes(),entry,size),symbol
 records.append(dict(symbol=symbol,body=body.hex(),relocations=[dict(site=site,symbol=name,kind=kind) for site,(name,kind) in rels.items()]))
_,disk=D.audit(b/'extra6.obj');c=Coff(b/'extra6.obj');symbol='?Open@CDiskFileWin32@@UAEXABVCWideString@@W4FileAccessType@@K@Z';sec,off=c.symbols[symbol];body,rels=c.sections[sec]
records.append(dict(symbol=symbol,body=body.hex(),relocations=[dict(site=site,symbol=name,kind=kind) for site,(name,kind) in rels.items()],locals={name:c.symbols[name][1] for name,kind in rels.values() if name.startswith('$L')}))
aliases=G.banklink.aliases(objects,b/'Fixture.cpp');aliases.update(G.banklink.aliases([],R/'rebuild/tests/integration/NativeBankLifetimeColdAliases.h'));aliases.update(F.ALIASES);aliases['??_ECDiskFileWin32@@UAEPAXI@Z']='??_GCDiskFileWin32@@UAEPAXI@Z'
mapping={}
for line in (b/'probe.map').read_text().splitlines():
 m=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
 if m:mapping[m[1]]=int(m[2],16)
def resolve(name,seen=()):
 if name in mapping:return mapping[name]
 assert name not in seen and name in aliases,name
 return resolve(aliases[name],seen+(name,))
fixups=0
with pefile.PE(str(b/'probe.exe')) as pe:
 for record in records:
  symbol=record['symbol'];at=resolve(symbol);expected=bytearray.fromhex(record['body'])
  for rel in record['relocations']:
   site,name,kind=rel['site'],rel['symbol'],rel['kind'];target=at+record['locals'][name] if name.startswith('$L') else resolve(name)
   value=target+struct.unpack_from('<i',expected,site)[0]-(at+site+4 if kind==20 else 0);struct.pack_into('<I',expected,site,value&0xffffffff);fixups+=1
  assert pe.get_data(at-pe.OPTIONAL_HEADER.ImageBase,len(expected))==bytes(expected),symbol
 c=Coff(b/'extra1.obj');sec,off=c.symbols['??_7CDiskFileWin32@@6B@'];body,rels=c.sections[sec];assert not off and len(body)==64 and len(rels)==16
 slots=[]
 for site,(name,kind) in sorted(rels.items()):
  assert kind==6;actual=struct.unpack('<I',pe.get_data(resolve('??_7CDiskFileWin32@@6B@')-pe.OPTIONAL_HEADER.ImageBase+site,4))[0];assert actual==resolve(name)
  slots.append(dict(slot=site//4,symbol=name,original=hex(struct.unpack('<I',read_virtual_data(EXE.read_bytes(),0x122d06c+site,4))[0]),cold=resolve(name)==resolve('_UnusedBankBoundary')))
proof=dict(status='PASS',wrapper_bodies=len(records),wrapper_fixups=fixups,symbols=[r['symbol'] for r in records],disk_vtable=slots,exe_sha256=hashlib.sha256((b/'probe.exe').read_bytes()).hexdigest(),boundary='Header parser controlled, inactive disk slots/write/error-only paths bind explicit fail-fast boundaries. Active wrappers, disk/scalar/count helpers original and final bytes audited.')
(b/'wrapper-verification.json').write_text(json.dumps(proof,indent=2)+'\n');print(len(records),fixups)
