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
    allocator->AllocationSizeIncludingHeader = 20;
    allocator->HeaderSize = 8;
    nodes[0].AreaCount = 3;
    nodes[0].Next = &nodes[1];
    nodes[1].AreaCount = 5;
    allocator->ControlledAreas = &nodes[0];
    if (allocator->GetTotalMemoryAllocated() != 176) return 1;
    allocator->ControlledAreas = 0;
    if (allocator->GetTotalMemoryAllocated() != 0) return 2;
    printf("FIXED_ALLOCATOR_TOTAL_ALLOCATED_TEST PASS\n");
    return 0;
}
