from pathlib import Path
import sys,json,re,struct
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import gate_native_bank_initialization as G
import bank_lifetime_host_evidence as L
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
b=Path(sys.argv[1]).resolve();mapping={};owners={}
for line in (b/'probe.map').read_text().splitlines():
 m=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
 if m:mapping[m[1]]=int(m[2],16)
 m=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+[0-9a-fA-F]{8}\s+(?:f\s+)?(?:i\s+)?root(\d+)\.obj\s*$',line)
 if m:owners[m[1]]=int(m[2])
targets=dict(G.ADDRESS);targets.update(L.ADDRESS);targets.update({s[2]:s[3] for s in L.SPECS});targets.update(_FixtureVectorMalloc=0xbfea0e,_FixtureVectorFree=0xbfea14)
specs=[s for s in G.SPECS if s[0] in ('bank','packed','cache','base')]+[L.SPECS[i] for i in (1,3,4,9,13)];proof=[]
for _,_,name,entry,size in specs:
 c=Coff(b/('root%d.obj'%owners[name]));section,offset=c.symbols[name];body,rels=c.sections[section];assert not offset and len(body)==size,(name,len(body),size)
 original=read_virtual_data(EXE.read_bytes(),entry,size);fixed=bytearray(body);dependencies=[]
 for site,(dependency,kind) in rels.items():
  assert dependency in targets,('missing target',dependency)
  target=targets[dependency];addend=struct.unpack_from('<i',body,site)[0];struct.pack_into('<I',fixed,site,(target+addend-(entry+site+4 if kind==20 else 0))&0xffffffff);dependencies.append(dict(symbol=dependency,target=hex(target)))
 assert bytes(fixed)==original,name
 proof.append(dict(symbol=name,entry=hex(entry),bytes=size,dependencies=dependencies))
(b/'lifetime-original-proof.json').write_text(json.dumps(dict(status='PASS_SCRATCH',proof=proof),indent=2));print('LIFETIME_ORIGINAL_EXACT',len(proof))
