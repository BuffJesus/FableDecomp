from pathlib import Path
import sys,json,re,subprocess
import pefile
sys.path.insert(0,'tools/decomp_pipeline');from gate_game_component_abi import Coff
b=Path(sys.argv[1]).resolve();sys.path.insert(0,str(Path(__file__).resolve().parents[2]/'rebuild/tests/integration'));from NativeBankVectorResize_model import SPECS
specs=SPECS;mapping={}
for line in (b/'probe.map').read_text().splitlines():
 m=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
 if m:mapping[m[1]]=int(m[2],16)
raw=(b/'probe.exe').read_bytes();pe=pefile.PE(data=raw);records=[]
for spec in specs:
 start=pe.get_offset_from_rva(mapping[spec['symbol']]-pe.OPTIONAL_HEADER.ImageBase);branch={'runtime':0x9d4033,'updates':0x9d3e06,'checksums':0x464945,'symbols':0x49b781}[spec['name']]-spec['address'];probes=[('branch',branch,b'\x72')];c=Coff(b/(spec['name']+'.obj'));s,o=c.symbols[spec['symbol']];fill=next(site for site,(name,kind) in c.sections[s][1].items() if name.startswith('?_M_fill_insert'));probes.append(('fill',fill-1,b'\x83\xc4\x0c\x90\x90'))
 if spec['name']=='symbols':probes.append(('cleanup',89,b'\x90'*5))
 if spec['name']=='runtime':probes.append(('finish-offset',0x9d404d-spec['address'],b'\x08'))
 for label,offset,replacement in probes:
  changed=bytearray(raw);changed[start+offset:start+offset+len(replacement)]=replacement;exe=b/('control-%d.exe'%len(records));assert not exe.exists();exe.write_bytes(changed);run=subprocess.run([str(exe)],cwd=b,capture_output=True,text=True,timeout=15);record=dict(name=spec['name']+'-'+label,returncode=run.returncode,stdout=run.stdout,stderr=run.stderr);(b/('control-%d.run.json'%len(records))).write_text(json.dumps(record,indent=2)+'\n');assert run.returncode==80 and 'VECTOR_RESIZE_FAILURE' in run.stderr,record;records.append(record['name'])
p=b/'native-controls.json';assert not p.exists();p.write_text(json.dumps(dict(status='PASS',controls=records),indent=2)+'\n');print('BANK_VECTOR_RESIZE_NATIVE_CONTROLS_PASS',len(records))
