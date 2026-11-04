"""Native main tracked allocation/free composed with actual Windows memory APIs."""
import argparse,json,subprocess,tempfile
from pathlib import Path
import check_one
from gate_main_allocator_os import SOURCES
ROOT=Path(__file__).resolve().parents[2]
def main():
    parser=argparse.ArgumentParser();parser.add_argument('--allocate-source',default='rebuild/src/compiled/00/9f/CMemoryAllocator_InternalAllocate_009f2a40.cpp');parser.add_argument('--free-source',default='rebuild/src/compiled/00/9f/CMemoryAllocator_InternalDeallocate_009f2920.cpp');args=parser.parse_args()
    sources=SOURCES+['rebuild/src/compiled/00/9f/CMemoryAllocator_GetAllocationSize_009f2390.cpp','rebuild/src/compiled/01/3c/AllocatorRangeData_013caaa4.cpp','rebuild/src/compiled/00/9f/CMemoryAllocator_InitDebugInfo_009f28d0.cpp',args.allocate_source,args.free_source,'work/MainDebugAllocateInternal_candidate.cpp','work/MainDebugAllocateStatic_candidate.cpp']
    work=Path(tempfile.mkdtemp(prefix='main-debug-edge-os-scratch-'));env=check_one.env();objects=[]
    for path in sources+['rebuild/tests/integration/MainDebugEdgeOS_test.cpp']:
        source=ROOT/path;obj=work/(source.stem+'.obj');r=check_one.cl(['/nologo','/c','/O2','/GS','/MT','/Fo'+str(obj),str(source)],env,work);assert r.returncode==0,r.stdout+r.stderr;objects.append(obj)
    exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
    for omitted in range(len(sources)):
        r=subprocess.run(link+[str(p)for i,p in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True);assert r.returncode!=0,('provider omitted',sources[omitted])
    r=subprocess.run(link+[str(p)for p in objects],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(exe)],cwd=work,capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'MAIN_DEBUG_EDGE_OS_OK scenarios=6 operations=768 checks=777'in r.stdout,(r.returncode,r.stdout,r.stderr)
    (ROOT/'rebuild/compile-gate/main-debug-edge-os-scratch.json').write_text(json.dumps(dict(native_sources=len(sources),omitted_provider_checks=len(sources),scenarios=6,operations=768,checks=777,sources=sources,result=r.stdout.strip(),invariants='actual five-argument static debug Allocate, protected debug overload, bool InternalAllocate, main metadata/size/free and all30 native classes; negative sizes-1/-2/-17/minsigned and zero plus positive sizes1..65536; internal clamp to1 for allocation/tracking addition, trailer retains original signed input and free subtracts original bit pattern; residual tracking counters independently predicted through two cycles; fixed/variable boundary236/237, categories0/9; ignored file/function invalid pointers and unsigned lineffffffff; guards/trailer/per-ID counters/NoAllocations/payload/nonoverlap/enum/native validation; three free orders and two cycles; genuine deferred shutdown with live allocations followed by protected free and explicit final shutdown; linked CRT cookie checker unmodified',boundary='scratch C++ debug providers with compiler-generated GS; no promotion; actual zero/negative/successful positive requests; no forced cookie mismatch'),indent=2)+'\n',encoding='utf-8')
    print(r.stdout.strip());print(f'MAIN_DEBUG_EDGE_OS_PASS sources={len(sources)} omitted={len(sources)}')
if __name__=='__main__':main()
