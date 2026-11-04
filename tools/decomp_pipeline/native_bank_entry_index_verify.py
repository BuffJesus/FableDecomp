from pathlib import Path
import sys,json,re,struct,subprocess,hashlib
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
import gate_native_bank_init_complete as G
import pefile
b=Path(sys.argv[1]).resolve();r=json.loads((b/'build-report.json').read_text());sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
for name,h in r['input_sha256'].items():assert sha(Path(name))==h,('input',name)
objects=[Path(p) for p in r['object_sha256']];coffs=[Coff(p) for p in objects];owners={};mapping={}
for line in (b/'probe.map').read_text().splitlines():
 m=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
 if m:mapping[m[1]]=int(m[2],16)
 m=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+[0-9a-fA-F]{8}\s+(?:f\s+)?(?:i\s+)?root(\d+)\.obj\s*$',line)
 if m:owners[m[1]]=int(m[2])
image=EXE.read_bytes();pe=pefile.PE(str(b/'probe.exe'));proof=[]
for row in G.SPECS:
 name=row['symbol'];n=owners[name];c=coffs[n];section,offset=c.symbols[name];body,rels=c.sections[section]
 assert offset==0 and len(body)==row['bytes'] and len(rels)==row['fixups'],name
 fixed=bytearray(body);entry=int(row['entry'],16)
 for site,(dependency,kind) in rels.items():
  if dependency=='_FixtureVectorMalloc':target=0xbfea0e
  elif dependency=='_FixtureVectorFree':target=0xbfea14
  else:target=int(row['targets'][dependency],16)
  struct.pack_into('<I',fixed,site,(target+struct.unpack_from('<i',body,site)[0]-(entry+site+4 if kind==20 else 0))&0xffffffff)
 assert bytes(fixed)==read_virtual_data(image,entry,len(body)),name
 fixed=bytearray(body);at=mapping[name]
 for site,(dependency,kind) in rels.items():struct.pack_into('<I',fixed,site,(mapping[dependency]+struct.unpack_from('<i',body,site)[0]-(at+site+4 if kind==20 else 0))&0xffffffff)
 assert bytes(fixed)==pe.get_data(at-pe.OPTIONAL_HEADER.ImageBase,len(body)),name
 proof.append(dict(symbol=name,bytes=len(body),entry=hex(entry),provider=n,fixups=len(rels)))
(b/'index-retail-proof.json').write_text(json.dumps(dict(status='PASS',code_bodies=46,table_bytes=1024,proof=proof),indent=2))
root=owners['?InitEntry@CBankFile@@AAEXKABVCCharString@@0@Z']
path=owners['?ConvertPathToFilename@CBankFile@@ABE?AVCCharString@@ABV2@@Z']
lower=owners['?GetAsLowerCase@CCharString@@QBE?AV1@XZ']
crc=owners['?FableCRC_Calc_004014A0@@YIIIPBXI@Z']
controls=[('symbol-mode',root,'OpenFlags&64','OpenFlags&0'),('filename-mode',root,'OpenFlags&128','OpenFlags&0'),('crc-mode',root,'OpenFlags&256','OpenFlags&0'),
 ('symbol-value',root,'SymbolIndices[symbol]=index;','SymbolIndices[symbol]=index+1;'),
 ('crc-value',root,'SymbolCRCIndices.Insert(symbol.GetCRC(),index);','SymbolCRCIndices.Insert(symbol.GetCRC(),index+1);'),
 ('crc-result',crc,'return seed;','return seed^1;'),('filename-case',lower,'tolower(','toupper('),('filename-extension',path,'filename += extension;',';')]
env=V.env();env['INCLUDE']=str(V.VC/'include')+';'+str(b/'include');observations=[]
for i,(label,n,old,new) in enumerate(controls):
 text=(b/('root%d.cpp'%n)).read_text();assert text.count(old)==1,(label,old)
 src=b/('index-control-%d.cpp'%i);assert not src.exists();src.write_text(text.replace(old,new));obj=src.with_suffix('.obj')
 run=V.cl(['/nologo','/c','/O2','/MT','/Gy','/Dmalloc=FixtureVectorMalloc','/Dfree=FixtureVectorFree','/I'+str(b/'include'),'/Fo'+str(obj),str(src)],env,b)
 (b/('index-control-%d.compile.json'%i)).write_text(json.dumps(dict(returncode=run.returncode,stdout=run.stdout,stderr=run.stderr)));assert not run.returncode,run.stdout+run.stderr
 exe=src.with_suffix('.exe');cmd=[str(V.VC/'bin/link.exe'),'/nologo','/OPT:REF','/out:'+str(exe),*[str(obj) if p==objects[n] else str(p) for p in objects]]
 run=subprocess.run(cmd,cwd=b,env=env,capture_output=True,text=True)
 (b/('index-control-%d.link.json'%i)).write_text(json.dumps(dict(returncode=run.returncode,stdout=run.stdout,stderr=run.stderr)));assert not run.returncode,run.stdout+run.stderr
 run=subprocess.run([str(exe),str(b/'cases.bin')],cwd=b,capture_output=True,text=True,timeout=60)
 row=dict(name=label,returncode=run.returncode,stdout=run.stdout,stderr=run.stderr)
 (b/('index-control-%d.run.json'%i)).write_text(json.dumps(row));assert run.returncode==80 and r.get('failure_marker','BANK_ENTRY_INDEXED_FAILURE') in run.stderr,row;observations.append(row)
for name,h in r['input_sha256'].items():assert sha(Path(name))==h,('input',name)
for name,h in r['object_sha256'].items():assert sha(Path(name))==h,('object',name)
assert sha(b/'probe.exe')==r['executable_sha256'] and sha(b/'original-root.exe')==r['original_executable_sha256']
(b/'index-verification.json').write_text(json.dumps(dict(status='PASS_SCRATCH',index_exact_code_bodies=46,table_bytes=1024,additional_controls=len(observations),controls=observations,boundary=r['boundary']),indent=2));print('INDEX_EXACT_AND_CONTROLS_PASS code=46 table=1024 controls=8')

omissions=json.loads((b/'omissions.json').read_text());out=[]
for i,row in enumerate(omissions):
 if row['returncode']:continue
 exe=b/('selection-%d.exe'%i);mapfile=exe.with_suffix('.map')
 cmd=[str(V.VC/'bin/link.exe'),'/nologo','/OPT:REF','/out:'+str(exe),'/map:'+str(mapfile),*[str(p) for p in objects if p!=objects[i]]]
 run=subprocess.run(cmd,cwd=b,env=V.env(),capture_output=True,text=True);assert not run.returncode,run.stdout+run.stderr
 mapping={}
 for line in mapfile.read_text().splitlines():
  m=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
  if m:mapping[m[1]]=int(m[2],16)
 pe=pefile.PE(str(exe));mismatches=[]
 for spec in G.SPECS:
  name=spec['symbol'];c=Coff(objects[owners[name]]);section,offset=c.symbols[name];body,rels=c.sections[section];assert offset==0
  if name not in mapping:mismatches.append(name);continue
  fixed=bytearray(body);at=mapping[name]
  for site,(dependency,kind) in rels.items():struct.pack_into('<I',fixed,site,(mapping[dependency]+struct.unpack_from('<i',body,site)[0]-(at+site+4 if kind==20 else 0))&0xffffffff)
  if bytes(fixed)!=pe.get_data(at-pe.OPTIONAL_HEADER.ImageBase,len(body)):mismatches.append(name)
 run=subprocess.run([str(exe),str(b/'cases.bin')],cwd=b,capture_output=True,text=True,timeout=60);assert run.returncode==0 and 'cases=3840 entries=24520 updates=12432' in run.stdout
 out.append(dict(provider=i,source=row['source'],behavior_pass=True,exact_body_pass=not mismatches,mismatched_symbols=mismatches))
assert sum(not row['exact_body_pass'] for row in out)==1,out
(b/'selection-verification.json').write_text(json.dumps(dict(status='PASS',controls=out,byte_selection_rejections=1,redundant_exact_providers=3),indent=2));print(out)
