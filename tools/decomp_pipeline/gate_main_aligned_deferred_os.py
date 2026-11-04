"""Native aligned wrappers composed with actual main/class allocators and Win32 APIs."""
import argparse,json,subprocess,tempfile
from pathlib import Path
import check_one
from gate_main_allocator_os import SOURCES
ROOT=Path(__file__).resolve().parents[2]
def main():
    parser=argparse.ArgumentParser();parser.add_argument('--aligned-source',default='rebuild/src/compiled/00/9f/CMemoryAllocator_Aligned_009f2be0.cpp');parser.add_argument('--allocate-source',default='rebuild/src/compiled/00/9f/CMemoryAllocator_InternalAllocate_009f2a40.cpp');parser.add_argument('--free-source',default='rebuild/src/compiled/00/9f/CMemoryAllocator_InternalDeallocate_009f2920.cpp');args=parser.parse_args()
    sources=SOURCES+['rebuild/src/compiled/00/9f/CMemoryAllocator_GetAllocationSize_009f2390.cpp','rebuild/src/compiled/01/3c/AllocatorRangeData_013caaa4.cpp','rebuild/src/compiled/00/9f/CMemoryAllocator_InitDebugInfo_009f28d0.cpp',args.allocate_source,args.free_source,args.aligned_source]
    work=Path(tempfile.mkdtemp(prefix='main-aligned-deferred-os-'));env=check_one.env();objects=[]
    for path in sources+['rebuild/tests/integration/MainAlignedDeferredOS_test.cpp']:
        source=ROOT/path;obj=work/(source.stem+'.obj');r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],env,work);assert r.returncode==0,r.stdout+r.stderr;objects.append(obj)
    exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
    for omitted in range(len(sources)):
        r=subprocess.run(link+[str(p)for i,p in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True);assert r.returncode!=0,('provider omitted',sources[omitted])
    r=subprocess.run(link+[str(p)for p in objects],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(exe)],cwd=work,capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'MAIN_ALIGNED_DEFERRED_OS_OK scenarios=4 operations=4'in r.stdout,(r.returncode,r.stdout,r.stderr)
    (ROOT/'rebuild/compile-gate/main-aligned-deferred-os.json').write_text(json.dumps(dict(native_sources=len(sources),omitted_provider_checks=len(sources),scenarios=4,operations=4,sources=sources,result=r.stdout.strip(),invariants='actual lazy creation by aligned allocation, sizes1/236/4096/65536 and alignments1/16/256/4096; genuine live allocation defers shutdown; final aligned release destroys all allocator objects and singleton, then executes original LeaveCriticalSection after real DeleteCriticalSection; retained original chunks fixture released; four create/shutdown cycles',boundary='actual unmodified Windows imports, single thread; forced allocation failure pending'),indent=2)+'\n',encoding='utf-8')
    print(r.stdout.strip());print(f'MAIN_ALIGNED_DEFERRED_OS_PASS sources={len(sources)} omitted={len(sources)}')
if __name__=='__main__':main()
