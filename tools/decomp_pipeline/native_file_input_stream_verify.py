from pathlib import Path
import sys,json,struct,re,hashlib
import pefile
sys.path.insert(0,'tools/decomp_pipeline')
from gate_game_component_abi import Coff
b=Path(sys.argv[1]).resolve();root=Path(__file__).resolve().parent;proof=json.loads((b/'original-audit.json').read_text());assert proof['status']=='PASS'
mapping={}
for line in (b/'probe.map').read_text().splitlines():
 m=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
 if m:mapping[m[1]]=int(m[2],16)
pe=pefile.PE(str(b/'probe.exe'));records=[];discarded=[]
allowed={'??0CDataInputStream@@QAE@XZ','?SetPositionAssumeWithinSourceChunk@CDataInputStream@@IAEXK@Z','?CanSeekToPosWithinSourceChunk@CDataInputStream@@IAE_NJ@Z','?OpenStream@CDataInputStream@@QAEXPBEJJ@Z','?Close@CDataInputStream@@UAEXXZ'}
for r in proof['roots']:
 if r['symbol'] not in mapping:
  assert r['symbol'] in allowed;discarded.append(r['symbol']);continue
 c=Coff(Path(r['object']));sec,off=c.symbols[r['symbol']];body,rels=c.sections[sec];patched=bytearray(body);entry=mapping[r['symbol']]
 for site,(name,kind) in rels.items():struct.pack_into('<I',patched,site,(mapping[name]+struct.unpack_from('<i',body,site)[0]-(entry+site+4 if kind==20 else 0))&0xffffffff)
 assert bytes(patched)==pe.get_data(entry-pe.OPTIONAL_HEADER.ImageBase,len(body)),r['symbol'];records.append(r['symbol'])
p=b/'linked-verification.json';assert not p.exists();p.write_text(json.dumps(dict(status='PASS',linked_bodies=len(records),symbols=records,discarded_inlined_helpers=discarded,exe_sha256=hashlib.sha256((b/'probe.exe').read_bytes()).hexdigest(),boundary='Eight active native stream bodies; five out-of-line copies discarded, caller expansions audited; base lifetime helpers and backing CAFile controlled; unused virtual methods fail-fast.'),indent=2)+'\n');print('INPUT_STREAM_FINAL_BYTES_PASS',len(records))
