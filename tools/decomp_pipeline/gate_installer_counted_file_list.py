import sys,json,tempfile,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one
from variable_allocator_parity import check
work=Path(tempfile.mkdtemp(prefix='installer-counted-list-'));obj=work/'list.obj'
r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/'rebuild/src/compiled/00/9c/InstallerCountedFileList_Clear_009c6cc0.cpp')],check_one.env(),work)
assert r.returncode==0,r.stdout+r.stderr
report=check(obj,'?Clear@InstallerCountedFileList@@QAEXXZ',0x9c6cc0,89,2,{'??3@YAXPAX@Z':0xbfe9bc,'_free':0xbfea14})
(ROOT/'rebuild/compile-gate/installer-counted-file-list-parity.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(report,indent=2))
source=ROOT/'rebuild/tests/integration/InstallerCountedFileListClear_test.cpp';fixture=work/'fixture.obj'
r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(fixture),str(source)],check_one.env(),work);assert r.returncode==0,r.stdout+r.stderr
blob=obj.read_bytes()
for original,replacement in [('_free','_Fnod'),('??3@YAXPAX@Z','_InfoDelete')]:
 needle=original.encode()+b'\0';value=replacement.encode()+b'\0';assert blob.count(needle)==1 and len(value)<=len(needle)
 blob=blob.replace(needle,value+b'\0'*(len(needle)-len(value)))
obj.write_bytes(blob)
exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
r=subprocess.run(link+[str(fixture)],env=check_one.env(),capture_output=True,text=True);assert r.returncode!=0
r=subprocess.run(link+[str(obj),str(fixture)],env=check_one.env(),capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
r=subprocess.run([str(exe)],capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'INSTALLER_COUNTED_LIST_OK cases=800' in r.stdout,(r.returncode,r.stdout,r.stderr)
(ROOT/'rebuild/compile-gate/installer-counted-file-list-behavior.json').write_text(json.dumps(dict(cases=800,omitted_provider_checks=1,boundary='actual counted-pointer destructor and list traversal; CRT release call sites redirected to snapshot hooks then actual free; callback modes reference replacement, cached-next mutation, payload mutation during info delete, head/tail replacement during node free; counts0/1/2/ffffffff'),indent=2)+'\n')
print(r.stdout.strip())
