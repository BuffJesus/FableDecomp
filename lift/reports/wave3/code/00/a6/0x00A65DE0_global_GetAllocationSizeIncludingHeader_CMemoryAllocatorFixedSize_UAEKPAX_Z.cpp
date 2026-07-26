#include <cstddef>

struct CMemoryAllocatorFixedSize_GetAllocationSizeIncludingHeader_Overlay {
    std::byte pad_0000[0x0C2C];
    unsigned long field_0C2C;
};

static_assert(offsetof(CMemoryAllocatorFixedSize_GetAllocationSizeIncludingHeader_Overlay, field_0C2C) == 0x0C2C);

unsigned long CMemoryAllocatorFixedSize::GetAllocationSizeIncludingHeader(void* /*pAllocation*/)
{
    auto const* self = reinterpret_cast<CMemoryAllocatorFixedSize_GetAllocationSizeIncludingHeader_Overlay const*>(this);
    return self->field_0C2C;
}