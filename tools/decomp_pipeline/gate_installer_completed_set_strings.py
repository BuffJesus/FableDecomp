"""Completed-file tree erasure with actual recovered string ownership."""
import json,subprocess,tempfile,hashlib
from pathlib import Path
import check_one
from replay_file_installer_constructor_os_scratch import STRINGS
ROOT=Path(__file__).resolve().parents[2]
def main():
 excluded={'CCharString_operator_0099efe0.cpp','CBasicRetailBankAliasMap_Erase_009aa830.cpp'}
 sources=['rebuild/src/compiled/00/4a/InstallerCompletedFileSet_Erase_004aba70.cpp']+['rebuild/src/compiled/'+p for p in STRINGS if Path(p).name not in excluded]
 work=Path(tempfile.mkdtemp(prefix='installer-set-strings-'));env=check_one.env();objects=[]
 for path in sources+['rebuild/tests/integration/InstallerCompletedSetStrings_test.cpp']:
  source=ROOT/path;obj=work/(source.stem+'.obj');r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],env,work);assert r.returncode==0,r.stdout+r.stderr;objects.append(obj)
 exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
 for omitted in range(len(sources)):
  r=subprocess.run(link+[str(p)for i,p in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True);assert r.returncode!=0,('provider omitted',sources[omitted])
 r=subprocess.run(link+[str(p)for p in objects],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
 r=subprocess.run([str(exe)],cwd=work,capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'INSTALLER_SET_STRINGS_OK cases=96' in r.stdout,(r.returncode,r.stdout,r.stderr)
 report=dict(sources=sources,native_sources=len(sources),omitted_provider_checks=len(sources),result=r.stdout.strip(),source_sha256={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest()for p in sources},boundary='actual CRT calloc/free tree nodes and actual production CharString/BasicString allocation/copy/destruction; fixture empty-string fallback data, manually built shapes; no tree insertion or installer destructor claim')
 (ROOT/'rebuild/compile-gate/installer-completed-set-strings.json').write_text(json.dumps(report,indent=2)+'\n')
 print(r.stdout.strip());print('providers='+str(len(sources))+' omissions='+str(len(sources)))
if __name__=='__main__':main()
