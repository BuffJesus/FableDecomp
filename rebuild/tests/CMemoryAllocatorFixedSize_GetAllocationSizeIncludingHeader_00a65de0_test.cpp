#include <stdio.h>
#include <string.h>
#include "candidates/fixed_size_allocator_accessors_compiled.h"

int main()
{
    fable_u8 storage[0xC48];
    memset(storage, 0, sizeof(storage));
    CMemoryAllocatorFixedSize* allocator = reinterpret_cast<CMemoryAllocatorFixedSize*>(storage);
    allocator->AllocationSizeIncludingHeader = 112;
    if (allocator->GetAllocationSizeIncludingHeader(0) != 112) return 1;
    printf("FIXED_ALLOCATOR_SIZE_WITH_HEADER_TEST PASS\n");
    return 0;
}
