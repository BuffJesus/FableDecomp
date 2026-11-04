from pathlib import Path
import sys,json,subprocess
sys.path.insert(0,'tools/decomp_pipeline');import check_one as V
b=Path(sys.argv[1]).resolve();source=(b/'candidate.cpp').read_text();command=json.loads((b/'link.json').read_text())['command'];env=V.env();env['INCLUDE']=str(V.VC/'include')+';'+str(b/'include')
controls=[('magic','"BREC"','"ZZZZ"'),('validation','!= validate','== validate'),('entry-arguments','ReadEntryList(stream, max_index, entry_count);','ReadEntryList(stream, entry_count, max_index);'),('entry-result','ReadEntryList(stream, max_index, entry_count);\n        return true;','return ReadEntryList(stream, max_index, entry_count);'),('absent-result','    }\n    return false;','    }\n    return true;'),('file-access','FILE_ACCESS_READ, 4','FILE_ACCESS_READ, 2')]

def save(n,v):
 p=b/n;assert not p.exists();p.write_text(json.dumps(v,indent=2)+'\n')
cases=[(name,old,new,source,9) for name,old,new in controls]
for name,file,old,new,index in [('disk-close','CDiskFileWin32_Close_0099A920.cpp','CloseHandle(WinFileHandle);',';',13),('disk-position','CDiskFileWin32_Read_0099a960.cpp','Position += read;','Position = 0;',10),('refill-offset','CFileDataInputStream_InternalReadNextSourceChunk_00994360.cpp','File->SetPosition(position);','File->SetPosition(0);',5)]:
 cases.append((name,old,new,(b/file).read_text(),index))
for name,old,new,source,index in cases:
 assert old in source;snap=b/(name+'.cpp');assert not snap.exists();snap.write_text(source.replace(old,new));obj=b/(name+'.obj');result=V.cl(['/nologo','/c','/O2','/GS','/MT','/I'+str(b/'include'),'/Fo'+str(obj),str(snap)],env,b);save(name+'.compile.json',dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert not result.returncode,result.stdout+result.stderr
 exe=b/(name+'.exe');cmd=[str(obj) if s==str(b/('provider%d.obj'%index)) else '/out:'+str(exe) if s.startswith('/out:') else s for s in command if not s.startswith('/map:')];result=subprocess.run(cmd,env=env,cwd=b,capture_output=True,text=True);save(name+'.link.json',dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert not result.returncode,result.stdout+result.stderr
 result=subprocess.run([str(exe),str(b/'synthetic-bank.bin')],cwd=b,capture_output=True,text=True,timeout=15);save(name+'.run.json',dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert result.returncode==80 and 'RECOVERY_FAILURE' in result.stderr,(name,result.returncode,result.stdout,result.stderr);print(name+' detected',flush=True)
save('controls-verification.json',dict(status='PASS',controls=[c[0] for c in cases]));print('RECOVERY_DISK_CONTROLS_PASS',len(cases))
