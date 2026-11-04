// Retail 0x009F26D0: frame statistics are disabled, leaving the lock round trip.
#include "fable_memory_allocator.h"

void CMemoryAllocator::NewFrame()
{
    EnterSynchronisation();
    LeaveSynchronisation();
}
