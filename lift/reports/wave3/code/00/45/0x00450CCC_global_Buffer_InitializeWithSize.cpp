#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct BufferOverlay {
    void* begin;
    void* current;
    void* end;
};

static_assert(offsetof(BufferOverlay, begin) == 0x0);
static_assert(offsetof(BufferOverlay, current) == 0x4);
static_assert(offsetof(BufferOverlay, end) == 0x8);
static_assert(sizeof(BufferOverlay) == 0xC);

BufferOverlay* __thiscall Buffer_InitializeWithSize(BufferOverlay* self, std::size_t size)
{
    void* allocation = nullptr;

    self->begin = nullptr;
    self->current = nullptr;
    self->end = nullptr;

    if (size != 0) {
        allocation = std::malloc(size);
    }

    self->begin = allocation;
    self->current = allocation;
    self->end = reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(allocation) + size);

    return self;
}