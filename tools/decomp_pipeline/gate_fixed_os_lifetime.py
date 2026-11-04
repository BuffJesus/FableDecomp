"""Entire recovered fixed allocator linked against actual Win32 process heap."""
import argparse,json,subprocess,tempfile
from pathlib import Path
import check_one
from gate_fixed_allocator_lifetime import LEAVES
ROOT=Path(__file__).resolve().parents[2]
def main():
    parser=argparse.ArgumentParser();parser.add_argument('--source',default='rebuild/src/compiled/00/a6/CMemoryAllocatorFixedSize_Allocation_00a662a0.cpp');args=parser.parse_args()
    sources=[args.source]+['rebuild/src/compiled/'+path for path in [
        '00/a6/CMemoryAllocatorFixedSize_Constructor_00a65ff0.cpp','00/a6/CMemoryAllocatorFixedSize_Destructor_00a65d20.cpp',
        '00/a6/CMemoryAllocatorBase_Constructor_00a65ad0.cpp','00/a6/CMemoryAllocatorBase_ConsolidateFreeAreas_00a65b50.cpp','00/a6/CMemoryAllocatorBase_OperatorDelete_00a66750.cpp',
        '00/a6/CMemoryAllocatorFixedSize_NewChunk_00a65f40.cpp','00/a6/CMemoryAllocatorFixedSize_ChunkLifetime_00a66160.cpp','00/a6/CMemoryAllocatorFixedSize_ChunkLists_00a65e70.cpp',
        '00/a6/CMemoryAllocatorFixedSize_Enumeration_00a660a0.cpp','00/a6/CMemoryAllocatorFixedSize_Validate_00a66260.cpp',
        '00/9f/CMemoryAllocator_OnFailedAllocation_009f1f00.cpp','00/9f/CMemoryAllocator_InternalValidateArea_009f1f40.cpp','01/3b/GFMainSingletonCodeData_013b7d58.cpp']]
    sources+=['rebuild/src/compiled/00/a6/CMemoryAllocatorFixedSize_%s_%08x.cpp'%(name,address)for name,address in LEAVES]
    work=Path(tempfile.mkdtemp(prefix='fixed-os-lifetime-'));env=check_one.env();objects=[]
    for path in sources+['rebuild/tests/integration/FixedOSLifetime_test.cpp']:
        source=ROOT/path;obj=work/(source.stem+'.obj');r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],env,work);assert r.returncode==0,r.stdout+r.stderr;objects.append(obj)
    exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
    for omitted in range(len(sources)):
        r=subprocess.run(link+[str(p)for i,p in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True);assert r.returncode!=0,('provider omitted',sources[omitted])
    r=subprocess.run(link+[str(p)for p in objects],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(exe)],capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'FIXED_OS_LIFETIME_OK scenarios=72'in r.stdout,(r.returncode,r.stdout,r.stderr)
    (ROOT/'rebuild/compile-gate/fixed-os-lifetime.json').write_text(json.dumps(dict(grade='NATIVE_OS_LIFETIME_PASS',native_sources=len(sources),omitted_provider_checks=len(sources),scenarios=72,sources=sources,result=r.stdout.strip(),invariants='actual Win32 HeapAlloc/HeapSize/HeapFree without import replacement; real constructor, all native vtable providers and shipping validation; align4/8/16, requested area16/60, minimum128/256, forward/reverse/permuted frees, two allocation/free cycles or live allocations at destruction; payload preservation/nonoverlap and alignment; OS extents; independent partition/list/count/total/header/size/enumeration checks every operation; current membership and signed empty counter transitions; original destructor preserves chunks and payload, fixture explicitly releases retained OS chunks after destruction',boundary='successful process-heap paths; no forced OS failure; GetFragmentation provider linked but absolute retail constant is not exercised; no allocator-object deleting thunk or main singleton construction claim'),indent=2)+'\n',encoding='utf-8')
    print(r.stdout.strip());print(f'FIXED_OS_LIFETIME_PASS sources={len(sources)} omitted={len(sources)}')
if __name__=='__main__':main()
