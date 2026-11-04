from pathlib import Path
from datetime import datetime,timezone
import sys,json,subprocess,shutil
sys.path.insert(0,'tools/decomp_pipeline');import check_one as V
R=Path(__file__).resolve().parents[2];b=Path(sys.argv[1]).resolve();b.mkdir();shutil.copytree(R/'rebuild/include',b/'include');env=V.env();env['INCLUDE']=str(V.VC/'include')+';'+str(b/'include');objects=[]
paths={'runtime': '00/9d/STLBankRuntimeVector_Resize_009d4010.cpp', 'updates': '00/9d/STLBankUpdateVector_Resize_009d3df0.cpp', 'symbols': '00/49/STLCharStringVector_Resize_0049b760.cpp', 'checksums': '00/46/UlongVector_Resize_00464931.cpp', 'checksum-storage': '00/46/UlongVector_Storage_0046496f.cpp', 'checksum-overflow': '00/46/UlongVector_Overflow_00464aab.cpp'}
for name in ('runtime','updates','checksums','checksum-storage','checksum-overflow','symbols','Fixture'):
 p=R/'rebuild/tests/integration/NativeBankVectorResize_test.cpp' if name=='Fixture' else R/'rebuild/src/compiled'/paths[name]
 src=b/(name+'.cpp');src.write_bytes(p.read_bytes());obj=b/(name+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(b/'include'),'/Fo'+str(obj),str(src)],env,b);(b/(name+'.compile.json')).write_text(json.dumps(dict(returncode=r.returncode,stdout=r.stdout,stderr=r.stderr),indent=2));assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(b/'probe.exe'),'/map:'+str(b/'probe.map'),*map(str,objects),'kernel32.lib'];r=subprocess.run(cmd,cwd=b,env=env,capture_output=True,text=True);(b/'link.json').write_text(json.dumps(dict(command=cmd,returncode=r.returncode,stdout=r.stdout,stderr=r.stderr),indent=2));assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(b/'probe.exe')],cwd=b,capture_output=True,text=True,timeout=15);(b/'run.json').write_text(json.dumps(dict(returncode=r.returncode,stdout=r.stdout,stderr=r.stderr),indent=2));assert not r.returncode,r.stdout+r.stderr;print(r.stdout)
