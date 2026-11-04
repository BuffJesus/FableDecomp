// lib_movable_resource_memory_pool.cpp. Ego_r constructor / SetMovableResource;
// retail 00A62E20 / 00A62EB0. The base constructor owns pool initialization.
#include "fable_handle_memory_pool.h"

CMovableResourceMemoryPool::CMovableResourceMemoryPool()
{
}

void CMovableResourceMemoryPool::SetMovableResource(long handle, CMovableResource* resource)
{
    InternalSetResource(handle, resource);
}
