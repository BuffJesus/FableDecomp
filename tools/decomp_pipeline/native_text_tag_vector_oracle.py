from pathlib import Path
import sys,json,struct,subprocess,capstone,pefile,re
P=Path(__file__).resolve().parent;R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'));from gate_game_component_abi import Coff;from pe_oracle import EXE,read_virtual_data
b=Path(sys.argv[1]);r=json.loads((b/'composition.json').read_text());c=Coff(Path(r['source_objects'][str(R/'rebuild/src/compiled/00/9c/NGameText_TagVectors_009caf60.cpp')]));symbol=next(n for n in c.symbols if n.startswith('?_M_fill_insert@?$vector@VCTextTag'));section,offset=c.symbols[symbol];body,rels=c.sections[section];original=read_virtual_data(EXE.read_bytes(),0x9caf60,280);native_calls=[i for i in capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32).disasm(original,0x9caf60) if i.mnemonic=='call' and i.bytes[0]==0xe8];candidate_calls=[(site,name) for site,(name,kind) in sorted(rels.items()) if kind==20];assert len(native_calls)==len(candidate_calls)==8;targets={};proof=[]
for ins,(site,name) in zip(native_calls,candidate_calls):
 address=int(ins.op_str,16)
 if address in targets:assert targets[address]==name
 targets[address]=name;proof.append(dict(original_site=hex(ins.address),original_target=hex(address),symbol=name))
assert targets[0x99ec30]=='??0CCharString@@QAE@ABV0@@Z' and targets[0x99eae0]=='??1CCharString@@QAE@XZ'
mapping={}
for line in (b/'tag.map').read_text().splitlines():
 m=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
 if m:mapping[m[1]]=int(m[2],16)
raw=(b/'tag.exe').read_bytes();pe=pefile.PE(data=raw);last=pe.sections[-1];align=lambda n,a:(n+a-1)//a*a;rva=align(last.VirtualAddress+max(last.Misc_VirtualSize,last.SizeOfRawData),pe.OPTIONAL_HEADER.SectionAlignment);base=pe.OPTIONAL_HEADER.ImageBase+rva;payload=bytearray(original)
for ins in native_calls:
 name=targets[int(ins.op_str,16)];site=ins.address-0x9caf60+1;struct.pack_into('<I',payload,site,(mapping[name]-base-site-4)&0xffffffff)
header=last.get_file_offset()+40;assert header+40<=pe.OPTIONAL_HEADER.SizeOfHeaders and not any(raw[header:header+40]);start=align(len(raw),pe.OPTIONAL_HEADER.FileAlignment);size=align(len(payload),pe.OPTIONAL_HEADER.FileAlignment);oracle=bytearray(raw);oracle.extend(bytes(start+size-len(oracle)));oracle[start:start+len(payload)]=payload;struct.pack_into('<8sIIIIIIHHI',oracle,header,b'.oracle\0',len(payload),rva,size,start,0,0,0,0,0x60000020);struct.pack_into('<H',oracle,pe.DOS_HEADER.e_lfanew+6,pe.FILE_HEADER.NumberOfSections+1);struct.pack_into('<I',oracle,pe.OPTIONAL_HEADER.get_field_absolute_offset('SizeOfImage'),align(rva+len(payload),pe.OPTIONAL_HEADER.SectionAlignment));at=mapping[symbol];off=pe.get_offset_from_rva(at-pe.OPTIONAL_HEADER.ImageBase);oracle[off:off+5]=b'\xe9'+struct.pack('<I',(base-at-5)&0xffffffff);(b/'tag-original.exe').write_bytes(oracle);run=subprocess.run([str(b/'tag-original.exe')],cwd=b,capture_output=True,text=True,timeout=30);assert not run.returncode and run.stdout==json.loads((b/'tag-run.json').read_text())['stdout'],(run.returncode,run.stdout,run.stderr)
(b/'tag-original-receipt.json').write_text(json.dumps(dict(status='PASS',cases=2268,original_cases=2268,aliases=720,grows=945,allocations=3195,original_calls=proof,boundary='Typed non-POD tag fill withguarded CRT allocation andindependent positions/string model. Candidate dependencies reused byoriginal root;deep helper byte checks separate.'),indent=2));print('TAG_VECTOR_ORIGINAL_PARITY_PASS cases=2268 original=2268')
