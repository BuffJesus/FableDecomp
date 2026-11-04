from pathlib import Path
from datetime import datetime,timezone
import json,hashlib,sys,subprocess,shutil
sys.path.insert(0,'tools/decomp_pipeline')
import check_one as V
from file_input_stream_sources import SOURCES
R=Path(__file__).resolve().parents[2];b=Path(sys.argv[1]).resolve();prior=b/'parent';parent=prior/'receipt.json';report=json.loads(parent.read_text());assert report['fresh']
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
objects=[Path(p) for p in report['objects'] if Path(p).name!='extra0.obj'];assert all(sha(p)==report['objects'][str(p)] for p in objects)
shutil.copytree(prior/'include',b/'include')
for p in (R/'rebuild/include').rglob('*.h'):
 q=b/'include'/p.relative_to(R/'rebuild/include');q.parent.mkdir(parents=True,exist_ok=True)
 if q.exists():assert q.read_bytes()==p.read_bytes(),p
 else:q.write_bytes(p.read_bytes())
(b/'include/NativeBankRecoveryColdAliases.h').write_bytes((R/'rebuild/tests/integration/NativeBankRecoveryColdAliases.h').read_bytes())
env=V.env();env['INCLUDE']=str(V.VC/'include')+';'+str(b/'include');providers=[]
for n,p in enumerate([*SOURCES,R/'rebuild/src/compiled/00/9d/CBankFile_ReadRecoveryFile_009d0380.cpp',*[R/'rebuild/src/compiled/00/99'/name for name in ('CDiskFileWin32_Read_0099a960.cpp','CDiskFileWin32_SetPosition_0099abf0.cpp','CDiskFileWin32_GetPosition_0099abd0.cpp')],R/'rebuild/tests/integration/NativeBankRecoveryOwned_test.cpp']):
 snap=b/('Fixture.cpp' if n==13 else p.name);snap.write_bytes(p.read_bytes());obj=b/('recovery%d.obj'%n);r=V.cl(['/nologo','/c','/O2','/MT',*(['/GS'] if n==9 else []),'/I'+str(b/'include'),'/Fo'+str(obj),str(snap)],env,b);(b/('compile%d.json'%n)).write_text(json.dumps(dict(returncode=r.returncode,stdout=r.stdout,stderr=r.stderr),indent=2));assert not r.returncode,r.stdout+r.stderr;objects.append(obj);providers.append(dict(source=str(p),object=str(obj),sha256=sha(obj)))
p=R/'rebuild/src/compiled/00/9c/CBankFile_RecoveryFile_009cbfe0.cpp';snap=b/p.name;snap.write_bytes(p.read_bytes());obj=b/'new0.obj';result=V.cl(['/nologo','/c','/O2','/MT','/I'+str(b/'include'),'/Fo'+str(obj),str(snap)],env,b);(b/'path.compile.json').write_text(json.dumps(dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr),indent=2));assert not result.returncode,result.stdout+result.stderr;objects.append(obj)
for n in range(1,12):shutil.copy2(prior/('extra%d.obj'%n),b/('extra%d.obj'%n))
for n in range(13):shutil.copy2(b/('recovery%d.obj'%n),b/('provider%d.obj'%n))
shutil.copy2(prior/'extra7.obj',b/'provider13.obj')
(b/'NativeBankRecoveryColdAliases.h').write_bytes((R/'rebuild/tests/integration/NativeBankRecoveryColdAliases.h').read_bytes())
(b/'CDiskFileWin32_Close_0099A920.cpp').write_bytes((R/'rebuild/src/compiled/00/99/CDiskFileWin32_Close_0099A920.cpp').read_bytes())
old=json.loads((prior/'link.json').read_text())['command'];cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(b/'probe.exe'),'/map:'+str(b/'probe.map'),*map(str,objects),'kernel32.lib',old[-1]]
r=subprocess.run(cmd,cwd=b,env=env,capture_output=True,text=True);(b/'link.json').write_text(json.dumps(dict(command=cmd,returncode=r.returncode,stdout=r.stdout,stderr=r.stderr),indent=2));assert not r.returncode,r.stdout+r.stderr
for name in ('synthetic.bin','owned-writes.bin'):shutil.copy2(prior/name,b/name)
for n in range(24):
 r=subprocess.run([str(b/'probe.exe'),str(b/'synthetic.bin'),str(b/'owned-writes.bin')],cwd=b,capture_output=True,text=True,timeout=15);(b/('run%d.json'%n)).write_text(json.dumps(dict(returncode=r.returncode,stdout=r.stdout,stderr=r.stderr),indent=2));assert not r.returncode and 'NATIVE_OWNED_RECOVERY_PASS cases=9 buffers=8 closed=8' in r.stdout,r.stdout+r.stderr
(b/'receipt.json').write_text(json.dumps(dict(status='PASS',fresh=True,parent=str(parent),parent_sha256=sha(parent),objects={str(p):sha(p) for p in objects},processes=24,recovery_cases=216,boundary='All157 linked providers freshly compiled. Native path/existence/disk Open/Read/Seek/Position/Close and recovery/stream lifetime; entry-list and header parser controlled. Retained synthetic recovery file only.'),indent=2)+'\n');print('NATIVE_OWNED_RECOVERY_PASS processes=24 cases=216')
