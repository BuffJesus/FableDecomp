#include <cstddef>
#include <cstdint>

struct VectorEraseOverlay {
    std::byte pad_0[4];
    std::uint32_t* end; // +0x04
};

static_assert(offsetof(VectorEraseOverlay, end) == 0x04);

std::uint32_t* __thiscall Vector_Erase(VectorEraseOverlay* self, std::uint32_t* first, int last)
{
    int count = (static_cast<int>(reinterpret_cast<std::uintptr_t>(self->end)) - last) / 0x14;
    std::uint32_t* newEnd = first;

    if (0 < count) {
        std::uint32_t* dst = first;
        do {
            newEnd = dst + 5;
            count = count + -1;

            std::uint32_t* src = reinterpret_cast<std::uint32_t*>(
                (last - static_cast<int>(reinterpret_cast<std::uintptr_t>(first))) +
                static_cast<int>(reinterpret_cast<std::uintptr_t>(dst))
            );

            for (int i = 5; i != 0; i = i + -1) {
                *dst = *src;
                src = src + 1;
                dst = dst + 1;
            }

            dst = newEnd;
        } while (count != 0);
    }

    self->end = newEnd;
    return first;
}