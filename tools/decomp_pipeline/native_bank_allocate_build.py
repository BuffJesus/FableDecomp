from pathlib import Path
from datetime import datetime,timezone
import sys,json,subprocess,shutil
sys.path.insert(0,'tools/decomp_pipeline');import check_one as V
R=Path(__file__).resolve().parents[2];b=Path(sys.argv[1]).resolve();b.mkdir();shutil.copytree(R/'rebuild/include',b/'include');env=V.env();env['INCLUDE']=str(V.VC/'include')+';'+str(b/'include')
for name in ('candidate','Fixture'):
 src=b/(name+'.cpp');src.write_bytes((R/'rebuild/src/compiled/00/9c/CBankFile_AllocateBuffers_009ceae0.cpp' if name=='candidate' else R/'rebuild/tests/integration/NativeBankAllocate_test.cpp').read_bytes());r=V.cl(['/nologo','/c','/O2','/MT',*(['/Dfree=FixtureBankAllocateFree'] if name=='candidate' else []),'/I'+str(b/'include'),'/Fo'+str(b/(name+'.obj')),str(src)],env,b);(b/(name+'.compile.json')).write_text(json.dumps(dict(returncode=r.returncode,stdout=r.stdout,stderr=r.stderr),indent=2));assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(b/'probe.exe'),'/map:'+str(b/'probe.map'),str(b/'candidate.obj'),str(b/'Fixture.obj'),'kernel32.lib'];r=subprocess.run(cmd,cwd=b,env=env,capture_output=True,text=True);(b/'link.json').write_text(json.dumps(dict(command=cmd,returncode=r.returncode,stdout=r.stdout,stderr=r.stderr),indent=2));assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(b/'probe.exe')],cwd=b,capture_output=True,text=True,timeout=15);(b/'run.json').write_text(json.dumps(dict(returncode=r.returncode,stdout=r.stdout,stderr=r.stderr),indent=2));assert not r.returncode,r.stdout+r.stderr;print(r.stdout)
