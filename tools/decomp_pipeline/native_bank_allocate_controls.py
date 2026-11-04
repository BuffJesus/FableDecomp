from pathlib import Path
import sys,json,subprocess
sys.path.insert(0,'tools/decomp_pipeline');import check_one as V
b=Path(sys.argv[1]).resolve();source=(b/'candidate.cpp').read_text();command=json.loads((b/'link.json').read_text())['command'];env=V.env();env['INCLUDE']=str(V.VC/'include')+';'+str(b/'include')
controls=[('wrong-size','Size = size;','Size = 0;'),('runtime-size','RuntimeData.resize(size, empty);','RuntimeData.resize(size+1, empty);'),('symbol-flag','OpenFlags & 8','OpenFlags & 4'),('checksum-flag','OpenFlags & 16','OpenFlags & 8'),('update-flag','OpenFlags & 32','OpenFlags & 16'),('checksum-free','Checksums.DeallocateMemory();',';'),('symbol-map','SymbolIndices.clear();',';'),('filename-map','FilenameIndices.clear();',';')]
def save(name,data):
 p=b/name;assert not p.exists();p.write_text(json.dumps(data,indent=2)+'\n')
for name,old,new in controls:
 assert source.count(old)==1;src=b/(name+'.cpp');src.write_text(source.replace(old,new));obj=b/(name+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/Dfree=FixtureBankAllocateFree','/I'+str(b/'include'),'/Fo'+str(obj),str(src)],env,b);save(name+'.compile.json',dict(returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));assert not r.returncode,r.stdout+r.stderr
 exe=b/('control-%d.exe'%controls.index((name,old,new)));cmd=[str(obj) if s==str(b/'candidate.obj') else '/out:'+str(exe) if s.startswith('/out:') else s for s in command if not s.startswith('/map:')];r=subprocess.run(cmd,cwd=b,env=env,capture_output=True,text=True);save(name+'.link.json',dict(returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));assert not r.returncode,r.stdout+r.stderr
 r=subprocess.run([str(exe)],cwd=b,capture_output=True,text=True,timeout=15);save(name+'.run.json',dict(returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));assert r.returncode==80 and 'BANK_ALLOCATION_FAILURE' in r.stderr,(name,r.returncode,r.stderr)
save('native-controls.json',dict(status='PASS',controls=[c[0] for c in controls]));print('ALLOCATION_NATIVE_CONTROLS_PASS',len(controls))
