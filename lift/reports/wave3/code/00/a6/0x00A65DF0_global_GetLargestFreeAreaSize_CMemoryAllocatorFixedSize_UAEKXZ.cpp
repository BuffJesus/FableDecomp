#include <cstddef>

struct CMemoryAllocatorFixedSize_GetLargestFreeAreaSize_NodeOverlay {
    std::byte pad_0000[0x04];
    unsigned long field_0004;
    CMemoryAllocatorFixedSize_GetLargestFreeAreaSize_NodeOverlay* field_0008;
};

static_assert(offsetof(CMemoryAllocatorFixedSize_GetLargestFreeAreaSize_NodeOverlay, field_0004) == 0x04);
static_assert(offsetof(CMemoryAllocatorFixedSize_GetLargestFreeAreaSize_NodeOverlay, field_0008) == 0x08);

struct CMemoryAllocatorFixedSize_GetLargestFreeAreaSize_Overlay {
    std::byte pad_0000[0x0C1C];
    CMemoryAllocatorFixedSize_GetLargestFreeAreaSize_NodeOverlay* field_0C1C;
    std::byte pad_0C20[0x08];
    unsigned long field_0C28;
    std::byte pad_0C2C[0x08];
    unsigned long field_0C34;
};

static_assert(offsetof(CMemoryAllocatorFixedSize_GetLargestFreeAreaSize_Overlay, field_0C1C) == 0x0C1C);
static_assert(offsetof(CMemoryAllocatorFixedSize_GetLargestFreeAreaSize_Overlay, field_0C28) == 0x0C28);
static_assert(offsetof(CMemoryAllocatorFixedSize_GetLargestFreeAreaSize_Overlay, field_0C34) == 0x0C34);

unsigned long CMemoryAllocatorFixedSize::GetLargestFreeAreaSize()
{
    auto* self = reinterpret_cast<CMemoryAllocatorFixedSize_GetLargestFreeAreaSize_Overlay*>(this);

    auto* node = self->field_0C1C;
    if (node != nullptr) {
        do {
            if (self->field_0C34 != node->field_0004) {
                return self->field_0C28;
            }
            node = node->field_0008;
        } while (node != nullptr);
    }

    return 0;
}