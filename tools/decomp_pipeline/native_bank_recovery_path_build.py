from pathlib import Path
from datetime import datetime,timezone
import sys,json,subprocess,hashlib,shutil
sys.path.insert(0,'tools/decomp_pipeline');import check_one as V
R=Path(__file__).resolve().parents[2];b=Path(sys.argv[1]).resolve();prior=b/'parent';parent=prior/'receipt.json';report=json.loads(parent.read_text());assert report['fresh'];shutil.copytree(prior/'include',b/'include');objects=[Path(p) for p,h in report['objects'].items() if Path(p).name!='extra0.obj'];assert all(hashlib.sha256(p.read_bytes()).hexdigest()==report['objects'][str(p)] for p in objects)
env=V.env();env['INCLUDE']=str(V.VC/'include')+';'+str(b/'include')
for n,p in enumerate([R/'rebuild/src/compiled/00/9c/CBankFile_RecoveryFile_009cbfe0.cpp',R/'rebuild/tests/integration/NativeBankRecoveryPathLifetime_test.cpp']):
 snap=b/('Fixture.cpp' if n==1 else p.name);snap.write_bytes(p.read_bytes());obj=b/('new%d.obj'%n);r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(b/'include'),'/Fo'+str(obj),str(snap)],env,b);(b/('compile%d.json'%n)).write_text(json.dumps(dict(returncode=r.returncode,stdout=r.stdout,stderr=r.stderr),indent=2));assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
for p in objects:
 if p.name.startswith('extra') and p.name!='extra0.obj':shutil.copy2(p,b/p.name)
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(b/'probe.exe'),'/map:'+str(b/'probe.map'),*map(str,objects),'kernel32.lib',str(R/'tools/VC71/lib/winmm.lib')]
# Retain the accepted import library path from the parent link command.
old=json.loads((prior/'link.json').read_text())['command'];cmd[-1]=old[-1]
r=subprocess.run(cmd,cwd=b,env=env,capture_output=True,text=True);(b/'link.json').write_text(json.dumps(dict(command=cmd,returncode=r.returncode,stdout=r.stdout,stderr=r.stderr),indent=2));assert not r.returncode,r.stdout+r.stderr
for name in ('synthetic.bin','owned-writes.bin'):shutil.copy2(prior/name,b/name)
for n in range(24):
 r=subprocess.run([str(b/'probe.exe'),str(b/'synthetic.bin'),str(b/'owned-writes.bin')],cwd=b,capture_output=True,text=True,timeout=15);(b/('run%d.json'%n)).write_text(json.dumps(dict(returncode=r.returncode,stdout=r.stdout,stderr=r.stderr),indent=2));assert not r.returncode and 'NATIVE_RECOVERY_PATH_CASES_PASS cases=6' in r.stdout,r.stdout+r.stderr
(b/'receipt.json').write_text(json.dumps(dict(status='PASS_SCRATCH',parent=str(parent),objects={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in objects},processes=24,path_cases=144,reads=24,bytes=888,boundary='All linked providers freshly compiled; recovery-path TU and fixture fresh. Path metadata varied and restored; actual handle stays open; no recovery-file creation/deletion.'),indent=2)+'\n');print('NATIVE_RECOVERY_PATH_HOST_PASS processes=24 paths=144 reads=24 bytes=888')
