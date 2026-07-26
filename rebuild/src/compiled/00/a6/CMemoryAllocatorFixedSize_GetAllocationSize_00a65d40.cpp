#include "candidates/fixed_size_allocator_accessors_compiled.h"

unsigned long CMemoryAllocatorFixedSize::GetAllocationSize(void*)
{
    return AllocationSize;
}
