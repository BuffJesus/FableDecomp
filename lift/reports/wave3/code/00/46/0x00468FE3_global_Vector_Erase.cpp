#include <cstddef>
#include <cstdint>

struct VectorEraseOverlay {
    std::byte pad_0x00[0x04];
    std::uint32_t* end;
};

static_assert(offsetof(VectorEraseOverlay, end) == 0x04);

std::uint32_t* __thiscall Vector_Erase(
    VectorEraseOverlay* self,
    std::uint32_t* first,
    std::uint32_t* last)
{
    int count = static_cast<int>(
        (reinterpret_cast<std::intptr_t>(self->end) - reinterpret_cast<std::intptr_t>(last)) >> 2);

    std::uint32_t* current = first;
    if (0 < count) {
        do {
            *current = *reinterpret_cast<std::uint32_t*>(
                (reinterpret_cast<std::intptr_t>(last) - reinterpret_cast<std::intptr_t>(first)) +
                reinterpret_cast<std::intptr_t>(current));
            current = current + 1;
            count = count + -1;
        } while (count != 0);
    }

    self->end = current;
    return first;
}