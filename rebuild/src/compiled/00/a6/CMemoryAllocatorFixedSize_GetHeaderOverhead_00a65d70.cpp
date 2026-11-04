#include "candidates/fixed_size_allocator_accessors_compiled.h"

unsigned long CMemoryAllocatorFixedSize::GetHeaderOverhead()
{
    unsigned long total = 0;
    CFixedSizeAllocatorNode* current = PFirstChunk;
    if (current != 0)
    {
        unsigned long headerSize = ChunkHeaderSize;
        do
        {
            total += headerSize + (unsigned long)current->NoAllocatedAreas * 4;
            current = current->PNextChunk;
        } while (current != 0);
    }
    return total;
}
