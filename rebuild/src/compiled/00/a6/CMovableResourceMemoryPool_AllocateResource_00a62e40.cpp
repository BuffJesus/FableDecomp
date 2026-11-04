// lib_movable_resource_memory_pool.cpp, retail 00A62E40. The scoped critical
// section encloses allocation and resource binding, including the failure exit.
#include "fable_handle_memory_pool.h"

bool CMovableResourceMemoryPool::AllocateResource(long* handle, CMovableResource* resource,
    unsigned long size, unsigned long flags)
{
    CScopedCritical critical(*this, (bool)(flags & 1));
    if (!critical.Entered)
        return false;
    if (Allocate(handle, size, flags))
    {
        InternalSetResource(*handle, resource);
        return true;
    }
    return false;
}
