from pathlib import Path
import sys,json,struct,re,hashlib
import pefile
sys.path.insert(0,'tools/decomp_pipeline')
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
b=Path(sys.argv[1]).resolve();sys.path.insert(0,str(Path(__file__).resolve().parents[2]/'rebuild/tests/integration'))
from NativeBankAllocate_model import TARGETS
targets=dict(TARGETS);targets.update({'_FixtureBankAllocateFree':0xbfea14,'??1CCharString@@QAE@XZ':0x99eae0,'?erase@?$vector@VCCharString@@V?$allocator@VCCharString@@@_STL@@@_STL@@QAEPAVCCharString@@PAV3@0@Z':0x43336a})
specs=[('?AllocateBuffers@CBankFile@@AAEXK@Z',0x9ceae0,375),('?DeallocateMemory@?$CArrayVector@UCRuntimeData@CBankFile@@@@QAEXXZ',0x9d3d90,90),('?DeallocateMemory@?$CArrayVector@VCCharString@@@@QAEXXZ',0x9d3fb0,95)]
copy='??$__copy@PAUCRuntimeData@CBankFile@@PAU12@H@_STL@@YIPAUCRuntimeData@CBankFile@@PAU12@00ABUrandom_access_iterator_tag@0@PAH@Z'
targets[copy]=0x9d1c30;specs.append((copy,0x9d1c30,74))
c=Coff(b/'candidate.obj');mapping={};aliases=dict(re.findall(r'/alternatename:([^=]+)=([^"\s]+)',(b/'Fixture.cpp').read_text()))
for line in (b/'probe.map').read_text().splitlines():
 m=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
 if m:mapping[m[1]]=int(m[2],16)
def resolve(name):return mapping[name] if name in mapping else mapping[aliases[name]]
records=[];image=EXE.read_bytes()
with pefile.PE(str(b/'probe.exe')) as pe:
 for symbol,address,size in specs:
  s,o=c.symbols[symbol];body,rels=c.sections[s];assert not o and len(body)==size;fixed=bytearray(body)
  for site,(name,kind) in rels.items():struct.pack_into('<I',fixed,site,(targets[name]+struct.unpack_from('<i',body,site)[0]-(address+site+4 if kind==20 else 0))&0xffffffff)
  assert bytes(fixed)==read_virtual_data(image,address,size),symbol
  start=mapping[symbol];fixed=bytearray(body)
  for site,(name,kind) in rels.items():struct.pack_into('<I',fixed,site,(resolve(name)+struct.unpack_from('<i',body,site)[0]-(start+site+4 if kind==20 else 0))&0xffffffff)
  assert bytes(fixed)==pe.get_data(start-pe.OPTIONAL_HEADER.ImageBase,size),symbol
  records.append(dict(symbol=symbol,address=address,bytes=size,fixups=len(rels)))
p=b/'verification.json';assert not p.exists();p.write_text(json.dumps(dict(status='PASS_SCRATCH',records=records,executable_sha256=hashlib.sha256((b/'probe.exe').read_bytes()).hexdigest(),boundary='Native root and runtime/symbol release helpers. Resize and tree/string erasure controlled; free observer forwards actual CRT free and tracks old blocks.'),indent=2)+'\n');print('ALLOCATION_FINAL_PARITY_PASS',len(records))
