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
            long areaCount = current->AreaCount;
            current = current->Next;
            total += headerSize + areaCount * 4;
        } while (current != 0);
    }
    return total;
}
