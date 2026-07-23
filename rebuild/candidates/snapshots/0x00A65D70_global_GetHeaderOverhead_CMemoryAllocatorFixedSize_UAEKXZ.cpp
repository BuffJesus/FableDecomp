#include <cstddef>

struct CMemoryAllocatorFixedSize_GetHeaderOverhead_NodeOverlay {
    std::byte pad_0000[0x04];
    unsigned long field_0004;
    CMemoryAllocatorFixedSize_GetHeaderOverhead_NodeOverlay* field_0008;
};

static_assert(offsetof(CMemoryAllocatorFixedSize_GetHeaderOverhead_NodeOverlay, field_0004) == 0x04);
static_assert(offsetof(CMemoryAllocatorFixedSize_GetHeaderOverhead_NodeOverlay, field_0008) == 0x08);

struct CMemoryAllocatorFixedSize_GetHeaderOverhead_Overlay {
    std::byte pad_0000[0x0C1C];
    CMemoryAllocatorFixedSize_GetHeaderOverhead_NodeOverlay* field_0C1C;
    std::byte pad_0C20[0x10];
    unsigned long field_0C30;
};

static_assert(offsetof(CMemoryAllocatorFixedSize_GetHeaderOverhead_Overlay, field_0C1C) == 0x0C1C);
static_assert(offsetof(CMemoryAllocatorFixedSize_GetHeaderOverhead_Overlay, field_0C30) == 0x0C30);

unsigned long CMemoryAllocatorFixedSize::GetHeaderOverhead()
{
    auto* self = reinterpret_cast<CMemoryAllocatorFixedSize_GetHeaderOverhead_Overlay*>(this);

    unsigned long total = 0;
    auto* node = self->field_0C1C;
    if (node != nullptr) {
        do {
            auto const currentField0004 = node->field_0004;
            node = node->field_0008;
            total = total + self->field_0C30 + currentField0004 * 4;
        } while (node != nullptr);
    }

    return total;
}