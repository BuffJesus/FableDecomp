import json,sys,tempfile,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one
from variable_allocator_parity import check
work=Path(tempfile.mkdtemp(prefix='installer-range-'));obj=work/'range.obj'
r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/'rebuild/src/compiled/00/9c/FileInstallerDestroyBufferRange_009c6d60.cpp')],check_one.env(),work)
assert r.returncode==0,r.stdout+r.stderr
targets={'?EnterSynchronisation@CMemoryAllocator@@KIXXZ':0x9f2000,'?LeaveSynchronisation@CMemoryAllocator@@KIXXZ':0x9f2010,'?g_FableMemoryAllocator_013CAA90@@3PAVCMemoryAllocator@@A':0x13caa90,'?g_FableAllocatorShutdownRequested_013CAA94@@3_NA':0x13caa94,'?InternalDeallocate@CMemoryAllocator@@IAEXPAX@Z':0x9f2920,'?GetNoAllocations@CMemoryAllocator@@QAEKXZ':0x9f1f10,'??1CMemoryAllocator@@IAE@XZ':0x9f2310,'??3CMemoryAllocator@@KIXPAX@Z':0x9f1f90}
report=check(obj,'?FableInstallerDestroyBufferRange@@YIXPAUInstallerBufferChunk@@0PAX@Z',0x9c6d60,125,11,targets)
(ROOT/'rebuild/compile-gate/installer-cleanup-destroy-buffer-range-parity.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(report,indent=2))
source=ROOT/'rebuild/tests/integration/InstallerDestroyBufferRangeShared_test.cpp';fixture=work/'fixture.obj'
r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(fixture),str(source)],check_one.env(),work)
assert r.returncode==0,r.stdout+r.stderr
blob=obj.read_bytes()
for original,replacement in [('__imp__EnterCriticalSection@4','_EnterSlot'),('__imp__LeaveCriticalSection@4','_LeaveSlot'),('__imp__VirtualFree@12','_FreeSlot')]:
 needle=original.encode()+b'\0';value=replacement.encode()+b'\0';assert blob.count(needle)==1;blob=blob.replace(needle,value+b'\0'*(len(needle)-len(value)))
obj.write_bytes(blob)
exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
r=subprocess.run(link+[str(fixture)],env=check_one.env(),capture_output=True,text=True);assert r.returncode!=0
r=subprocess.run(link+[str(obj),str(fixture)],env=check_one.env(),capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
r=subprocess.run([str(exe)],capture_output=True,text=True,timeout=30)
assert r.returncode==0 and 'INSTALLER_DESTROY_RANGE_OK cases=320' in r.stdout,(r.returncode,r.stdout,r.stderr)
(ROOT/'rebuild/compile-gate/installer-cleanup-destroy-buffer-range-behavior.json').write_text(json.dumps(dict(cases=320,omitted_provider_checks=1,boundary='independent retail-instruction event model; allocator member boundaries and Windows imports recorded; genuine out-of-line support methods execute; snapshots include six chunk words plus singleton/shutdown/lock depth'),indent=2)+'\n')
print(r.stdout.strip())
