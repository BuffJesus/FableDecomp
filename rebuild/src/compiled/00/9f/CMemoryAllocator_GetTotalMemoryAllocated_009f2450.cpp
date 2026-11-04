// Retail 0x009F2450: aggregate slot 7 while holding the shared allocator lock.
#include "fable_memory_allocator.h"

unsigned long CMemoryAllocator::GetTotalMemoryAllocated()
{
    unsigned long total = 0;
    EnterSynchronisation();
    for (unsigned long i = 0; i < NoAllocators; ++i)
        total += AllocatorInfo[i].PAllocator->GetTotalMemoryAllocated();
    LeaveSynchronisation();
    return total;
}
