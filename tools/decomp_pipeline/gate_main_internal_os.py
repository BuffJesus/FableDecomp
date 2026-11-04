"""Native main tracked allocation/free composed with actual Windows memory APIs."""
import argparse,json,subprocess,tempfile
from pathlib import Path
import check_one
from gate_main_allocator_os import SOURCES
ROOT=Path(__file__).resolve().parents[2]
def main():
    parser=argparse.ArgumentParser();parser.add_argument('--allocate-source',default='rebuild/src/compiled/00/9f/CMemoryAllocator_InternalAllocate_009f2a40.cpp');parser.add_argument('--free-source',default='rebuild/src/compiled/00/9f/CMemoryAllocator_InternalDeallocate_009f2920.cpp');args=parser.parse_args()
    sources=SOURCES+['rebuild/src/compiled/00/9f/CMemoryAllocator_GetAllocationSize_009f2390.cpp','rebuild/src/compiled/01/3c/AllocatorRangeData_013caaa4.cpp','rebuild/src/compiled/00/9f/CMemoryAllocator_InitDebugInfo_009f28d0.cpp',args.allocate_source,args.free_source]
    work=Path(tempfile.mkdtemp(prefix='main-internal-os-'));env=check_one.env();objects=[]
    for path in sources+['rebuild/tests/integration/MainInternalOS_test.cpp']:
        source=ROOT/path;obj=work/(source.stem+'.obj');r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],env,work);assert r.returncode==0,r.stdout+r.stderr;objects.append(obj)
    exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
    for omitted in range(len(sources)):
        r=subprocess.run(link+[str(p)for i,p in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True);assert r.returncode!=0,('provider omitted',sources[omitted])
    r=subprocess.run(link+[str(p)for p in objects],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(exe)],cwd=work,capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'MAIN_INTERNAL_OS_OK scenarios=6 operations=768 checks=777'in r.stdout,(r.returncode,r.stdout,r.stderr)
    (ROOT/'rebuild/compile-gate/main-internal-os.json').write_text(json.dumps(dict(native_sources=len(sources),omitted_provider_checks=len(sources),scenarios=6,operations=768,checks=777,sources=sources,result=r.stdout.strip(),invariants='actual main tracked allocation/free with real constructor/table/all30 native classes/Win32 memory; independent row selection at fixed/variable boundary236/237, signed negative/zero clamp and categories0/9; guard/trailer/ID/requested metadata and per-ID bytes/NoAllocations compared every operation; payload preservation/nonoverlap, enumeration userPointer-4, native statistics/validation; true/false debug bool; real nested critical sections; forward/reverse/middle frees,two cycles; genuine allocation count causes deferred shutdown with live allocations, no artificial count; explicit final shutdown releases main and class object regions, original retained chunks verified and fixture released',boundary='fixture sets documented current tracking byte and wraps protected internal calls in outer critical section; no recovered public main/aligned wrapper claim; no forced OS failure or engine shutdown registration claim'),indent=2)+'\n',encoding='utf-8')
    print(r.stdout.strip());print(f'MAIN_INTERNAL_OS_PASS sources={len(sources)} omitted={len(sources)}')
if __name__=='__main__':main()
