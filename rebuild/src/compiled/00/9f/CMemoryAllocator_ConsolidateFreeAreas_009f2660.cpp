// CMemoryAllocator::ConsolidateFreeAreas, retail 0x009F2660
// (FableWin ?ConsolidateFreeAreas@CMemoryAllocator@@QAEXXZ): under the allocator
// lock, ask every size-class allocator to merge its free areas. (FableWin's
// debug-only InternalValidate calls are compiled out.)
#include "fable_memory_allocator.h"

void CMemoryAllocator::ConsolidateFreeAreas()
{
    EnterSynchronisation();
    for (unsigned long i = 0; i < NoAllocators; ++i)
        AllocatorInfo[i].PAllocator->ConsolidateFreeAreas();
    LeaveSynchronisation();
}