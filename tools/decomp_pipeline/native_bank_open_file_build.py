from pathlib import Path
from datetime import datetime,timezone
import sys,json,hashlib,shutil,re,subprocess
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import check_one as V
import gate_native_threaded_file_read_paths as P
import gate_native_bank_shared_worker as G
import bank_file_lifetime_factory_worker_evidence as F
b=Path(sys.argv[1]).resolve();b.mkdir();print(b,flush=True)
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
bank=json.loads((R/'rebuild/build/native-bank-file_lifetime_factory-owner/00/9d/009d6060/20261019T221038607481Z/receipt.json').read_text());prior=Path(bank['build'])
paths=json.loads((R/'rebuild/build/native-file-read-paths/00/98/0098e000/20261019T021303326518Z/receipt.json').read_text());pb=Path(paths['build'])
inc=b/'include';shutil.copytree(prior/'include',inc)
header_refresh=[]
for snapshot in inc.rglob('*'):
 if snapshot.is_file():
  current=R/'rebuild/include'/snapshot.relative_to(inc)
  if current.is_file() and snapshot.read_bytes()!=current.read_bytes():
   header_refresh.append(dict(path=str(current),previous_sha256=sha(snapshot),current_sha256=sha(current)))
   snapshot.write_bytes(current.read_bytes())
for p in (prior/'sources').glob('*.h'):shutil.copy2(p,inc/p.name)
indices={p.name.split('_',1)[1]:int(p.name.split('_',1)[0]) for p in (prior/'sources').glob('[0-9]*_*.cpp')}
objects={name:prior/('provider%d.obj'%i) for name,i in indices.items() if i}
# Read-path providers whose strong helper expansions supersede standalone TUs.
exclude={7,11,12,17,18,25,26,27,28,29,30,37,38,42}
for i in [*range(1,43),44]:
 if i not in exclude:objects.setdefault(P.SOURCES[i].name,pb/('provider%d.obj'%i))
for name in ('CCharString_Destructor_0099eae0.cpp','CCharString_UnassignString_0099e9b0.cpp','CWideString_AllocStringData_0099b3c0.cpp','CWideString_GetLength_0099b220.cpp'):
 objects.pop(name,None)
objects['CWideString_Destructor_0099b510.cpp']=pb/'provider8.obj'
for p in objects.values():
 report=bank if p.parent==prior else paths
 assert sha(p)==report['object_sha256'][str(p)],p
# Fresh added providers use the current bank include snapshot. Resolve only their actual include closure.
seen=set()
def closure(p):
 if p in seen:return
 seen.add(p)
 for name in re.findall(r'^\s*#include\s*"([^"\n]+)"',p.read_text(),re.M):
  q=next(q for q in (p.parent/name,R/'rebuild/include'/name) if q.is_file())
  if q.is_relative_to(R/'rebuild/include'):
   dest=inc/q.relative_to(R/'rebuild/include');dest.parent.mkdir(parents=True,exist_ok=True)
   if dest.exists():assert dest.read_bytes()==q.read_bytes(),q
   else:dest.write_bytes(q.read_bytes())
  closure(q)
for name,p in objects.items():
 if p.parent==pb:closure(next(x for x in P.SOURCES if x.name==name))
fixture=b/'fixture.cpp';fixture.write_bytes((R/'rebuild/tests/integration/NativeOpenFileBankLifetimeOwner_test.cpp').read_bytes());obj=b/'fixture.obj'
env=V.env();env['INCLUDE']=str(V.VC/'include')+';'+str(inc)
def save(n,v):(b/n).write_text(json.dumps(v,indent=2)+'\n')
r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(inc),'/Fo'+str(obj),str(fixture)],env,b);save('compile.json',dict(stdout=r.stdout,stderr=r.stderr,returncode=r.returncode));assert r.returncode==0,r.stdout+r.stderr
from gate_game_component_abi import Coff
for name,old in list(objects.items()):
 if old.parent!=pb:continue
 source=next(x for x in P.SOURCES if x.name==name);index=P.SOURCES.index(source);snap=b/name;snap.write_bytes(source.read_bytes());fresh=b/('path%d.obj'%index)
 r=V.cl(['/nologo','/c','/O2','/MT',*(['/GS'] if index in (15,31) else []),'/I'+str(inc),'/Fo'+str(fresh),str(snap)],env,b)
 save('path%d.compile.json'%index,dict(stdout=r.stdout,stderr=r.stderr,returncode=r.returncode));assert r.returncode==0,r.stdout+r.stderr
 before,after=Coff(old),Coff(fresh)
 for record in paths['helpers']:
  symbol=record['symbol']
  if symbol not in before.symbols:continue
  sec,off=before.symbols[symbol];newsec,newoff=after.symbols[symbol]
  assert off==newoff and before.sections[sec]==after.sections[newsec],(name,symbol)
 objects[name]=fresh
if True:
 for name,old in list(objects.items()):
  if old.parent!=prior:continue
  candidates=[Path(p) for p in bank['input_sha256'] if Path(p).name==name and Path(p).suffix=='.cpp'];assert len(candidates)==1,(name,candidates)
  source=candidates[0];assert sha(source)==bank['input_sha256'][str(source)]
  snap=b/name;snap.write_bytes(source.read_bytes());fresh=b/('fresh-'+old.name)
  flags=G.banklink.flags(source)+F.flags(source)
  r=V.cl(['/nologo','/c','/O2','/MT',*flags,'/I'+str(inc),'/Fo'+str(fresh),str(snap)],env,b)
  save('fresh-'+old.stem+'.compile.json',dict(stdout=r.stdout,stderr=r.stderr,returncode=r.returncode));assert r.returncode==0,r.stdout+r.stderr
  before,after=Coff(old),Coff(fresh)
  for symbol,(section,offset) in before.symbols.items():
   if symbol.startswith('.'):continue # Debug/source-name sections are not executable providers.
   normalize=lambda n:re.sub(r'\?A0x[0-9a-fA-F]+', '?ANONYMOUS',n)
   matches=[n for n in after.symbols if normalize(n)==normalize(symbol)];assert len(matches)==1,(name,symbol)
   newsec,newoff=after.symbols[matches[0]]
   oldbody,oldrels=before.sections[section];newbody,newrels=after.sections[newsec]
   normrels=lambda rels:{site:(normalize(target),kind) for site,(target,kind) in rels.items()}
   assert offset==newoff and oldbody==newbody and normrels(oldrels)==normrels(newrels),(name,symbol)
  objects[name]=fresh
preferred=['CThreadedFile_Close_0098ec40.cpp','CBankFileAsyncEntry_CountedLifetime_009d69a0.cpp',G.T.SOURCES[0].name]
linked=[objects[n] for n in preferred]+[p for n,p in objects.items() if n not in preferred]+[obj]
exe=b/'probe.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/subsystem:console','/map:'+str(b/'probe.map'),'/out:'+str(exe),*map(str,linked),'kernel32.lib',str(G.WINMM)]
r=subprocess.run(cmd,env=env,cwd=b,capture_output=True,text=True);save('link.json',dict(command=cmd,stdout=r.stdout,stderr=r.stderr,returncode=r.returncode));assert r.returncode==0,r.stdout+r.stderr
for name in ('synthetic.bin','owned-writes.bin'):shutil.copy2(prior/name,b/name)
for i in range(24):
 r=subprocess.run([str(exe),str(b/'synthetic.bin'),str(b/'owned-writes.bin')],cwd=b,capture_output=True,text=True,timeout=15);save('run%d.json'%i,dict(returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));assert r.returncode==0,r.stdout+r.stderr
save('receipt.json',dict(status='PASS_SCRATCH',fresh=True,processes=24,header_refresh=header_refresh,objects={str(p):sha(p) for p in linked},boundary='Original provider bytes pinned; all providers freshly compiled with current headers and compared against accepted named COFF sections. Source-dependent anonymous namespace hashes normalized; debug sections excluded. Final linked checks follow separately.'))
print('OPEN_FILE_BANK_HOST_PASS processes=24')
