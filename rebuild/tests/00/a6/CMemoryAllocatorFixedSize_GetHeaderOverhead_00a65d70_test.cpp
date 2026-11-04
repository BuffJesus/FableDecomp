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
    allocator->ChunkHeaderSize = 12;
    nodes[0].NoAllocatedAreas = 3;
    nodes[0].PNextChunk = &nodes[1];
    nodes[1].NoAllocatedAreas = 5;
    allocator->PFirstChunk = &nodes[0];
    if (allocator->CMemoryAllocatorFixedSize::GetHeaderOverhead() != 56) return 1;
    printf("FIXED_ALLOCATOR_HEADER_OVERHEAD_TEST PASS\n");
    return 0;
}
