#include "candidates/fixed_size_allocator_accessors_compiled.h"

unsigned long CMemoryAllocatorFixedSize::GetAllocationSizeIncludingHeader(void*)
{
    return AllocationSizeIncludingHeader;
}
