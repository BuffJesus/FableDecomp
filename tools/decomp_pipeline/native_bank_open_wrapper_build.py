from pathlib import Path
from datetime import datetime,timezone
import sys,json,hashlib,shutil,re,subprocess
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import check_one as V
import gate_native_bank_shared_worker as G
import gate_native_threaded_file_read_paths as P
import bank_file_lifetime_factory_worker_evidence as F
from gate_game_component_abi import Coff
from bank_open_wrapper_sources import PARENT,FIXTURE,EXTRAS
b=Path(sys.argv[1]).resolve();b.mkdir();print(b,flush=True)
report=json.loads(PARENT.read_text());prior=Path(report['build']);sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
objects=[Path(p) for p in report['object_sha256'] if Path(p).name!='fixture.obj'];object_sources={};assert all(sha(p)==report['object_sha256'][str(p)] for p in objects)
inc=b/'include';shutil.copytree(prior/'include',inc)
# Additive bank declarations are checked by recompiling every provider and
# comparing its existing bodies/relocations below, then auditing the final PE.
refresh=[]
for name in ('fable_game_text_bank.h','fable_stlport_vector.h','fable_stlport_tree.h','fable_bank_owned_map.h','fable_small_vector.h','fable_packed_uint_array.h','fable_stlport_algo.h'):
 header=R/'rebuild/include'/name;snapshot=inc/name
 refresh.append(dict(path=str(header),previous_sha256=sha(snapshot),current_sha256=sha(header)))
 snapshot.write_bytes(header.read_bytes())
(b/'header-refresh.json').write_text(json.dumps(refresh,indent=2)+'\n')
extras=EXTRAS
seen=set()
def closure(p):
 if p in seen:return
 seen.add(p)
 for name in re.findall(r'^\s*#include\s*"([^"\n]+)"',p.read_text(),re.M):
  q=next(q for q in (p.parent/name,R/'rebuild/include'/name,inc/name) if q.is_file())
  if q.is_relative_to(R/'rebuild/include'):
   target=inc/q.relative_to(R/'rebuild/include');target.parent.mkdir(parents=True,exist_ok=True)
   if target.exists():assert target.read_bytes()==q.read_bytes(),q
   else:target.write_bytes(q.read_bytes())
  closure(q)
for p in [FIXTURE,*extras]:closure(p)
env=V.env();env['INCLUDE']=str(V.VC/'include')+';'+str(inc)
def save(n,data):(b/n).write_text(json.dumps(data,indent=2)+'\n')
for i,p in enumerate([FIXTURE,*extras]):
 source=b/('fixture.cpp' if i==0 else p.name);source.write_bytes(p.read_bytes());obj=b/('extra%d.obj'%i)
 r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(inc),'/Fo'+str(obj),str(source)],env,b);save('compile%d.json'%i,dict(returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
# Resolve each reused provider to its source, then optionally rebuild all of them.
old=R/'rebuild/build/native-bank-file_lifetime_factory-owner/00/9d/009d6060/20261019T221038607481Z'
indices={int(p.name.split('_',1)[0]):p.name.split('_',1)[1] for p in (old/'sources').glob('[0-9]*_*.cpp')}
for index,obj in enumerate(list(objects)):
 if obj.parent==b:
  n=int(obj.stem[5:]);object_sources[str(obj)]=str(FIXTURE if n==0 else extras[n-1]);continue
 if obj.name.startswith('path'):source=P.SOURCES[int(obj.stem[4:])];flags=['/GS'] if P.SOURCES.index(source) in (15,31) else []
 else:
  name=indices[int(obj.stem.split('provider')[1])];source=next(Path(p) for p in report['input_sha256'] if Path(p).name==name and Path(p).suffix=='.cpp');flags=G.banklink.flags(source)+F.flags(source)
 assert sha(source)==report['input_sha256'][str(source)],source
 if True:
  closure(source);snap=b/source.name;snap.write_bytes(source.read_bytes());fresh=b/('rebuilt-'+obj.name)
  r=V.cl(['/nologo','/c','/O2','/MT',*flags,'/I'+str(inc),'/Fo'+str(fresh),str(snap)],env,b);save(fresh.stem+'.compile.json',dict(returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));assert not r.returncode,r.stdout+r.stderr
  before,after=Coff(obj),Coff(fresh);normalize=lambda n:re.sub(r'\?A0x[0-9a-fA-F]+','?ANONYMOUS',n)
  for symbol,(section,offset) in before.symbols.items():
   if symbol.startswith('.'):continue
   matches=[n for n in after.symbols if normalize(n)==normalize(symbol)];assert len(matches)==1,(source,symbol)
   sec,off=after.symbols[matches[0]];a,ar=before.sections[section];z,zr=after.sections[sec];norm=lambda rels:{site:(normalize(name),kind) for site,(name,kind) in rels.items()}
   assert offset==off and a==z and norm(ar)==norm(zr),(source,symbol)
  objects[index]=fresh;obj=fresh
 object_sources[str(obj)]=str(source)
exe=b/'probe.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/subsystem:console','/map:'+str(b/'probe.map'),'/out:'+str(exe),*map(str,objects),'kernel32.lib',str(G.WINMM)]
r=subprocess.run(cmd,cwd=b,env=env,capture_output=True,text=True);save('link.json',dict(command=cmd,returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));assert not r.returncode,r.stdout+r.stderr
for name in ('synthetic.bin','owned-writes.bin'):shutil.copy2(prior/name,b/name)
for i in range(24):
 r=subprocess.run([str(exe),str(b/'synthetic.bin'),str(b/'owned-writes.bin')],cwd=b,capture_output=True,text=True,timeout=15);save('run%d.json'%i,dict(returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));assert not r.returncode,r.stdout+r.stderr
save('receipt.json',dict(status='PASS_SCRATCH',processes=24,fresh=True,object_sources=object_sources,objects={str(p):sha(p) for p in objects},boundary='All providers freshly compiled; wrapper and disk lifetime integrated. Header parser controlled; final-link audit still pending.'))
print('NATIVE_BANK_OPEN_HOST_PASS processes=24')
