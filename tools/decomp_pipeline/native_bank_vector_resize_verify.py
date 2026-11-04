from pathlib import Path
import sys,json,struct,re,hashlib,subprocess
import pefile
sys.path.insert(0,'tools/decomp_pipeline');import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
b=Path(sys.argv[1]).resolve();sys.path.insert(0,str(Path(__file__).resolve().parents[2]/'rebuild/tests/integration'));from NativeBankVectorResize_model import SPECS
specs=SPECS;mapping={}
for line in (b/'probe.map').read_text().splitlines():
 m=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
 if m:mapping[m[1]]=int(m[2],16)
aliases=dict(re.findall(r'/alternatename:([^=]+)=([^"\s]+)',(b/'Fixture.cpp').read_text()))
def resolve(name):return mapping[name] if name in mapping else mapping[aliases[name]]
image=EXE.read_bytes();records=[];pe=pefile.PE(str(b/'probe.exe'))
for spec in specs:
 c=Coff(b/(spec['name']+'.obj'));s,o=c.symbols[spec['symbol']];body,rels=c.sections[s];assert not o and len(body)==spec['bytes'];original=read_virtual_data(image,spec['address'],spec['bytes']);fixed=bytearray(body)
 for site,(name,kind) in rels.items():struct.pack_into('<I',fixed,site,(spec['targets'][name]+struct.unpack_from('<i',body,site)[0]-(spec['address']+site+4 if kind==20 else 0))&0xffffffff)
 assert bytes(fixed)==original,spec['name'];entry=mapping[spec['symbol']];fixed=bytearray(body)
 for site,(name,kind) in rels.items():struct.pack_into('<I',fixed,site,(resolve(name)+struct.unpack_from('<i',body,site)[0]-(entry+site+4 if kind==20 else 0))&0xffffffff)
 assert bytes(fixed)==pe.get_data(entry-pe.OPTIONAL_HEADER.ImageBase,len(body)),spec['name'];records.append(dict(name=spec['name'],symbol=spec['symbol'],address=spec['address'],bytes=len(body),fixups=len(rels)))
helpers=[('runtime','??$__copy@PAUCRuntimeData@CBankFile@@PAU12@H@_STL@@YIPAUCRuntimeData@CBankFile@@PAU12@00ABUrandom_access_iterator_tag@0@PAH@Z',0x9d1c30,74),('checksum-storage','?erase@?$vector@KV?$allocator@K@_STL@@@_STL@@QAEPAKPAK0@Z',0x46496f,52)]
helpers.extend([('checksum-storage','?_M_fill_insert@?$vector@KV?$allocator@K@_STL@@@_STL@@QAEXPAKIABK@Z',0x4649a3,264),('checksum-overflow','??$__uninitialized_copy@K@_STL@@YIPAKPBK0PAKABU__true_type@0@@Z',0x464aab,35),('checksum-overflow','?_M_insert_overflow@?$vector@KV?$allocator@K@_STL@@@_STL@@QAEXPAKABKABU__true_type@2@I_N@Z',0x464ace,188)])
helper_targets={symbol:address for name,symbol,address,size in helpers};helper_targets.update(_malloc=0xbfea0e,_free=0xbfea14,_memmove=0xbfeae6)
for name,symbol,address,size in helpers:
 c=Coff(b/(name+'.obj'));s,o=c.symbols[symbol];body,rels=c.sections[s];assert not o and len(body)==size;fixed=bytearray(body)
 for site,(n,k) in rels.items():struct.pack_into('<I',fixed,site,(helper_targets[n]+struct.unpack_from('<i',body,site)[0]-(address+site+4))&0xffffffff)
 assert bytes(fixed)==read_virtual_data(image,address,size),symbol;entry=mapping[symbol];fixed=bytearray(body)
 for site,(n,k) in rels.items():struct.pack_into('<I',fixed,site,(resolve(n)+struct.unpack_from('<i',body,site)[0]-(entry+site+4))&0xffffffff)
 assert bytes(fixed)==pe.get_data(entry-pe.OPTIONAL_HEADER.ImageBase,size);records.append(dict(name=name+'-helper',symbol=symbol,address=address,bytes=size,fixups=len(rels)))
command=json.loads((b/'link.json').read_text())['command'];omissions=[]
for name in ('runtime','updates','checksums','symbols','checksum-storage','checksum-overflow'):
 cmd=['/out:'+str(b/('omit-'+name+'.exe')) if s.startswith('/out:') else s for s in command if s!=str(b/(name+'.obj')) and not s.startswith('/map:')];r=subprocess.run(cmd,cwd=b,env=V.env(),capture_output=True,text=True);(b/('omit-'+name+'.json')).write_text(json.dumps(dict(returncode=r.returncode,stdout=r.stdout,stderr=r.stderr),indent=2));assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),name;omissions.append(name)
p=b/'verification.json';assert not p.exists();p.write_text(json.dumps(dict(status='PASS_SCRATCH',records=records,omissions=omissions,executable_sha256=hashlib.sha256((b/'probe.exe').read_bytes()).hexdigest(),boundary='Four resize roots, runtime copy and complete five-body checksum vector chain. Other fill operations and string lifetime/erasure controlled; checksum overflow linked but bounded host cases use spare capacity.'),indent=2)+'\n');print('BANK_VECTOR_RESIZE_FINAL_PASS',len(records))
