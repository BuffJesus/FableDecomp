#include "candidates/fixed_size_allocator_accessors_compiled.h"

unsigned long CMemoryAllocatorFixedSize::GetTotalMemoryControlledByAllocator()
{
    CFixedSizeAllocatorNode* current = PFirstChunk;
    unsigned long total = ExtraMemUsedByOS;
    if (current != 0)
    {
        do
        {
            current = current->PNextChunk;
            total += ChunkSize;
        } while (current != 0);
    }
    return total;
}
