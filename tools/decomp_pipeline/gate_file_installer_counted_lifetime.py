"""Native boot counted ownership composition; real CRT allocation boundary."""
import json,subprocess,tempfile
from pathlib import Path
import check_one
ROOT=Path(__file__).resolve().parents[2]
def main():
 sources=['rebuild/src/compiled/00/40/FileInstallerCountedReset_00403db0.cpp','rebuild/src/compiled/00/40/FileInstallerDestroy_00403630.cpp'];work=Path(tempfile.mkdtemp(prefix='progress-counted-life-'));env=check_one.env();objects=[]
 for path in sources+['rebuild/tests/integration/FileInstallerCountedLifetime_test.cpp']:
  source=ROOT/path;obj=work/(source.stem+'.obj');r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],env,work);assert r.returncode==0,r.stdout+r.stderr;objects.append(obj)
 exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
 for omitted in range(2):
  r=subprocess.run(link+[str(o)for i,o in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True);assert r.returncode!=0
 r=subprocess.run(link+[str(o)for o in objects],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
 r=subprocess.run([str(exe)],cwd=work,capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'FILE_INSTALLER_LIFETIME_OK scenarios=32 destroyed=64 blocks_released=64'in r.stdout,(r.returncode,r.stdout,r.stderr)
 (ROOT/'rebuild/compile-gate/file-installer-counted-lifetime.json').write_text(json.dumps(dict(sources=sources,scenarios=32,omitted_provider_checks=2,result=r.stdout.strip(),invariants='real reset/destroy with null clear, initial control block creation, sharedcount2 manually seeded, replacement decrements oldsharedcount to1, otherowner clear destroys old object/block, replacement clear destroys new object/block exactly once;64actualCRTobjects/blocks',boundary='FableOperatorNew/Delete fixture forwards real CRT operator new/delete; polymorphic test objects; sharing count manually established by fixture, no actual CFileInstaller destructor/body claim'),indent=2)+'\n');print(r.stdout.strip())
if __name__=='__main__':main()
