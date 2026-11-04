#include "fable_handle_memory_pool.h"
void CHandleBasedMemoryPool::InternalSetResource(long handle, void* resource)
{
    Handles[handle].Header->Resource=resource;
}
