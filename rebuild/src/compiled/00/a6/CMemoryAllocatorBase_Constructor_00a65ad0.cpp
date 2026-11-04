// Retail 0x00A65AD0; only the cumulative tracking array is initialized here.
#include "fable_memory_allocator.h"
#include <string.h>

CMemoryAllocatorBase::CMemoryAllocatorBase(unsigned long alignment)
{
    Alignment=alignment;
    switch(alignment) {
    case 4: AlignmentShift=2;break;
    case 8: AlignmentShift=3;break;
    case 16: AlignmentShift=4;break;
    default: AlignmentShift=9999999;break;
    }
}
