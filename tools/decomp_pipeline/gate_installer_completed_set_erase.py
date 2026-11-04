import json,sys,tempfile,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one
from variable_allocator_parity import check
work=Path(tempfile.mkdtemp(prefix='installer-set-'));obj=work/'set.obj'
symbol='?Erase@InstallerCompletedFileSet@@QAEXPAUNode@1@@Z'
r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/'rebuild/src/compiled/00/4a/InstallerCompletedFileSet_Erase_004aba70.cpp')],check_one.env(),work)
assert r.returncode==0,r.stdout+r.stderr
report=check(obj,symbol,0x4aba70,59,3,{symbol:0x4aba70,'??1CCharString@@QAE@XZ':0x99eae0,'_free':0xbfea14})
(ROOT/'rebuild/compile-gate/installer-completed-set-erase-parity.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(report,indent=2))
source=ROOT/'rebuild/tests/integration/InstallerCompletedSetErase_test.cpp';fixture=work/'fixture.obj'
r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(fixture),str(source)],check_one.env(),work);assert r.returncode==0,r.stdout+r.stderr
exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
r=subprocess.run(link+[str(fixture)],env=check_one.env(),capture_output=True,text=True);assert r.returncode!=0
r=subprocess.run(link+[str(obj),str(fixture)],env=check_one.env(),capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
r=subprocess.run([str(exe)],capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'INSTALLER_SET_ERASE_OK' in r.stdout,(r.returncode,r.stdout,r.stderr)
(ROOT/'rebuild/compile-gate/installer-completed-set-erase-behavior.json').write_text(json.dumps(dict(result=r.stdout.strip(),omitted_provider_checks=1,boundary='actual malloc/free nodes; string destructor recorded, right-root-left ordering and cached-left mutation tested'),indent=2)+'\n')
print(r.stdout.strip())
