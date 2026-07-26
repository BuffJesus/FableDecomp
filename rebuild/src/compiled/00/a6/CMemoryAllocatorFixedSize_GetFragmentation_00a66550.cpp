#include "candidates/fixed_size_allocator_accessors_compiled.h"

float CMemoryAllocatorFixedSize::GetFragmentation()
{
    return *reinterpret_cast<const float*>(0x0122DEDC);
}
