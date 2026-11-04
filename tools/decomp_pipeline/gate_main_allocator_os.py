"""Real main singleton creation/destruction with all recovered size-class providers."""
import json,subprocess,tempfile
from pathlib import Path
import check_one
ROOT=Path(__file__).resolve().parents[2]
SOURCES=['rebuild/src/compiled/00/a6/CMemoryAllocatorFixedSize_Allocation_00a662a0.cpp',
 'rebuild/src/compiled/00/a6/CMemoryAllocatorFixedSize_Constructor_00a65ff0.cpp',
 'rebuild/src/compiled/00/a6/CMemoryAllocatorFixedSize_Destructor_00a65d20.cpp',
 'rebuild/src/compiled/00/a6/CMemoryAllocatorBase_Constructor_00a65ad0.cpp',
 'rebuild/src/compiled/00/a6/CMemoryAllocatorBase_ConsolidateFreeAreas_00a65b50.cpp',
 'rebuild/src/compiled/00/a6/CMemoryAllocatorBase_OperatorDelete_00a66750.cpp',
 'rebuild/src/compiled/00/a6/CMemoryAllocatorFixedSize_NewChunk_00a65f40.cpp',
 'rebuild/src/compiled/00/a6/CMemoryAllocatorFixedSize_ChunkLifetime_00a66160.cpp',
 'rebuild/src/compiled/00/a6/CMemoryAllocatorFixedSize_ChunkLists_00a65e70.cpp',
 'rebuild/src/compiled/00/a6/CMemoryAllocatorFixedSize_Enumeration_00a660a0.cpp',
 'rebuild/src/compiled/00/a6/CMemoryAllocatorFixedSize_Validate_00a66260.cpp',
 'rebuild/src/compiled/00/9f/CMemoryAllocator_OnFailedAllocation_009f1f00.cpp',
 'rebuild/src/compiled/00/9f/CMemoryAllocator_InternalValidateArea_009f1f40.cpp',
 'rebuild/src/compiled/01/3b/GFMainSingletonCodeData_013b7d58.cpp',
 'rebuild/src/compiled/00/a6/CMemoryAllocatorFixedSize_ConsolidateFreeAreas_00a65d30.cpp',
 'rebuild/src/compiled/00/a6/CMemoryAllocatorFixedSize_GetAllocationSize_00a65d40.cpp',
 'rebuild/src/compiled/00/a6/CMemoryAllocatorFixedSize_GetTotalMemoryControlledByAllocator_00a65d50.cpp',
 'rebuild/src/compiled/00/a6/CMemoryAllocatorFixedSize_GetHeaderOverhead_00a65d70.cpp',
 'rebuild/src/compiled/00/a6/CMemoryAllocatorFixedSize_GetTotalMemoryAllocated_00a65da0.cpp',
 'rebuild/src/compiled/00/a6/CMemoryAllocatorFixedSize_GetAllocationSizeIncludingHeader_00a65de0.cpp',
 'rebuild/src/compiled/00/a6/CMemoryAllocatorFixedSize_GetLargestFreeAreaSize_00a65df0.cpp',
 'rebuild/src/compiled/00/a6/CMemoryAllocatorFixedSize_GetNoFreeAreas_00a65e20.cpp',
 'rebuild/src/compiled/00/a6/CMemoryAllocatorFixedSize_GetNoAllocatedAreas_00a65e50.cpp',
 'rebuild/src/compiled/00/a6/CMemoryAllocatorFixedSize_GetFragmentation_00a66550.cpp',
 'rebuild/src/compiled/00/a6/CMemoryAllocatorVariableSize_AllocateMemoryFromExistingChunks_00a656e0.cpp',
 'rebuild/src/compiled/00/a6/CMemoryAllocatorVariableSize_RemoveFromTree_00a64ef0.cpp',
 'rebuild/src/compiled/00/a6/CMemoryAllocatorVariableSize_RemoveFromTreeFixup_00a64910.cpp',
 'rebuild/src/compiled/00/a6/VariableAllocatorChunk_AllocAreaFromArea_00a65480.cpp',
 'rebuild/src/compiled/00/a6/VariableAllocatorChunk_RemoveAreaFromFreeList_00a65060.cpp',
 'rebuild/src/compiled/00/a6/VariableAllocatorChunk_InsertAreaIntoFreeList_00a650f0.cpp',
 'rebuild/src/compiled/00/a6/CMemoryAllocatorVariableSize_InsertIntoTree_00a64e70.cpp',
 'rebuild/src/compiled/00/a6/CMemoryAllocatorVariableSize_InsertFixup_00a64740.cpp',
 'rebuild/src/compiled/00/a6/CMemoryAllocatorVariableSize_TreeQueries_00a64c10.cpp',
 'rebuild/src/compiled/01/3d/VariableAllocatorAlignmentData_013d28d4.cpp',
 'rebuild/src/compiled/00/a6/VariableAllocatorChunk_FreeArea_00a658e0.cpp',
 'rebuild/src/compiled/00/a6/CMemoryAllocatorVariableSize_AllocationSizes_00a651b0.cpp',
 'rebuild/src/compiled/00/a6/CMemoryAllocatorVariableSize_GetLargestFreeAreaSize_00a64500.cpp',
 'rebuild/src/compiled/00/a6/CMemoryAllocatorVariableSize_GetFragmentation_00a64590.cpp',
 'rebuild/src/compiled/00/a6/CMemoryAllocatorVariableSize_Counts_00a64530.cpp',
 'rebuild/src/compiled/00/a6/CMemoryAllocatorVariableSize_Enumeration_00a65260.cpp',
 'rebuild/src/compiled/00/a6/CMemoryAllocatorVariableSize_Validate_00a65330.cpp',
 'rebuild/src/compiled/00/a6/CMemoryAllocatorVariableSize_Allocate_00a659c0.cpp',
 'rebuild/src/compiled/00/a6/CMemoryAllocatorVariableSize_Deallocate_00a65a50.cpp',
 'rebuild/src/compiled/00/a6/CMemoryAllocatorVariableSize_DeleteChunk_00a65390.cpp',
 'rebuild/src/compiled/00/a6/CMemoryAllocatorVariableSize_Destructor_00a65690.cpp',
 'rebuild/src/compiled/00/a6/CMemoryAllocatorVariableSize_Constructor_00a65930.cpp',
 'rebuild/src/compiled/00/a6/CMemoryAllocatorVariableSize_NewChunk_00a65720.cpp',
 'rebuild/src/compiled/00/a6/CMemoryAllocatorVariableSize_NewChunkDemandPaged_00a65830.cpp',
 'rebuild/src/compiled/00/a6/VariableAllocatorChunk_Init_00a65410.cpp',
 'rebuild/src/compiled/00/a6/CMemoryAllocatorBase_OperatorNew_00a66730.cpp',
 'rebuild/src/compiled/00/a6/CMemoryAllocatorVariableSize_Totals_00a64db0.cpp',
 'rebuild/src/compiled/00/9f/CMemoryAllocator_Constructor_009f20b0.cpp',
 'rebuild/src/compiled/00/9f/CMemoryAllocator_Destructor_009f2310.cpp',
 'rebuild/src/compiled/00/9f/CMemoryAllocator_Create_009f2870.cpp',
 'rebuild/src/compiled/00/9f/MemoryAllocatorShutdown_009f2d20.cpp',
 'rebuild/src/compiled/00/9f/InitializeMemorySystem_009f1d20.cpp',
 'rebuild/src/compiled/01/39/MemoryAllocatorData_01396868.cpp',
 'rebuild/src/compiled/00/9f/CMemoryAllocator_GetNoAllocatedAreas_009f2550.cpp',
 'rebuild/src/compiled/00/9f/CMemoryAllocator_GetTotalMemoryAllocated_009f2450.cpp']

def main():
    sources=SOURCES
    work=Path(tempfile.mkdtemp(prefix='main-allocator-os-'));env=check_one.env();objects=[]
    for path in sources+['rebuild/tests/integration/MainAllocatorOS_test.cpp']:
        source=ROOT/path;obj=work/(source.stem+'.obj');r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],env,work);assert r.returncode==0,r.stdout+r.stderr;objects.append(obj)
    exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
    for omitted in range(len(sources)):
        r=subprocess.run(link+[str(p)for i,p in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True);assert r.returncode!=0,('provider omitted',sources[omitted])
    r=subprocess.run(link+[str(p)for p in objects],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(exe)],cwd=work,capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'MAIN_ALLOCATOR_OS_OK scenarios=6'in r.stdout,(r.returncode,r.stdout,r.stderr)
    (ROOT/'rebuild/compile-gate/main-allocator-os.json').write_text(json.dumps(dict(grade='NATIVE_OS_LIFETIME_PASS',native_sources=len(sources),omitted_provider_checks=len(sources),scenarios=6,sources=sources,result=r.stdout.strip(),invariants='real Create/main constructor/30-row static table/initializer/critical sections/CRT malloc/native size-class constructors/vtables; exact lookup boundaries0..257 and tracking categories0/9; real fixed/variable allocations and frees in two cycles/three free orders or live payload at shutdown; native counts/totals/enumeration/validation and payload/alignment/nonoverlap; real deferred shutdown with explicit documented main counter; class objects and singleton VirtualFree; original main destructor retains class-owned chunks/payload, fixture explicitly releases retained OS memory afterward',boundary='actual Win32 APIs, no import replacements or algorithm substitutes; initializer runs in isolated directory with absent boot.ini; allocations intentionally through native size-class methods, not yet recovered main static Allocate/Deallocate; NoAllocations manually set only to test deferred branch; shutdown helper directly invoked, no engine exit registration claim; no forced OS failure'),indent=2)+'\n',encoding='utf-8')
    print(r.stdout.strip());print(f'MAIN_ALLOCATOR_OS_PASS sources={len(sources)} omitted={len(sources)}')
if __name__=='__main__':main()
