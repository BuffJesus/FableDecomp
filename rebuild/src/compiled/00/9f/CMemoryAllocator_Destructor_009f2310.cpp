// Retail 0x009F2310: release audit storage, size classes, lookup tables and lock.
#include "fable_memory_allocator.h"
#include <stdlib.h>
extern "C" __declspec(dllimport) void __stdcall DeleteCriticalSection(_RTL_CRITICAL_SECTION*);

CMemoryAllocator::~CMemoryAllocator()
{
    if(MemoryAuditBuffer)
        free(MemoryAuditBuffer);
    for(unsigned long i=0;i<NoAllocators;++i)
        CMemoryAllocatorBase::operator delete(AllocatorInfo[i].PAllocator);
    for(unsigned long category=0;category<15;++category)
        if(AllocSizeToAllocator[category])
            free(AllocSizeToAllocator[category]);
    DeleteCriticalSection(&g_CMemoryAllocatorCriticalSection_013CAA70);
}
