import sys,json,tempfile,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one
from variable_allocator_parity import check
work=Path(tempfile.mkdtemp(prefix='installer-array-'));obj=work/'array.obj'
r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/'rebuild/src/compiled/00/9c/FileInstallerBufferArray_009c7150.cpp')],check_one.env(),work)
assert r.returncode==0,r.stdout+r.stderr
destroy='?FableInstallerDestroyBufferRange@@YIXPAUInstallerBufferChunk@@0PAX@Z'
reports=[]
reports.append(check(obj,'?RemoveRange@CFileInstallerBufferArray@@QAEPAUInstallerBufferChunk@@PAU2@0@Z',0x9c7150,76,1,{destroy:0x9c6d60}))
reports.append(check(obj,'?DeallocateMemory@CFileInstallerBufferArray@@QAEXXZ',0x9c76d0,85,3,{destroy:0x9c6d60,'?RemoveRange@CFileInstallerBufferArray@@QAEPAUInstallerBufferChunk@@PAU2@0@Z':0x9c7150,'_free':0xbfea14}))
(ROOT/'rebuild/compile-gate/installer-cleanup-buffer-array-shared-parity.json').write_text(json.dumps(reports,indent=2)+'\n')
print(json.dumps(reports,indent=2))
fixture=ROOT/'rebuild/tests/integration/InstallerBufferArrayShared_test.cpp';testobj=work/'fixture.obj'
r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(testobj),str(fixture)],check_one.env(),work)
assert r.returncode==0,r.stdout+r.stderr
exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
r=subprocess.run(link+[str(testobj)],env=check_one.env(),capture_output=True,text=True);assert r.returncode!=0
r=subprocess.run(link+[str(obj),str(testobj)],env=check_one.env(),capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
r=subprocess.run([str(exe)],capture_output=True,text=True,timeout=30)
assert r.returncode==0 and 'INSTALLER_BUFFER_ARRAY_OK' in r.stdout,(r.returncode,r.stdout,r.stderr)
print(r.stdout.strip())
(ROOT/'rebuild/compile-gate/installer-cleanup-buffer-array-shared-behavior.json').write_text(json.dumps(dict(stdout=r.stdout.strip(),omitted_provider_checks=1,boundary='range destruction recorded; native RemoveRange and DeallocateMemory execute; malloc/free actual for array storage; both complete-body parity thresholds enforced'),indent=2)+'\n')
