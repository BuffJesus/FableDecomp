from pathlib import Path
import sys,json,subprocess
sys.path.insert(0,'tools/decomp_pipeline');import check_one as V
b=Path(sys.argv[1]).resolve();text=(b/'CBankFile_RecoveryFile_009cbfe0.cpp').read_text();command=json.loads((b/'link.json').read_text())['command'];env=V.env();env['INCLUDE']=str(V.VC/'include')+';'+str(b/'include')
def save(n,v):
 p=b/n;assert not p.exists();p.write_text(json.dumps(v,indent=2)+'\n')
controls=[('wrong-extension','L".rec"','L".bad"'),('missing-filename','path_drive + path_dir + path_filename + L".rec"','path_drive + path_dir + L".rec"'),('wrong-path-owner','BankFile->GetPathname()','Filename')]
for name,old,new in controls:
 assert text.count(old)==1;snap=b/(name+'.cpp');snap.write_text(text.replace(old,new));obj=b/(name+'.obj');result=V.cl(['/nologo','/c','/O2','/MT','/I'+str(b/'include'),'/Fo'+str(obj),str(snap)],env,b);save(name+'.compile.json',dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert not result.returncode,result.stdout+result.stderr
 exe=b/(name+'.exe');cmd=[str(obj) if s==str(b/'new0.obj') else '/out:'+str(exe) if s.startswith('/out:') else s for s in command if not s.startswith('/map:')];result=subprocess.run(cmd,cwd=b,env=env,capture_output=True,text=True);save(name+'.link.json',dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert not result.returncode,result.stdout+result.stderr
 result=subprocess.run([str(exe),str(b/'synthetic.bin'),str(b/'owned-writes.bin')],cwd=b,capture_output=True,text=True,timeout=15);save(name+'.run.json',dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert result.returncode==80 and 'NATIVE_WORKER_FAILURE' in result.stderr,(name,result.returncode,result.stdout,result.stderr)
save('recovery-controls.json',dict(status='PASS',controls=[c[0] for c in controls]));print('NATIVE_RECOVERY_PATH_CONTROLS_PASS',len(controls))
