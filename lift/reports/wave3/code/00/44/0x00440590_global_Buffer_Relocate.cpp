#include <cstddef>
#include <cstdint>
#include <cstring>

struct BufferRelocateOverlay {
    std::byte pad_0[0x4];
    std::uint32_t end;
};

static_assert(offsetof(BufferRelocateOverlay, end) == 0x4);

void* __thiscall Buffer_Relocate(void* self, void* dest, void* src)
{
    auto* const buffer = static_cast<BufferRelocateOverlay*>(self);
    const std::uint32_t src32 = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(src));

    if (buffer->end == src32) {
        buffer->end = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(dest));
        return dest;
    }

    const std::size_t size = static_cast<std::size_t>(buffer->end - src32);
    void* const moved = std::memmove(dest, src, size);
    buffer->end = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(moved)) +
                  static_cast<std::uint32_t>(size);
    return dest;
}