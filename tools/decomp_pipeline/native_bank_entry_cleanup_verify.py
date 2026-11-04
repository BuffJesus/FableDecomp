from pathlib import Path
import sys,json,re,struct,subprocess,hashlib
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
import bank_lifetime_host_evidence as L
b=Path(sys.argv[1]).resolve();r=json.loads((b/'build-report.json').read_text());objects=[Path(p) for p in r['object_sha256']];owners={};mapping={}
for line in (b/'probe.map').read_text().splitlines():
 m=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
 if m:mapping[m[1]]=int(m[2],16)
 m=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+[0-9a-fA-F]{8}\s+(?:f\s+)?(?:i\s+)?root(\d+)\.obj\s*$',line)
 if m:owners[m[1]]=int(m[2])
proof=[];image=EXE.read_bytes();targets=dict(L.ADDRESS)
targets['_FixtureVectorFree']=0xbfea14
for _,_,name,entry,size in L.SPECS:targets[name]=entry
targets['?DeallocateMemory@?$CArrayVector@VCCharString@@@@QAEXXZ']=0x9d3fb0
for spec in (L.SPECS[2],L.SPECS[11]):
 _,_,name,entry,size=spec;n=owners[name];c=Coff(objects[n]);section,offset=c.symbols[name];body,rels=c.sections[section];assert not offset and len(body)==size
 fixed=bytearray(body);dependencies=[];original=read_virtual_data(image,entry,size)
 for site,(dependency,kind) in rels.items():
  addend=struct.unpack_from('<i',body,site)[0];target=(struct.unpack_from('<I',original,site)[0]-addend+(entry+site+4 if kind==20 else 0))&0xffffffff
  if dependency in targets:assert target==targets[dependency],(dependency,hex(target),hex(targets[dependency]))
  else:raise AssertionError(('unknown dependency',dependency,hex(target)))
  struct.pack_into('<I',fixed,site,(target+addend-(entry+site+4 if kind==20 else 0))&0xffffffff)
  dependencies.append(dict(symbol=dependency,target=hex(target)))
 assert bytes(fixed)==original,name
 proof.append(dict(symbol=name,bytes=size,entry=hex(entry),dependencies=dependencies))
close=owners[L.SPECS[2][2]]
controls=[('update-delete','delete *p;',';'),('update-array','ReleaseArray(self->UpdateData);',';'),('runtime-array','ReleaseArray(self->RuntimeData);',';'),('symbol-array','ReleaseBankSymbols(&self->Symbols);',';'),('symbol-map','self->SymbolIndices.clear();',';'),('crc-map','self->SymbolCRCIndices.clear();',';'),('filename-map','self->FilenameIndices.clear();',';'),('packed-offset','ClearPackedBankArray(&self->PackedDataOffset);',';'),('packed-size','ClearPackedBankArray(&self->PackedDataSize);',';'),('packed-type','ClearPackedBankArray(&self->PackedDataType);',';'),('packed-valid','ClearPackedBankArray(&self->PackedValid);',';'),('size-reset','self->Size = 0;','self->Size = 1;')]
env=V.env();env['INCLUDE']=str(V.VC/'include')+';'+str(b/'include');observations=[]
for i,(label,old,new) in enumerate(controls):
 text=(b/('root%d.cpp'%close)).read_text();assert text.count(old)==1
 src=b/('cleanup-control-%d.cpp'%i);assert not src.exists();src.write_text(text.replace(old,new));obj=src.with_suffix('.obj')
 run=V.cl(['/nologo','/c','/O2','/MT','/Gy','/Dmalloc=FixtureVectorMalloc','/Dfree=FixtureVectorFree','/I'+str(b/'include'),'/Fo'+str(obj),str(src)],env,b)
 (src.with_suffix('.compile.json')).write_text(json.dumps(dict(returncode=run.returncode,stdout=run.stdout,stderr=run.stderr)));assert not run.returncode,run.stdout+run.stderr
 exe=src.with_suffix('.exe');run=subprocess.run([str(V.VC/'bin/link.exe'),'/nologo','/OPT:REF','/out:'+str(exe),*[str(obj) if p==objects[close] else str(p) for p in objects]],cwd=b,env=env,capture_output=True,text=True)
 src.with_suffix('.link.json').write_text(json.dumps(dict(returncode=run.returncode,stdout=run.stdout,stderr=run.stderr)));assert not run.returncode,run.stdout+run.stderr
 run=subprocess.run([str(exe),str(b/'cases.bin')],cwd=b,capture_output=True,text=True,timeout=60)
 row=dict(name=label,returncode=run.returncode,stdout=run.stdout,stderr=run.stderr);src.with_suffix('.run.json').write_text(json.dumps(row));assert run.returncode==80 and r.get('failure_marker','BANK_ENTRY_CLOSED_FAILURE') in run.stderr,row;observations.append(row)
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
for name,h in r['input_sha256'].items():assert sha(Path(name))==h,('input',name)
for name,h in r['object_sha256'].items():assert sha(Path(name))==h,('object',name)
assert sha(b/'probe.exe')==r['executable_sha256'] and sha(b/'original-root.exe')==r['original_executable_sha256']
(b/'cleanup-verification.json').write_text(json.dumps(dict(status='PASS_SCRATCH',proof=proof,controls=observations),indent=2));print('CLEANUP_EXACT_AND_CONTROLS_PASS bodies=2 controls=12')
