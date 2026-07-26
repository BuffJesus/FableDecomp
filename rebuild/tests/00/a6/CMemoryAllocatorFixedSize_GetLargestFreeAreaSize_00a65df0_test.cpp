#include <stdio.h>
#include <string.h>
#include "candidates/fixed_size_allocator_accessors_compiled.h"

int main()
{
    fable_u8 storage[0xC48];
    CFixedSizeAllocatorNode nodes[2];
    memset(storage, 0, sizeof(storage));
    memset(nodes, 0, sizeof(nodes));
    CMemoryAllocatorFixedSize* allocator = reinterpret_cast<CMemoryAllocatorFixedSize*>(storage);
    allocator->AllocationSize = 64;
    allocator->AreaCapacity = 8;
    nodes[0].AreaCount = 8;
    nodes[0].Next = &nodes[1];
    nodes[1].AreaCount = 7;
    allocator->ControlledAreas = &nodes[0];
    if (allocator->GetLargestFreeAreaSize() != 64) return 1;
    nodes[1].AreaCount = 8;
    if (allocator->GetLargestFreeAreaSize() != 0) return 2;
    printf("FIXED_ALLOCATOR_LARGEST_FREE_TEST PASS\n");
    return 0;
}
