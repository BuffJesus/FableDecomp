"""Exact retail-byte audit of the newly composed bank/vector allocation closure."""
from pathlib import Path
import sys,json,struct,re,hashlib
R=Path(__file__).resolve().parents[2];sys.path[:0]=[str(R/'tools/decomp_pipeline'),str(R/'rebuild/tests/integration')]
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
import NativeBankAllocate_model as A
import NativeBankVectorResize_model as Z
b=Path(sys.argv[1]).resolve();report=json.loads((b/'build-report.json').read_text());image=EXE.read_bytes();sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
for p,h in report['input_sha256'].items():assert sha(Path(p))==h,('input',p)
for p,h in report['object_sha256'].items():assert sha(Path(p))==h,('object',p)
coffs=[Coff(b/('root%d.obj'%n)) for n in range(len(report['object_sha256'])-1)];mapping={};owners={}
for line in (b/'probe.map').read_text().splitlines():
 m=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
 if m:mapping[m[1]]=int(m[2],16)
 m=re.match(r'^\s+0001:[0-9a-fA-F]{8}\s+(\S+)\s+[0-9a-fA-F]{8}\s+f\s+(?:i\s+)?root(\d+)\.obj\s*$',line)
 if m:owners[m[1]]=int(m[2])
targets=dict(A.TARGETS);targets.update(_FixtureVectorMalloc=0xbfea0e,_FixtureVectorFree=0xbfea14,_malloc=0xbfea0e,_free=0xbfea14,_memmove=0xbfeae6)
for row in Z.SPECS:targets[row['symbol']]=row['address'];targets.update(row['targets'])
roots=[('?AllocateBuffers@CBankFile@@AAEXK@Z',0x9ceae0)]
roots += [(row['symbol'],row['address']) for row in Z.SPECS]
roots += [('?DeallocateMemory@?$CArrayVector@UCRuntimeData@CBankFile@@@@QAEXXZ',0x9d3d90),('?DeallocateMemory@?$CArrayVector@VCCharString@@@@QAEXXZ',0x9d3fb0)]
# Typed homes from the accepted string-vector and STLport units.
starts={'?_M_fill_insert@?$vector@VCCharString@@':0x4333b3,'?_M_insert_overflow@?$vector@VCCharString@@':0x433530,
 '?erase@?$vector@VCCharString@@':0x43336a,'??$uninitialized_fill_n@VCCharString@@':0x433504,
 '??$_Destroy_range@VCCharString@@':0x414e00,'?erase@?$vector@KV':0x46496f,
 '?_M_fill_insert@?$vector@KV':0x4649a3,'??$__uninitialized_copy@K@':0x464aab,
 '?_M_insert_overflow@?$vector@KV':0x464ace,
 '?_M_erase@?$_Rb_tree@VCCharString@@':0x579435}
for prefix,entry in starts.items():
 names=[name for name in owners if name.startswith(prefix)];assert len(names)==1,(prefix,names);roots.append((names[0],entry));targets[names[0]]=entry
new_units={owners[name] for name,entry in roots};new_units.update(owners[name] for name in owners if ('vector@UCRuntimeData@' in name or 'vector@PAVCBankFileEntryUpdateData' in name))
seen=set();queue=list(roots);proof=[];unmatched=[]
while queue:
 name,entry=queue.pop(0)
 if name in seen:continue
 seen.add(name)
 if name not in owners:continue
 n=owners[name]
 if name not in coffs[n].symbols:continue
 section,offset=coffs[n].symbols[name];body,rels=coffs[n].sections[section];assert offset==0
 orig=read_virtual_data(image,entry,len(body));fixed=bytearray(body)
 for site in rels:fixed[site:site+4]=orig[site:site+4]
 if bytes(fixed)!=orig:
  unmatched.append(dict(symbol=name,entry=hex(entry),bytes=len(body),first_difference=next(i for i,(x,y) in enumerate(zip(fixed,orig)) if x!=y)));continue
 dependencies=[]
 for site,(dependency,kind) in rels.items():
  addend=struct.unpack_from('<i',body,site)[0];target=(struct.unpack_from('<I',orig,site)[0]-addend+(entry+site+4 if kind==20 else 0))&0xffffffff
  if dependency in targets:assert targets[dependency]==target,(dependency,hex(target),hex(targets[dependency]))
  else:targets[dependency]=target
  dependencies.append(dict(symbol=dependency,target=hex(target),site=site,kind=kind))
  if kind==20 and dependency in owners and owners[dependency] in new_units:queue.append((dependency,target))
 proof.append(dict(symbol=name,entry=hex(entry),bytes=len(body),provider=n,fixups=dependencies))
out=dict(status='PASS' if not unmatched else 'PARTIAL',exact_bodies=len(proof),proof=proof,unmatched=unmatched,boundary='Known retail homes plus exact instruction bytes and recursively checked dependency addresses. Final linked bodies separately audited by parent probe. No additional behavioral or root credit.')
path=b/'allocation-retail-proof.json';assert not path.exists();path.write_text(json.dumps(out,indent=2));print('EXACT',len(proof),'UNMATCHED',unmatched)
