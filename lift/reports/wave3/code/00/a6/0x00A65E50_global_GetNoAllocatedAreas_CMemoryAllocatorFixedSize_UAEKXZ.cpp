#include <cstddef>

struct CMemoryAllocatorFixedSize_GetNoAllocatedAreas_NodeOverlay {
    std::byte pad_0000[0x04];
    unsigned long field_0004;
    CMemoryAllocatorFixedSize_GetNoAllocatedAreas_NodeOverlay* field_0008;
};

static_assert(offsetof(CMemoryAllocatorFixedSize_GetNoAllocatedAreas_NodeOverlay, field_0004) == 0x04);
static_assert(offsetof(CMemoryAllocatorFixedSize_GetNoAllocatedAreas_NodeOverlay, field_0008) == 0x08);

struct CMemoryAllocatorFixedSize_GetNoAllocatedAreas_Overlay {
    std::byte pad_0000[0x0C1C];
    CMemoryAllocatorFixedSize_GetNoAllocatedAreas_NodeOverlay* field_0C1C;
};

static_assert(offsetof(CMemoryAllocatorFixedSize_GetNoAllocatedAreas_Overlay, field_0C1C) == 0x0C1C);

unsigned long CMemoryAllocatorFixedSize::GetNoAllocatedAreas()
{
    auto* self = reinterpret_cast<CMemoryAllocatorFixedSize_GetNoAllocatedAreas_Overlay*>(this);

    unsigned long total = 0;
    for (auto* node = self->field_0C1C; node != nullptr; node = node->field_0008) {
        total = total + node->field_0004;
    }

    return total;
}