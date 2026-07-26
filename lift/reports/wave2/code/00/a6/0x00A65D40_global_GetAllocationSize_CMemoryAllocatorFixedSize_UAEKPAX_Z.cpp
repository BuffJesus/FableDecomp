#include <cstddef>

struct CMemoryAllocatorFixedSize_GetAllocationSize_Overlay {
    std::byte pad_0000[0xC28];
    unsigned long field_0C28;
};

static_assert(offsetof(CMemoryAllocatorFixedSize_GetAllocationSize_Overlay, field_0C28) == 0x0C28);

unsigned long CMemoryAllocatorFixedSize::GetAllocationSize(void* /*pAllocation*/)
{
    auto const* self = reinterpret_cast<CMemoryAllocatorFixedSize_GetAllocationSize_Overlay const*>(this);
    return self->field_0C28;
}