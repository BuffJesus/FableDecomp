"""Shared installer cleanup with actual allocator members and Windows APIs."""
import json,subprocess,tempfile,hashlib
from pathlib import Path
import check_one
from gate_main_allocator_os import SOURCES
ROOT=Path(__file__).resolve().parents[2]
def main():
 sources=SOURCES+['rebuild/src/compiled/00/9f/CMemoryAllocator_GetNoAllocations_009f1f10.cpp','rebuild/src/compiled/00/9f/CMemoryAllocator_GetAllocationSize_009f2390.cpp','rebuild/src/compiled/01/3c/AllocatorRangeData_013caaa4.cpp','rebuild/src/compiled/00/9f/CMemoryAllocator_InitDebugInfo_009f28d0.cpp','rebuild/src/compiled/00/9f/CMemoryAllocator_InternalAllocate_009f2a40.cpp','rebuild/src/compiled/00/9f/CMemoryAllocator_InternalDeallocate_009f2920.cpp','rebuild/src/compiled/00/9f/CMemoryAllocator_Aligned_009f2be0.cpp','rebuild/src/compiled/00/9c/FileInstallerDestroyBufferRange_009c6d60.cpp','rebuild/src/compiled/00/9c/FileInstallerBufferArray_009c7150.cpp','rebuild/src/compiled/00/9c/FileInstallerClearBuffers_009c5110.cpp','rebuild/src/compiled/00/a6/CCriticalSectionLock_Constructor_00a62920.cpp','rebuild/src/compiled/00/a6/CCriticalSectionLock_Destructor_00a629a0.cpp']
 assert len(sources)==len(set(sources))
 work=Path(tempfile.mkdtemp(prefix='installer-range-os-'));env=check_one.env();objects=[]
 for path in sources+['rebuild/tests/integration/InstallerClearBuffersSharedOS_test.cpp']:
  source=ROOT/path;obj=work/(source.stem+'.obj');r=check_one.cl(['/nologo','/c','/O2','/MT','/DFABLE_MEMORY_ALLOCATOR_OUT_OF_LINE_SUPPORT','/Fo'+str(obj),str(source)],env,work);assert r.returncode==0,r.stdout+r.stderr;objects.append(obj)
 exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
 for omitted in range(len(sources)):
  r=subprocess.run(link+[str(p)for i,p in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True);assert r.returncode!=0,('provider omitted',sources[omitted])
 r=subprocess.run(link+[str(p)for p in objects],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
 r=subprocess.run([str(exe)],cwd=work,capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'INSTALLER_CLEAR_DEFERRED_OS_OK scenarios=4 operations=4' in r.stdout,(r.returncode,r.stdout,r.stderr)
 report=dict(sources=sources,native_sources=len(sources),omitted_provider_checks=len(sources),result=r.stdout.strip(),source_sha256={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest()for p in sources},boundary='actual recovered main/30-class allocator sources compiled with consistent out-of-line support declarations, actual Windows memory and CS; shared production allocator ABI, genuine out-of-line support candidates, no fixture member adapters; actual array RemoveRange/DeallocateMemory, malloc/free array storage, null/live/null owned chunks; shared ClearBuffers executes with actual recursive lock and GlobalAlloc/GlobalFree read/write handles; deferred shutdown and original leave after deleted CS; retained chunks explicitly fixture-released')
 (ROOT/'rebuild/compile-gate/installer-cleanup-clear-buffers-shared-os.json').write_text(json.dumps(report,indent=2)+'\n')
 print(r.stdout.strip());print('providers='+str(len(sources))+' omissions='+str(len(sources)))
if __name__=='__main__':main()
