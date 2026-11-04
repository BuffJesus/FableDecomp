#include "fable_handle_memory_pool.h"

// Retail 00A02710: unlocking an already unlocked area is a no-op.
void CHandleBasedMemoryPool::Unlock(long handle)
{
    CHeader* header = Handles[handle].Header;
    if (header->Locked)
    {
        header->Locked = 0;
        --LockCount;
    }
}
