from pathlib import Path
from collections import Counter
import sys,json,hashlib,capstone,pefile,struct
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
from near_match import compare,retail_relocations
B=Path(sys.argv[1]).resolve();obj=Path(sys.argv[2]).resolve();c=Coff(obj);sec,off=c.symbols['?ReadFileInformation@CBankFile@@AAE_N_N@Z'];body,relocations=c.sections[sec];raw=EXE.read_bytes();pe=pefile.PE(data=raw);native=read_virtual_data(raw,0x9d0880,1635);expected=Counter();literals={};md=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);md.detail=True;built_immediates={i.address+i.imm_offset for i in md.disasm(body,0) if i.imm_size==4 and not i.group(capstone.CS_GRP_JUMP) and not i.group(capstone.CS_GRP_CALL)}
for site,(name,kind) in relocations.items():
 if not name.startswith('??_C@_0') or site not in built_immediates:continue
 assert kind==6;section,offset=c.symbols[name];data=c.sections[section][0][offset:];data=data[:data.index(b'\0')+1];literals[data]=name;expected[data]+=1
md=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);md.detail=True
def audit(image,code):
 actual=Counter();sites=[];proof=[]
 for ins in md.disasm(code,0x9d0880):
  if ins.imm_size!=4 or ins.group(capstone.CS_GRP_JUMP) or ins.group(capstone.CS_GRP_CALL):continue
  for op in ins.operands:
   if op.type!=capstone.x86.X86_OP_IMM:continue
   target=op.imm&0xffffffff;section=pe.get_section_by_rva(target-pe.OPTIONAL_HEADER.ImageBase)
   if section is None or section.Characteristics&0xa0000000:continue
   matches=[data for data in literals if read_virtual_data(image,target,len(data))==data]
   if not matches:continue
   assert len(matches)==1;data=matches[0];actual[data]+=1;site=ins.address-0x9d0880+ins.imm_offset;sites.append(site);proof.append(dict(site=site,instruction=hex(ins.address),target=hex(target),literal=data[:-1].decode('ascii'),sha256=hashlib.sha256(data).hexdigest(),section=section.Name.rstrip(b'\0').decode()))
 assert actual==expected,(actual,expected)
 return sites,proof
sites,proof=audit(raw,native);raw_grade=compare(body,native,tuple(relocations),retail_relocations('009d0880',1635)).to_dict();proven_grade=compare(body,native,tuple(relocations),sites).to_dict();controls=[]
for row in proof:
 bad=bytearray(native);struct.pack_into('<I',bad,row['site'],0x400000);rejected=False
 try:audit(raw,bytes(bad))
 except AssertionError:rejected=True
 assert rejected;controls.append(dict(site=row['site'],name='literal-target-to-PE-header',rejected=True))
# A permutation preserves the literal multiset, so data proof alone must not
# claim semantic equivalence. The independent throw-message oracle rejects it.
sys.path.insert(0,str(R/'rebuild/tests/integration'));import NativeBankHeaderThrow_model as THROW
args=(1,1,64,32,1,1,False,7,19,32,1);THROW.code=native;THROW.execute(args)
first=next(x for x in proof if x['literal']==' is not a valid resource file!');second=next(x for x in proof if x['literal']==' has the wrong bank format! (should be ');swapped=bytearray(native);struct.pack_into('<I',swapped,first['site'],int(second['target'],16));struct.pack_into('<I',swapped,second['site'],int(first['target'],16));audit(raw,bytes(swapped));THROW.code=bytes(swapped);rejected=False
try:THROW.execute(args)
except AssertionError:rejected=True
finally:THROW.code=native
assert rejected;semantic_control=dict(name='two-valid-literal-targets-swapped',data_multiset_preserved=True,semantic_oracle_rejected=True)
report=dict(status='PASS_LITERAL_CORRESPONDENCE_ONLY',raw_grade=raw_grade,literal_verified_grade=proven_grade,proof=proof,controls=controls,semantic_control=semantic_control,original_sha256=hashlib.sha256(raw).hexdigest(),object_sha256=hashlib.sha256(obj.read_bytes()).hexdigest(),boundary='Raw score retained. Existing compare API unchanged. Additional original pointer sites derive only from exact fullNUL-terminated COFF literal bytes in nonwritable/nonexecutable retail data, with exact occurrence counts; code constants, RTTI and other unproved pointers remain unmasked. Target faults rejected per site. Not source promotion or full behavior proof.')
(B/'literal-correspondence.json').write_text(json.dumps(report,indent=2));print('LITERAL_CORRESPONDENCE',len(proof),'CONTROLS',len(controls),'RAW',raw_grade,'PROVEN_LITERAL',proven_grade)
