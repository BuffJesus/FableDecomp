#include <stdio.h>
#include <string.h>
#include "candidates/fixed_size_allocator_accessors_compiled.h"

int main()
{
    fable_u8 storage[0xC4C];
    CFixedSizeAllocatorNode nodes[2];
    memset(storage, 0, sizeof(storage));
    memset(nodes, 0, sizeof(nodes));
    CMemoryAllocatorFixedSize* allocator = reinterpret_cast<CMemoryAllocatorFixedSize*>(storage);
    allocator->AreaSize = 64;
    allocator->NoAreasPerChunk = 8;
    nodes[0].NoAllocatedAreas = 8;
    nodes[0].PNextChunk = &nodes[1];
    nodes[1].NoAllocatedAreas = 7;
    allocator->PFirstChunk = &nodes[0];
    if (allocator->CMemoryAllocatorFixedSize::GetLargestFreeAreaSize() != 64) return 1;
    nodes[1].NoAllocatedAreas = 8;
    if (allocator->CMemoryAllocatorFixedSize::GetLargestFreeAreaSize() != 0) return 2;
    printf("FIXED_ALLOCATOR_LARGEST_FREE_TEST PASS\n");
    return 0;
}
