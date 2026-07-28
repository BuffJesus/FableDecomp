#include <cstddef>
#include <cstring>

struct BufferOverlay {
    std::byte _pad0[0x4];
    void* end;
};

static_assert(offsetof(BufferOverlay, end) == 0x4);

void* __thiscall Buffer_MoveData(BufferOverlay* self, void* destination, void* source) {
    if (self->end == source) {
        self->end = destination;
        return destination;
    }

    const std::size_t size =
        static_cast<std::size_t>(
            static_cast<std::byte*>(self->end) - static_cast<std::byte*>(source));

    void* const moved = std::memmove(destination, source, size);
    self->end = static_cast<void*>(static_cast<std::byte*>(moved) + size);
    return destination;
}