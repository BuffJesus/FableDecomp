#include <cstddef>
#include <cstdint>

struct CMemoryAllocatorFixedSize_GetTotalMemoryControlledByAllocator_NodeOverlay {
    std::byte pad_0000[0x08];
    CMemoryAllocatorFixedSize_GetTotalMemoryControlledByAllocator_NodeOverlay* next;
};

static_assert(offsetof(CMemoryAllocatorFixedSize_GetTotalMemoryControlledByAllocator_NodeOverlay, next) == 0x08);

struct CMemoryAllocatorFixedSize_GetTotalMemoryControlledByAllocator_Overlay {
    std::byte pad_0000[0x0C1C];
    CMemoryAllocatorFixedSize_GetTotalMemoryControlledByAllocator_NodeOverlay* field_0C1C;
    std::byte pad_0C20[0x18];
    unsigned long field_0C38;
    std::byte pad_0C3C[0x08];
    unsigned long field_0C44;
};

static_assert(offsetof(CMemoryAllocatorFixedSize_GetTotalMemoryControlledByAllocator_Overlay, field_0C1C) == 0x0C1C);
static_assert(offsetof(CMemoryAllocatorFixedSize_GetTotalMemoryControlledByAllocator_Overlay, field_0C38) == 0x0C38);
static_assert(offsetof(CMemoryAllocatorFixedSize_GetTotalMemoryControlledByAllocator_Overlay, field_0C44) == 0x0C44);

unsigned long CMemoryAllocatorFixedSize::GetTotalMemoryControlledByAllocator()
{
    auto* self = reinterpret_cast<CMemoryAllocatorFixedSize_GetTotalMemoryControlledByAllocator_Overlay*>(this);

    auto* current = self->field_0C1C;
    unsigned long total = self->field_0C44;

    if (current != nullptr) {
        do {
            current = current->next;
            total = total + self->field_0C38;
        } while (current != nullptr);
    }

    return total;
}