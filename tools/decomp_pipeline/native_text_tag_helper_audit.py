from pathlib import Path
import sys,json,struct,capstone
P=Path(__file__).resolve().parent;R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'));from gate_game_component_abi import Coff;from pe_oracle import EXE,read_virtual_data
b=Path(sys.argv[1]);r=json.loads((b/'composition.json').read_text());c=Coff(Path(r['source_objects'][str(R/'rebuild/src/compiled/00/9c/NGameText_TagVectors_009caf60.cpp')]));image=EXE.read_bytes();md=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32)
root=next(n for n in c.symbols if n.startswith('?_M_fill_insert@?$vector@VCTextTag'));sec,_=c.symbols[root];body,rels=c.sections[sec];calls=[int(i.op_str,16) for i in md.disasm(read_virtual_data(image,0x9caf60,280),0x9caf60) if i.mnemonic=='call' and i.bytes[0]==0xe8];mapping={};queue=[]
for target,(site,(name,kind)) in zip(calls,sorted(rels.items())):
 assert kind==20
 if name in mapping:assert mapping[name]==target
 mapping[name]=target
 if name in c.symbols:queue.append(name)
mapping[root]=0x9caf60;queue.append(root)
resize=next(n for n in c.symbols if n.startswith('?resize@?$vector@VCTextTag') and n.endswith('QAEXI@Z'));mapping[resize]=0x9cb7d0;queue.append(resize)
known={'??0CCharString@@QAE@XZ':0x99e4b0,'_malloc':0xbfea0e,'_free':0xbfea14,'??0CCharString@@QAE@ABV0@@Z':0x99ec30,'??4CCharString@@QAEAAV0@ABV0@@Z':0x99efb0,'??1CCharString@@QAE@XZ':0x99eae0};mapping.update(known);seen=set();proof=[];gaps=[]
while queue:
 name=queue.pop(0)
 if name in seen:continue
 seen.add(name);address=mapping[name];sec,off=c.symbols[name];body,rels=c.sections[sec];assert off==0;original=read_virtual_data(image,address,len(body));left=bytearray(body);right=bytearray(original)
 for site in rels:left[site:site+4]=right[site:site+4]=bytes(4)
 if left!=right:gaps.append(dict(symbol=name,address=hex(address),bytes=len(body)));continue
 deps=[]
 for site,(dep,kind) in rels.items():
  assert kind==20 and original[site-1] in (0xe8,0xe9)
  target=(address+site+4+struct.unpack_from('<i',original,site)[0])&0xffffffff
  if dep in mapping:assert mapping[dep]==target,(dep,hex(mapping[dep]),hex(target))
  mapping[dep]=target
  if dep in c.symbols:queue.append(dep)
  deps.append(dict(symbol=dep,address=hex(target)))
 proof.append(dict(symbol=name,address=hex(address),bytes=len(body),dependencies=deps))
(b/'tag-helper-proof.json').write_text(json.dumps(dict(exact=proof,gaps=gaps),indent=2));print('TAG_HELPER_EXACT',len(proof));print('GAPS',gaps);assert not gaps
