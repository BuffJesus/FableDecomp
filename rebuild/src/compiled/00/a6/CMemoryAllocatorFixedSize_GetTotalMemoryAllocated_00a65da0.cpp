#include "candidates/fixed_size_allocator_accessors_compiled.h"

unsigned long CMemoryAllocatorFixedSize::GetTotalMemoryAllocated()
{
    unsigned long total = 0;
    CFixedSizeAllocatorNode* current = PFirstChunk;
    while (current != 0)
    {
        total += current->NoAllocatedAreas * StatAreaSize + ChunkHeaderSize;
        current = current->PNextChunk;
    }
    return total;
}
