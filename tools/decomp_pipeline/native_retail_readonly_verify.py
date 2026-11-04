"""Check selected native COFF bodies and relocations against the linked PE."""
from pathlib import Path
import json,re,struct,hashlib
import pefile
from gate_game_component_abi import Coff
import bank_begin_worker_link_oracle as banklink

def verify(build,objects,native_objects,host):
 coffs={p.name:Coff(p) for p in objects};mapping={};owners={}
 for line in (build/'probe.map').read_text().splitlines():
  m=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
  if m:mapping[m[1]]=int(m[2],16)
  m=re.match(r'^\s+0001:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\s+f\s+(?:i\s+)?(\S+\.obj)\s*$',line)
  if m and m[3] in native_objects and not m[1].startswith('_$E'):owners[m[1]]=m[3]
 aliases=banklink.aliases(objects,host)
 def resolve(name,seen=()):
  if name in mapping:return mapping[name]
  assert name not in seen and name in aliases,name
  return resolve(aliases[name],seen+(name,))
 pe=pefile.PE(str(build/'probe.exe'));linked=[];bodyaliases=[]
 if '?Open@CThreadedFile@@QAE_NABVCWideString@@_N@Z' in mapping:
  symbol='?Open@CThreadedFile@@QAE_NABVCWideString@@_N@Z';locations=[struct.unpack('<I',pe.get_data(mapping[symbol]-pe.OPTIONAL_HEADER.ImageBase+x,4))[0] for x in (265,275)];assert locations[0]==locations[1];rv=locations[0]-pe.OPTIONAL_HEADER.ImageBase;section=pe.get_section_by_rva(rv);assert section.Characteristics&0x80000000 and (rv-section.VirtualAddress>=section.SizeOfRawData or pe.get_data(rv,4)==b'\0'*4);mapping['_s_NextReadPhysicalSortKey']=locations[0]
 if '?OpenForWrite@CThreadedFile@@QAE_NABVCWideString@@_N@Z' in mapping:
  symbol='?OpenForWrite@CThreadedFile@@QAE_NABVCWideString@@_N@Z';locations=[struct.unpack('<I',pe.get_data(mapping[symbol]-pe.OPTIONAL_HEADER.ImageBase+x,4))[0] for x in (107,117)];assert locations[0]==locations[1] and locations[0]!=mapping.get('_s_NextReadPhysicalSortKey');rv=locations[0]-pe.OPTIONAL_HEADER.ImageBase;section=pe.get_section_by_rva(rv);assert section is not None and section.Characteristics&0x80000000 and not section.Characteristics&0x20000000 and rv+4<=section.VirtualAddress+max(section.Misc_VirtualSize,section.SizeOfRawData) and (rv-section.VirtualAddress>=section.SizeOfRawData or pe.get_data(rv,4)==b'\0'*4);mapping['_s_NextPhysicalSortKey']=locations[0]
 for name,obj in owners.items():
  c=coffs[obj];at=resolve(name)
  if name not in c.symbols:
   assert any(mapping.get(s)==at for s in c.symbols);bodyaliases.append(name);continue
  sec,offset=c.symbols[name];body,rels=c.sections[sec];assert not offset;fixed=bytearray(body)
  for site,(dep,kind) in rels.items():
   target=at+c.symbols[dep][1] if dep.startswith('$L') and c.symbols[dep][0]==sec else resolve(dep)
   assert kind in (6,20);struct.pack_into('<I',fixed,site,(target+struct.unpack_from('<i',body,site)[0]-(at+site+4 if kind==20 else 0))&0xffffffff)
  assert pe.get_data(at-pe.OPTIONAL_HEADER.ImageBase,len(body))==bytes(fixed),name
  linked.append(dict(symbol=name,object=obj,address=hex(at),bytes=len(body)))
 report=dict(status='PASS',linked_symbols=len(linked),unique_bodies=len({x['address'] for x in linked}),linked=linked,aliases=bodyaliases,exe_sha256=hashlib.sha256((build/'probe.exe').read_bytes()).hexdigest());(build/'native-final-proof.json').write_text(json.dumps(report,indent=2));return report
