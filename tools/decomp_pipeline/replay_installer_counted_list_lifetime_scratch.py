"""Unredirected CRT lifetime composition for recovered list cleanup."""
import json,subprocess,tempfile,hashlib
from pathlib import Path
import check_one
ROOT=Path(__file__).resolve().parents[2]
def main():
 sources=['work/InstallerCountedFileListClear_candidate.cpp','rebuild/tests/integration/InstallerCountedFileListLifetime_test.cpp']
 work=Path(tempfile.mkdtemp(prefix='installer-list-lifetime-'));env=check_one.env();objects=[]
 for path in sources:
  source=ROOT/path;obj=work/(source.stem+'.obj');r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],env,work);assert r.returncode==0,r.stdout+r.stderr;objects.append(obj)
 exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
 r=subprocess.run(link+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode!=0
 r=subprocess.run(link+[str(o)for o in objects],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
 r=subprocess.run([str(exe)],capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'INSTALLER_COUNTED_LIST_LIFETIME_OK cases=96 objects=96' in r.stdout,(r.returncode,r.stdout,r.stderr)
 (ROOT/'work/file_installer_constructor/counted-list-lifetime.json').write_text(json.dumps(dict(cases=96,objects=96,omitted_provider_checks=1,source_sha256={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest()for p in sources},boundary='unredirected CRT malloc/free nodes and new/delete control blocks and fixture objects; shared counts1/2/3 across lists, exactly-once final destruction and repeated empty clear; no real CFileInfo destructor claim'),indent=2)+'\n')
 print(r.stdout.strip())
if __name__=='__main__':main()
