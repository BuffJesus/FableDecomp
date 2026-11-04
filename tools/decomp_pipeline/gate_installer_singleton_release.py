import sys,tempfile,subprocess,json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one
from variable_allocator_parity import check
w=Path(tempfile.mkdtemp(prefix='installer-singleton-release-'));env=check_one.env();objects=[]
for i,source in enumerate(['rebuild/src/compiled/00/40/CFileInstallerSingleton_Release_00404490.cpp','rebuild/src/compiled/01/3c/FileInstaller_Data_013ca818.cpp','rebuild/tests/integration/FileInstallerSingletonRelease_test.cpp']):
    obj=w/('%d.obj'%i);r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/source)],env,w)
    assert r.returncode==0,r.stdout+r.stderr;objects.append(obj)
report=check(objects[0],'?Release@?$CTBaseSingleton@VCFileInstaller@@@@SIXXZ',0x404490,57,6,{'?g_CFileInstallerSingleton_013CA818@@3UCCountedFileInstaller@@A':0x13ca818,'?FableOperatorDelete@@YAXPAX@Z':0xbfe9bc})
exe=w/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
for omit in (0,1):
    r=subprocess.run(cmd+[str(o)for i,o in enumerate(objects)if i!=omit],env=env,capture_output=True,text=True);assert r.returncode!=0
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'INSTALLER_SINGLETON_RELEASE_PASS cases=32'in r.stdout,(r.returncode,r.stdout,r.stderr)
report.update(cases=32,omission_checks=2,boundary='Actual singleton storage/release; destructor and deallocator recorded, including reentrant replacement/clearing of singleton pointer.')
(ROOT/'rebuild/compile-gate/installer-singleton-release.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2));print(r.stdout.strip())
