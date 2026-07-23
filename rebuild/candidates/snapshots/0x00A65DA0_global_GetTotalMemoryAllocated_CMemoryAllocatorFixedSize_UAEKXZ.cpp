#include <cstddef>

struct CMemoryAllocatorFixedSize_GetTotalMemoryAllocated_NodeOverlay {
    std::byte pad_0000[0x04];
    unsigned long field_0004;
    CMemoryAllocatorFixedSize_GetTotalMemoryAllocated_NodeOverlay* field_0008;
};

static_assert(offsetof(CMemoryAllocatorFixedSize_GetTotalMemoryAllocated_NodeOverlay, field_0004) == 0x04);
static_assert(offsetof(CMemoryAllocatorFixedSize_GetTotalMemoryAllocated_NodeOverlay, field_0008) == 0x08);

struct CMemoryAllocatorFixedSize_GetTotalMemoryAllocated_Overlay {
    std::byte pad_0000[0x0C1C];
    CMemoryAllocatorFixedSize_GetTotalMemoryAllocated_NodeOverlay* field_0C1C;
    std::byte pad_0C20[0x0C];
    unsigned long field_0C2C;
    unsigned long field_0C30;
};

static_assert(offsetof(CMemoryAllocatorFixedSize_GetTotalMemoryAllocated_Overlay, field_0C1C) == 0x0C1C);
static_assert(offsetof(CMemoryAllocatorFixedSize_GetTotalMemoryAllocated_Overlay, field_0C2C) == 0x0C2C);
static_assert(offsetof(CMemoryAllocatorFixedSize_GetTotalMemoryAllocated_Overlay, field_0C30) == 0x0C30);

unsigned long CMemoryAllocatorFixedSize::GetTotalMemoryAllocated()
{
    auto* self = reinterpret_cast<CMemoryAllocatorFixedSize_GetTotalMemoryAllocated_Overlay*>(this);

    auto* node = self->field_0C1C;
    unsigned long total = 0;
    if (node != nullptr) {
        do {
            auto const currentField0004 = node->field_0004;
            node = node->field_0008;
            total = total + currentField0004 * self->field_0C2C + self->field_0C30;
        } while (node != nullptr);
    }

    return total;
}