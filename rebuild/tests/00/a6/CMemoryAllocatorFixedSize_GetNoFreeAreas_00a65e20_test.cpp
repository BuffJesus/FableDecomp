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
    allocator->NoAreasPerChunk = 10;
    nodes[0].NoAllocatedAreas = 7;
    nodes[0].PNextChunk = &nodes[1];
    nodes[1].NoAllocatedAreas = 4;
    allocator->PFirstChunk = &nodes[0];
    if (allocator->CMemoryAllocatorFixedSize::GetNoFreeAreas() != 9) return 1;
    printf("FIXED_ALLOCATOR_NO_FREE_AREAS_TEST PASS\n");
    return 0;
}
