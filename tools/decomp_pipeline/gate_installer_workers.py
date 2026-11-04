import json,sys,tempfile,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one
from variable_allocator_parity import check
work=Path(tempfile.mkdtemp(prefix='installer-workers-'))
obj=work/'workers.obj'
r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/'rebuild/src/compiled/00/9c/CFileInstaller_Workers_009c5870.cpp')],check_one.env(),work)
assert r.returncode==0,r.stdout+r.stderr
reports=[]
for symbol,address,member in [('?FableInstallerWorker_009C5870@@YAXPAX@Z',0x9c5870,'?ThreadMain@CFileInstaller@@QAEXXZ'),('?FableInstallerDecompressionWorker_009C4420@@YAXPAX@Z',0x9c4420,'?DecompressionThreadMain@CFileInstaller@@QAEXXZ')]:
 reports.append(check(obj,symbol,address,24,3,{'__imp__GetCurrentThread@0':0x143fdcc,'__imp__SetThreadPriority@8':0x143fdd0,member:0x9c5690 if address==0x9c5870 else 0x9c4230}))
(ROOT/'rebuild/compile-gate/installer-worker-parity.json').write_text(json.dumps(reports,indent=2)+'\n')
print(json.dumps(reports,indent=2))
fixture=ROOT/'rebuild/tests/integration/FileInstallerWorkers_test.cpp'
testobj=work/'fixture.obj'
r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(testobj),str(fixture)],check_one.env(),work)
assert r.returncode==0,r.stdout+r.stderr
raw=obj.read_bytes()
for old,new in [('__imp__GetCurrentThread@0','_GetSlot'),('__imp__SetThreadPriority@8','_PrioritySlot')]:
 needle=old.encode()+b'\0';value=new.encode()+b'\0';assert raw.count(needle)==1
 raw=raw.replace(needle,value+b'\0'*(len(needle)-len(value)))
obj.write_bytes(raw)
exe=work/'fixture.exe'
link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
r=subprocess.run(link+[str(testobj)],env=check_one.env(),capture_output=True,text=True)
assert r.returncode!=0
r=subprocess.run(link+[str(obj),str(testobj)],env=check_one.env(),capture_output=True,text=True)
assert r.returncode==0,r.stdout+r.stderr
r=subprocess.run([str(exe)],capture_output=True,text=True,timeout=30)
assert r.returncode==0 and 'FILE_INSTALLER_WORKERS_OK cases=54' in r.stdout,(r.returncode,r.stdout,r.stderr)
(ROOT/'rebuild/compile-gate/installer-worker-behavior.json').write_text(json.dumps(dict(cases=54,omitted_provider_checks=1,actual_threads=0,boundary='recorded OS imports and member loop bodies; exact genuine C++ entry wrappers execute'),indent=2)+'\n')
print(r.stdout.strip())
