#include <cstddef>

struct CMemoryAllocatorFixedSize_GetNoFreeAreas_NodeOverlay {
    std::byte pad_0000[0x04];
    unsigned long field_0004;
    CMemoryAllocatorFixedSize_GetNoFreeAreas_NodeOverlay* field_0008;
};

static_assert(offsetof(CMemoryAllocatorFixedSize_GetNoFreeAreas_NodeOverlay, field_0004) == 0x04);
static_assert(offsetof(CMemoryAllocatorFixedSize_GetNoFreeAreas_NodeOverlay, field_0008) == 0x08);

struct CMemoryAllocatorFixedSize_GetNoFreeAreas_Overlay {
    std::byte pad_0000[0x0C1C];
    CMemoryAllocatorFixedSize_GetNoFreeAreas_NodeOverlay* field_0C1C;
    std::byte pad_0C20[0x14];
    unsigned long field_0C34;
};

static_assert(offsetof(CMemoryAllocatorFixedSize_GetNoFreeAreas_Overlay, field_0C1C) == 0x0C1C);
static_assert(offsetof(CMemoryAllocatorFixedSize_GetNoFreeAreas_Overlay, field_0C34) == 0x0C34);

unsigned long CMemoryAllocatorFixedSize::GetNoFreeAreas()
{
    auto* self = reinterpret_cast<CMemoryAllocatorFixedSize_GetNoFreeAreas_Overlay*>(this);

    auto* node = self->field_0C1C;
    unsigned long total = 0;
    if (node != nullptr) {
        do {
            auto const currentField0004 = node->field_0004;
            node = node->field_0008;
            total = total + (self->field_0C34 - currentField0004);
        } while (node != nullptr);
    }

    return total;
}