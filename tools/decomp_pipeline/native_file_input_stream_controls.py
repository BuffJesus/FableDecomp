from pathlib import Path
import sys,json,subprocess
sys.path.insert(0,'tools/decomp_pipeline');import check_one as V
b=Path(sys.argv[1]).resolve();root=Path(__file__).resolve().parent;providers=json.loads((b/'providers.json').read_text());command=json.loads((b/'link.json').read_text())['command'];env=V.env()
controls=[('no-buffered-read',6,'return size < BufferSize;','return false;'),('wrong-refill-offset',5,'File->SetPosition(position);','File->SetPosition(position+1);'),('no-direct-position',3,'StreamPos = position + size;','StreamPos = position;'),('no-close-position',4,'File->SetPosition(GetPosition());',';'),('no-buffer-free',4,'delete[] Buffer;',';'),('no-window-drain',3,'Read(data, available);',';')]
def save(name,data):
 p=b/name;assert not p.exists();p.write_text(json.dumps(data,indent=2)+'\n')
for name,index,old,new in controls:
 p=Path(providers[index]['source']);text=p.read_text();assert text.count(old)==1;src=b/(name+'.cpp');assert not src.exists();src.write_text(text.replace(old,new));obj=b/(name+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(b/'include'),'/Fo'+str(obj),str(src)],env,b);save(name+'.compile.json',dict(returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));assert not r.returncode,r.stdout+r.stderr
 exe=b/(name+'.exe');cmd=[str(obj) if s==providers[index]['object'] else '/out:'+str(exe) if s.startswith('/out:') else s for s in command if not s.startswith('/map:')];r=subprocess.run(cmd,env=env,cwd=b,capture_output=True,text=True);save(name+'.link.json',dict(returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));assert not r.returncode,r.stdout+r.stderr
 r=subprocess.run([str(exe)],capture_output=True,text=True,timeout=15);save(name+'.run.json',dict(returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));assert r.returncode==80 and 'INPUT_STREAM_FAILURE' in r.stderr,(name,r.returncode,r.stdout,r.stderr);print(name+' detected',flush=True)
save('controls-verification.json',dict(status='PASS',controls=[v[0] for v in controls]));print('INPUT_STREAM_CONTROLS_PASS',len(controls))
