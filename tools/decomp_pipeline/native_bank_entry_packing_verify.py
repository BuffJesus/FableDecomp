from pathlib import Path
import sys,json,re,struct,subprocess,hashlib
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
import gate_native_bank_pack_runtime as P
import pefile
b=Path(sys.argv[1]).resolve();r=json.loads((b/'build-report.json').read_text());objects=[Path(p) for p in r['object_sha256']];coffs=[Coff(p) for p in objects];owners={};mapping={}
for line in (b/'probe.map').read_text().splitlines():
 m=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
 if m:mapping[m[1]]=int(m[2],16)
 m=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+[0-9a-fA-F]{8}\s+(?:f\s+)?(?:i\s+)?root(\d+)\.obj\s*$',line)
 if m:owners[m[1]]=int(m[2])
pe=pefile.PE(str(b/'probe.exe'));image=EXE.read_bytes();proof=[]
for spec in P.SPECS:
 name=spec['symbol'];n=owners[name];section,offset=coffs[n].symbols[name];body,rels=coffs[n].sections[section];assert not offset and len(body)==spec['bytes'] and len(rels)==spec['fixups'],name
 entry=int(spec['entry'],16);fixed=bytearray(body)
 for site,(dependency,kind) in rels.items():struct.pack_into('<I',fixed,site,(int(spec['targets'][dependency],16)+struct.unpack_from('<i',body,site)[0]-(entry+site+4 if kind==20 else 0))&0xffffffff)
 assert bytes(fixed)==read_virtual_data(image,entry,len(body)),name
 proof.append(dict(symbol=name,bytes=len(body),entry=hex(entry),provider=n,fixups=len(rels)))
getters={'?GetEntryDataType@CBankFile@@QBEKK@Z':0x9cc210,'?GetEntryDataSize@CBankFile@@QBEKK@Z':0x9cc240,'?GetEntryDataOffset@CBankFile@@QBEKK@Z':0x9cc270,'?EntryExists@CBankFile@@QBE_NK@Z':0x9cc190,'?CountEntries@CBankFile@@QBEKXZ':0x9cc3b0,'?GetTotalDataSize@CBankFile@@QBEKXZ':0x9cc450}
for name,entry in getters.items():
 n=owners[name];section,offset=coffs[n].symbols[name];body,rels=coffs[n].sections[section];assert not offset;fixed=bytearray(body)
 for site,(dependency,kind) in rels.items():
  assert dependency=='??ACPackedUIntArray@@QBEKK@Z' and kind==20,(name,dependency,kind)
  struct.pack_into('<I',fixed,site,(0xa629f0+struct.unpack_from('<i',body,site)[0]-(entry+site+4))&0xffffffff)
 assert bytes(fixed)==read_virtual_data(image,entry,len(body)),name
 proof.append(dict(symbol=name,bytes=len(body),entry=hex(entry),provider=n,fixups=len(rels)))
for row in proof:
 name=row['symbol'];n=row['provider'];section,offset=coffs[n].symbols[name];body,rels=coffs[n].sections[section];fixed=bytearray(body);at=mapping[name]
 for site,(dependency,kind) in rels.items():struct.pack_into('<I',fixed,site,(mapping[dependency]+struct.unpack_from('<i',body,site)[0]-(at+site+4 if kind==20 else 0))&0xffffffff)
 assert bytes(fixed)==pe.get_data(at-pe.OPTIONAL_HEADER.ImageBase,len(body)),name
(b/'packing-retail-proof.json').write_text(json.dumps(dict(status='PASS',code_bodies=14,table_bytes=132,proof=proof),indent=2))
pack=owners['?PackRuntimeData@CBankFile@@AAEXXZ'];get=owners['??ACPackedUIntArray@@QBEKK@Z'];dtype=owners['?GetEntryDataType@CBankFile@@QBEKK@Z'];count=owners['?CountEntries@CBankFile@@QBEKXZ']
controls=[('first-record',pack,'&RuntimeData[1]','&RuntimeData[0]'),('representation',pack,'packed < unpacked','packed > unpacked'),('type-width',pack,'&data->DataType, 8,','&data->DataType, 4,'),('valid-width',pack,'&data->Valid, 8,','&data->Valid, 0,'),('clear-type',pack,'ClearPackedBankArray(&PackedDataType);',';'),('decoded-value',get,'+ Bias;','+ Bias + 1;'),('type-query',dtype,'return RuntimeData[index].DataType;','return RuntimeData[index].Valid;'),('entry-count',count,'++count;','count+=2;'),('total-size',count,'total += GetEntryDataSize(index);','total += GetEntryDataSize(index)+1;')]
env=V.env();env['INCLUDE']=str(V.VC/'include')+';'+str(b/'include');observations=[]
for i,(label,n,old,new) in enumerate(controls):
 text=(b/('root%d.cpp'%n)).read_text();assert text.count(old)==1,(label,old)
 src=b/('packing-control-%d.cpp'%i);assert not src.exists();src.write_text(text.replace(old,new));obj=src.with_suffix('.obj')
 run=V.cl(['/nologo','/c','/O2','/MT','/Gy','/Dmalloc=FixtureVectorMalloc','/Dfree=FixtureVectorFree','/I'+str(b/'include'),'/Fo'+str(obj),str(src)],env,b)
 (b/('packing-control-%d.compile.json'%i)).write_text(json.dumps(dict(returncode=run.returncode,stdout=run.stdout,stderr=run.stderr)));assert not run.returncode,run.stdout+run.stderr
 exe=src.with_suffix('.exe');cmd=[str(V.VC/'bin/link.exe'),'/nologo','/OPT:REF','/out:'+str(exe),*[str(obj) if p==objects[n] else str(p) for p in objects]]
 run=subprocess.run(cmd,cwd=b,env=env,capture_output=True,text=True)
 (b/('packing-control-%d.link.json'%i)).write_text(json.dumps(dict(returncode=run.returncode,stdout=run.stdout,stderr=run.stderr)));assert not run.returncode,run.stdout+run.stderr
 run=subprocess.run([str(exe),str(b/'cases.bin')],cwd=b,capture_output=True,text=True,timeout=60)
 row=dict(name=label,returncode=run.returncode,stdout=run.stdout,stderr=run.stderr)
 (b/('packing-control-%d.run.json'%i)).write_text(json.dumps(row));assert run.returncode==80 and r.get('failure_marker','BANK_ENTRY_PACKED_FAILURE') in run.stderr,row;observations.append(row)
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
for name,h in r['input_sha256'].items():assert sha(Path(name))==h,('input',name)
for name,h in r['object_sha256'].items():assert sha(Path(name))==h,('object',name)
assert sha(b/'probe.exe')==r['executable_sha256'] and sha(b/'original-root.exe')==r['original_executable_sha256']
(b/'packing-verification.json').write_text(json.dumps(dict(status='PASS_SCRATCH',code_bodies=14,table_bytes=132,additional_controls=9,controls=observations),indent=2));print('PACKING_EXACT_AND_CONTROLS_PASS code=14 table=132 controls=9')
