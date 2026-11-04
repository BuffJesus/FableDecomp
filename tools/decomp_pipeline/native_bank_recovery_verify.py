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
for file in ('Fixture.cpp','NativeBankRecoveryColdAliases.h'):
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
symbol='?ReadRecoveryFile@CBankFile@@AAE_NK@Z';c=Coff(b/'provider9.obj');sec,off=c.symbols[symbol];body,rels=c.sections[sec]
sys.path.insert(0,str(Path(__file__).resolve().parents[2]/'rebuild/tests/integration'))
from NativeBankRecovery_model import compare
model=compare(b/'provider9.obj');assert model['bytes']==679 and model['cases']==432
patched=bytearray(body);entry=mapping[symbol]
for site,(name,kind) in rels.items():struct.pack_into('<I',patched,site,(resolve(name)+struct.unpack_from('<i',body,site)[0]-(entry+site+4 if kind==20 else 0))&0xffffffff)
assert bytes(patched)==pe.get_data(entry-pe.OPTIONAL_HEADER.ImageBase,len(body))
from native_exact_roots import import_names
imports={name:address for address,name in import_names(image).items()}
helper_targets={'??0CCharString@@QAE@PBDJ@Z':0x99ebf0,'??0CCharString@@QAE@ABV0@@Z':0x99ec30,'??BCCharString@@QBEPBDXZ':0x99e4c0,'??1CCharString@@QAE@XZ':0x99eae0,'__TI2?AVCBBBFileException@@':0x1369380,'__TI1?AVCBBBFileException@@':0x1369380,'__CxxThrowException@8':0xbfeb84}
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
command=json.loads((b/'link.json').read_text())['command'];omissions=[];redundant=[]
for index in range(14):
 obj=b/('provider%d.obj'%index);exe=b/('omit%d.exe'%index);cmd=['/out:'+str(exe) if s.startswith('/out:') else s for s in command if s!=str(obj) and not s.startswith('/map:')];result=subprocess.run(cmd,cwd=b,env=V.env(),capture_output=True,text=True,timeout=30)
 p=b/('omit%d.link.json'%index);assert not p.exists();p.write_text(json.dumps(dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr),indent=2)+'\n');(omissions if result.returncode else redundant).append(index)
 if index!=2:assert result.returncode and ('LNK2001' in result.stdout or 'LNK2019' in result.stdout),index
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
p=b/'verification.json';assert not p.exists();p.write_text(json.dumps(dict(status='PASS_SCRATCH',original_stream_bodies=13,linked_exact_stream_bodies=8,disk_bodies=disk,recovery_bytes=len(body),recovery_classification='MATCH',model=model,recovery_fixups=len(rels),omissions=omissions,redundant=redundant,objects={str(b/('provider%d.obj'%i)):sha(b/('provider%d.obj'%i)) for i in range(15)},headers={str(p):sha(p) for p in (b/'include').rglob('*.h')},executable_sha256=sha(b/'probe.exe'),boundary='192 native cases; native stream and disk Read/Seek/Position/Close. Path, existence, disk Open handle adoption, strings and entry-list decoding controlled. Normal cleanup only; no exception unwinding or production acceptance.'),indent=2)+'\n');print('RECOVERY_DISK_FINAL_VERIFY_PASS streams=8 disk=4 recovery=679 omissions=13')
