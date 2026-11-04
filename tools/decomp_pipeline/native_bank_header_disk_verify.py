from pathlib import Path
import sys,json,re,struct,hashlib,subprocess
import pefile
sys.path.insert(0,'tools/decomp_pipeline')
import check_one as V
from gate_game_component_abi import Coff
from file_input_stream_sources import ROOTS,TARGETS
from pe_oracle import EXE,read_virtual_data
b=Path(sys.argv[1]).resolve();mapping={};aliases={}
for line in (b/'probe.map').read_text().splitlines():
 m=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
 if m:mapping[m[1]]=int(m[2],16)
for file in ('Fixture.cpp','cold-aliases.h'):
 for name,target in re.findall(r'/alternatename:([^=]+)=([^"\s]+)',(b/file).read_text()):aliases[name]=target
def resolve(name,seen=()):
 if name in mapping:return mapping[name]
 assert name not in seen and name in aliases,name
 return resolve(aliases[name],seen+(name,))
pe=pefile.PE(str(b/'probe.exe'));image=EXE.read_bytes();active=[];discarded=[]
for index,symbol,address,size in ROOTS:
 c=Coff(b/('provider%d.obj'%index));sec,off=c.symbols[symbol];body,rels=c.sections[sec];assert not off and len(body)==size;patched=bytearray(body)
 for site,(name,kind) in rels.items():struct.pack_into('<I',patched,site,(TARGETS[name]+struct.unpack_from('<i',body,site)[0]-(address+site+4 if kind==20 else 0))&0xffffffff)
 assert bytes(patched)==read_virtual_data(image,address,size),symbol
 if symbol not in mapping:discarded.append(symbol);continue
 patched=bytearray(body);entry=mapping[symbol]
 for site,(name,kind) in rels.items():struct.pack_into('<I',patched,site,(resolve(name)+struct.unpack_from('<i',body,site)[0]-(entry+site+4 if kind==20 else 0))&0xffffffff)
 assert bytes(patched)==pe.get_data(entry-pe.OPTIONAL_HEADER.ImageBase,size);active.append(symbol)
assert len(active)==8 and len(discarded)==5
symbol='?ReadFileInformation@CBankFile@@AAE_N_N@Z';c=Coff(b/'provider9.obj');sec,off=c.symbols[symbol];body,rels=c.sections[sec]
prior=Coff(Path(sys.argv[2]).resolve());ps,po=prior.symbols[symbol];assert not off and not po and c.sections[sec]==prior.sections[ps]
patched=bytearray(body);entry=mapping[symbol]
for site,(name,kind) in rels.items():struct.pack_into('<I',patched,site,(resolve(name)+struct.unpack_from('<i',body,site)[0]-(entry+site+4 if kind==20 else 0))&0xffffffff)
assert bytes(patched)==pe.get_data(entry-pe.OPTIONAL_HEADER.ImageBase,len(body))
ctor='??0CBankFileException@@QAE@VCCharString@@W4EBankFileExceptionType@0@@Z';co=Coff(b/'provider9.obj');cs,offset=co.symbols[ctor];cb,cr=co.sections[cs];assert not offset and len(cb)==33;fixed=bytearray(cb);start=mapping[ctor]
for site,(name,kind) in cr.items():struct.pack_into('<I',fixed,site,(resolve(name)+struct.unpack_from('<i',cb,site)[0]-(start+site+4 if kind==20 else 0))&0xffffffff)
assert bytes(fixed)==pe.get_data(start-pe.OPTIONAL_HEADER.ImageBase,len(cb))
ctor_targets={'??BCCharString@@QBEPBDXZ':0x99e4c0,'??1CCharString@@QAE@XZ':0x99eae0};ctor_original=bytearray(cb)
for site,(name,kind) in cr.items():
 target=ctor_targets[name];assert kind==20;struct.pack_into('<I',ctor_original,site,(target-(0x9d1e70+site+4))&0xffffffff)
assert bytes(ctor_original)==read_virtual_data(image,0x9d1e70,33)
from native_exact_roots import import_names
imports={name:address for address,name in import_names(image).items()}
helper_targets={'??0CCharString@@QAE@PBDJ@Z':0x99ebf0,'??0CCharString@@QAE@ABV0@@Z':0x99ec30,'??BCCharString@@QBEPBDXZ':0x99e4c0,'??1CCharString@@QAE@XZ':0x99eae0,'__TI1?AVCBBBFileException@@':0x1369380,'__TI2?AVCBBBFileException@@':0x1369380,'__CxxThrowException@8':0xbfeb84}
disk=[]
for index,symbol,address,size in [(10,'?Read@CDiskFileWin32@@UAEXPAXJPAVCASuspendableProcess@@@Z',0x99a960,290),(11,'?SetPosition@CDiskFileWin32@@UAEXJ@Z',0x99abf0,159),(12,'?GetPosition@CDiskFileWin32@@UBEJXZ',0x99abd0,24),(13,'?Close@CDiskFileWin32@@UAEXXZ',0x99a920,52)]:
 co=Coff(b/('provider%d.obj'%index));section,offset=co.symbols[symbol];cb,cr=co.sections[section];assert not offset and len(cb)==size;original=read_virtual_data(image,address,size);fixed=bytearray(cb)
 for site,(name,kind) in cr.items():
  addend=struct.unpack_from('<i',cb,site)[0]
  if name.startswith('__imp__'):target=imports[name[7:].split('@')[0]]
  elif name.startswith('??_C@'):
   target=(struct.unpack_from('<I',original,site)[0]-addend)&0xffffffff;ds,do=co.symbols[name];literal=co.sections[ds][0][do:];assert read_virtual_data(image,target,len(literal))==literal
  else:target=helper_targets[name]
  struct.pack_into('<I',fixed,site,(target+addend-(address+site+4 if kind==20 else 0))&0xffffffff)
 assert bytes(fixed)==original,symbol
 fixed=bytearray(cb);entry=mapping[symbol]
 for site,(name,kind) in cr.items():struct.pack_into('<I',fixed,site,(resolve(name)+struct.unpack_from('<i',cb,site)[0]-(entry+site+4 if kind==20 else 0))&0xffffffff)
 assert bytes(fixed)==pe.get_data(entry-pe.OPTIONAL_HEADER.ImageBase,size);disk.append(dict(symbol=symbol,bytes=size,fixups=len(cr)))
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
report=dict(status='PASS_FRESH_HEADER_DISK_BODY_PROOF',original_stream_bodies=13,linked_exact_stream_bodies=8,disk_bodies=disk,header_bytes=len(body),header_fixups=len(rels),constructor_bytes=33,object_sha256={str(p):sha(p) for p in b.glob('provider*.obj')},header_sha256={str(p):sha(p) for p in Path(sys.argv[3]).rglob('*.h')},executable_sha256=sha(b/'probe.exe'),boundary='64 nonthrow native disk/header cases. Header bytes/relocations equal bounded same-TU reference;13 original stream bodies and8 active final bodies plus4 disk bodies exact. Current canonical header snapshot, controlled strings/entry/recovery/progress and adopted CAFile handle. No source omissions, actual throw/unwind or full production parser claim.')
(b/'body-proof.json').write_text(json.dumps(report,indent=2));print('HEADER_DISK_BODY_PROOF_PASS original_stream=13 linked_stream=8 disk=4 header='+str(len(body)))
