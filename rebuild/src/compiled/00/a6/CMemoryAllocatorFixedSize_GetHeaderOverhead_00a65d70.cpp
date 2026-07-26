#include "candidates/fixed_size_allocator_accessors_compiled.h"

unsigned long CMemoryAllocatorFixedSize::GetHeaderOverhead()
{
    unsigned long total = 0;
    CFixedSizeAllocatorNode* current = ControlledAreas;
    if (current != 0)
    {
        unsigned long headerSize = HeaderSize;
        do
        {
            total += headerSize + (unsigned long)current->AreaCount * 4;
            current = current->Next;
        } while (current != 0);
    }
    return total;
}
