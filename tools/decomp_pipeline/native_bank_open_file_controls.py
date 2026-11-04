from pathlib import Path
import sys,json,re,subprocess,hashlib
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import check_one as V
import gate_native_bank_shared_worker as G
import gate_native_threaded_file_read_paths as P
import bank_file_lifetime_factory_worker_evidence as F
b=Path(sys.argv[1]).resolve();report=json.loads((b/'receipt.json').read_text());assert report['fresh']
objects=[Path(p) for p in report['objects']]
assert all(hashlib.sha256(p.read_bytes()).hexdigest()==report['objects'][str(p)] for p in objects)
bank=json.loads((R/'rebuild/build/native-bank-file_lifetime_factory-owner/00/9d/009d6060/20261019T221038607481Z/receipt.json').read_text());prior=Path(bank['build'])
names={int(p.name.split('_',1)[0]):p.name.split('_',1)[1] for p in (prior/'sources').glob('[0-9]*_*.cpp')}
sources=[]
for obj in objects:
 if obj.name=='fixture.obj':sources.append(b/'fixture.cpp')
 elif obj.name.startswith('path'):sources.append(P.SOURCES[int(obj.stem[4:])])
 else:
  name=names[int(obj.stem.split('provider')[1])];sources.append(next(Path(p) for p in bank['input_sha256'] if Path(p).name==name and Path(p).suffix=='.cpp'))
original=[R/'rebuild/tests/integration/NativeFileBankLifetimeOwner_test.cpp',*G.SOURCES[1:],*F.SOURCES]
controls=[]
for name,index,text in F.controls(original):
 source=original[index]
 if source.name=='CCharString_Destructor_0099eae0.cpp':
  source=next(p for p in sources if p.name=='CCharString_GetAsUpperCase_0099ee20.cpp');text=source.read_text();assert text.count('--g_CCharStringInstanceCount_013BD800;')==1;text=text.replace('--g_CCharStringInstanceCount_013BD800;',';')
 assert source in sources,(name,source)
 controls.append((name,sources.index(source),text))
source=next(p for p in sources if p.name=='CThreadedFile_Open_0098e1e0.cpp');text=source.read_text()
for name,old,new in [('read-open-device','deviceId_ = onDvd ? 1 : 0;','deviceId_ = 0;'),('read-open-name','filenameStorage_ = filename;',';'),('read-open-length','length_ = GetFileSize(fileHandle_, 0);','length_ = 0;'),('read-open-key','physicalSortKey_ = ++s_NextReadPhysicalSortKey;','physicalSortKey_ = 0;'),('read-open-flag','openFlag_ = true;','openFlag_ = false;'),('read-open-result','return true;','return false;')]:
 assert old in text;controls.append((name,sources.index(source),text.replace(old,new,1)))
env=V.env();env['INCLUDE']=str(V.VC/'include')+';'+str(b/'include')
def save(n,v):
 p=b/n;assert not p.exists();p.write_text(json.dumps(v,indent=2)+'\n')
for name,index,text in controls:
 snap=b/(name+'.cpp');assert not snap.exists();snap.write_text(text);obj=b/(name+'.obj');source=sources[index]
 flags=G.banklink.flags(source)+F.flags(source)
 if source.name=='CWideString_Destructor_0099b510.cpp':flags=[]
 if source in P.SOURCES and P.SOURCES.index(source) in (15,31):flags+=['/GS']
 r=V.cl(['/nologo','/c','/O2','/MT',*flags,'/I'+str(b/'include'),'/Fo'+str(obj),str(snap)],env,b);save(name+'.compile.json',dict(returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));assert r.returncode==0,r.stdout+r.stderr
 replacement=list(objects);replacement[index]=obj;exe=b/(name+'.exe')
 r=subprocess.run([str(V.VC/'bin/link.exe'),'/nologo','/subsystem:console','/out:'+str(exe),*map(str,replacement),'kernel32.lib',str(G.WINMM)],env=env,cwd=b,capture_output=True,text=True);save(name+'.link.json',dict(returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));assert r.returncode==0,r.stdout+r.stderr
 r=subprocess.run([str(exe),str(b/'synthetic.bin'),str(b/'owned-writes.bin')],cwd=b,capture_output=True,text=True,timeout=15);save(name+'.run.json',dict(returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));assert r.returncode==80 and 'NATIVE_WORKER_FAILURE' in r.stderr,(name,r.returncode,r.stdout,r.stderr)
 print(name+' detected',flush=True)
save('controls-verification.json',dict(status='PASS',controls=[name for name,_,_ in controls]));print('OPEN_FILE_BANK_CONTROLS_PASS',len(controls))
