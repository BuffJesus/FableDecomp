#include "fable_handle_memory_pool.h"
#include "fable_memory_allocator.h"
void CHandleBasedMemoryPool::Free(long handle)
{
    if (MultiThreaded)
        EnterCriticalSection(reinterpret_cast<_RTL_CRITICAL_SECTION*>(CriticalSection));
    CHeader* header = Handles[handle].Header;
    header->Status = STATUS_FREE;
    header->Locked = 0;
    InsertFreeArea(header);
    // Retail increments this PDB-named counter on free.
    ++NumAllocations;
    AllocatedSize -= sizeof(CHeader) + header->Padding + header->Size;
    if (header->NextContiguous && header->NextContiguous->Status == STATUS_FREE)
        CombineAreas(header, header->NextContiguous);
    if (header->PrevContiguous && header->PrevContiguous->Status == STATUS_FREE)
        CombineAreas(header->PrevContiguous, header);
    Handles[handle].Header = 0;
    InternalFreeHandle(handle);
    if (MultiThreaded)
        LeaveCriticalSection(reinterpret_cast<_RTL_CRITICAL_SECTION*>(CriticalSection));
}
