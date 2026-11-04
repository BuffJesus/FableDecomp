// Retail 0x009F2550: aggregate slot 9 while holding the shared allocator lock.
#include "fable_memory_allocator.h"

unsigned long CMemoryAllocator::GetNoAllocatedAreas()
{
    unsigned long total = 0;
    EnterSynchronisation();
    for (unsigned long i = 0; i < NoAllocators; ++i)
        total += AllocatorInfo[i].PAllocator->GetNoAllocatedAreas();
    LeaveSynchronisation();
    return total;
}
