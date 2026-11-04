"""Native aligned wrappers composed with actual main/class allocators and Win32 APIs."""
import argparse,json,subprocess,tempfile
from pathlib import Path
import check_one
from gate_main_allocator_os import SOURCES
ROOT=Path(__file__).resolve().parents[2]
def main():
    parser=argparse.ArgumentParser();parser.add_argument('--aligned-source',default='rebuild/src/compiled/00/9f/CMemoryAllocator_Aligned_009f2be0.cpp');parser.add_argument('--allocate-source',default='rebuild/src/compiled/00/9f/CMemoryAllocator_InternalAllocate_009f2a40.cpp');parser.add_argument('--free-source',default='rebuild/src/compiled/00/9f/CMemoryAllocator_InternalDeallocate_009f2920.cpp');args=parser.parse_args()
    sources=SOURCES+['rebuild/src/compiled/00/9f/CMemoryAllocator_GetAllocationSize_009f2390.cpp','rebuild/src/compiled/01/3c/AllocatorRangeData_013caaa4.cpp','rebuild/src/compiled/00/9f/CMemoryAllocator_InitDebugInfo_009f28d0.cpp',args.allocate_source,args.free_source,args.aligned_source]
    work=Path(tempfile.mkdtemp(prefix='main-aligned-os-'));env=check_one.env();objects=[]
    for path in sources+['rebuild/tests/integration/MainAlignedOS_test.cpp']:
        source=ROOT/path;obj=work/(source.stem+'.obj');r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],env,work);assert r.returncode==0,r.stdout+r.stderr;objects.append(obj)
    exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
    for omitted in range(len(sources)):
        r=subprocess.run(link+[str(p)for i,p in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True);assert r.returncode!=0,('provider omitted',sources[omitted])
    r=subprocess.run(link+[str(p)for p in objects],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(exe)],cwd=work,capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'MAIN_ALIGNED_OS_OK scenarios=3 operations=384 checks=387'in r.stdout,(r.returncode,r.stdout,r.stderr)
    (ROOT/'rebuild/compile-gate/main-aligned-os.json').write_text(json.dumps(dict(native_sources=len(sources),omitted_provider_checks=len(sources),scenarios=3,operations=384,checks=387,sources=sources,result=r.stdout.strip(),invariants='actual public aligned wrappers composed with main tracked allocation/free and all fixed/variable providers; alignments1 through4096, zero/small/boundary/large payloads; prefix round trip, aligned payload fits original request; guards/trailer/row/tracking metadata and all live payloads every operation; counters/enum/native statistics/validation; two cycles and three free orders; ordinary explicit shutdown and retained chunk cleanup',boundary='actual unmodified Windows imports; deferred final aligned release and forced failure behavior pending'),indent=2)+'\n',encoding='utf-8')
    print(r.stdout.strip());print(f'MAIN_ALIGNED_OS_PASS sources={len(sources)} omitted={len(sources)}')
if __name__=='__main__':main()
