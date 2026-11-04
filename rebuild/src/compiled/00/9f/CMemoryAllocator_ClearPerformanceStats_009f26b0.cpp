// CMemoryAllocator::ClearPerformanceStats, retail 0x009F26B0
// (FableWin ?ClearPerformanceStats@CMemoryAllocator@@QAEXXZ): the per-allocator
// statistics are compiled out of retail (FableWin guards them with the
// UsePerformanceStats constant), leaving only the lock round trip.
#include "fable_memory_allocator.h"

void CMemoryAllocator::ClearPerformanceStats()
{
    EnterSynchronisation();
    LeaveSynchronisation();
}