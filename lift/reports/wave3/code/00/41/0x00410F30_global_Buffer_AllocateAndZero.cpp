#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct BufferOverlay {
    void* base;           // +0x00
    std::byte* current;   // +0x04
    std::byte* end;       // +0x08
};

static_assert(sizeof(void*) == 4);
static_assert(offsetof(BufferOverlay, base) == 0x00);
static_assert(offsetof(BufferOverlay, current) == 0x04);
static_assert(offsetof(BufferOverlay, end) == 0x08);

BufferOverlay* __thiscall Buffer_AllocateAndZero(BufferOverlay* self, std::uint32_t size) {
    std::uint32_t* allocation = nullptr;

    self->base = nullptr;
    self->current = nullptr;
    self->end = nullptr;

    if (size != 0) {
        allocation = static_cast<std::uint32_t*>(std::malloc(size));
    }

    self->end = reinterpret_cast<std::byte*>(reinterpret_cast<std::uintptr_t>(allocation) + size);

    std::uint32_t dwordCount = size >> 2;
    self->base = allocation;

    std::uint32_t* dwordCursor = allocation;
    for (; dwordCount != 0; dwordCount = dwordCount - 1) {
        *dwordCursor = 0;
        dwordCursor = dwordCursor + 1;
    }

    for (dwordCount = size & 3; dwordCount != 0; dwordCount = dwordCount - 1) {
        *reinterpret_cast<std::byte*>(dwordCursor) = std::byte{0};
        dwordCursor = reinterpret_cast<std::uint32_t*>(
            reinterpret_cast<std::uintptr_t>(dwordCursor) + 1
        );
    }

    self->current = reinterpret_cast<std::byte*>(reinterpret_cast<std::uintptr_t>(allocation) + size);
    return self;
}