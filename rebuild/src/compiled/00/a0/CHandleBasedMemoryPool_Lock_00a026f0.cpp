#include "fable_handle_memory_pool.h"
void CHandleBasedMemoryPool::Lock(long handle)
{
    CHeader* header = Handles[handle].Header;
    if (!header->Locked)
    {
        header->Locked = 1;
        ++LockCount;
    }
}
