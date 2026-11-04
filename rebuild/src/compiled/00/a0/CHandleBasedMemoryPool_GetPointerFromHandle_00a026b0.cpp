#include "fable_handle_memory_pool.h"

// Retail 00A026B0: the payload immediately follows its allocation header.
void* CHandleBasedMemoryPool::GetPointerFromHandle(long handle)
{
    return Handles[handle].Header + 1;
}

// Retail 00A026D0: report the allocation's size without its tail padding.
unsigned long CHandleBasedMemoryPool::GetSizeFromHandle(long handle)
{
    return Handles[handle].Header->Size;
}
