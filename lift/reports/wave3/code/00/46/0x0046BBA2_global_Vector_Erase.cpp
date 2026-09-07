#include <cstddef>
#include <cstdint>

struct VectorEraseOverlay {
    std::uint8_t pad_0x00[0x04];
    std::uint32_t* end;
};

static_assert(offsetof(VectorEraseOverlay, end) == 0x04);

std::uint32_t* __thiscall Vector_Erase(VectorEraseOverlay* self, std::uint32_t* first, std::intptr_t erase_end_raw)
{
    int count = static_cast<int>(
        (reinterpret_cast<std::intptr_t>(self->end) - erase_end_raw) / 0xC
    );

    std::uint32_t* dst = first;
    if (0 < count) {
        do {
            std::uint32_t* src = reinterpret_cast<std::uint32_t*>(
                (erase_end_raw - reinterpret_cast<std::intptr_t>(first)) +
                reinterpret_cast<std::intptr_t>(dst)
            );

            *dst = *src;
            std::uint32_t* dst_third = dst + 2;
            dst[1] = src[1];
            dst = dst + 3;
            count = count + -1;
            *dst_third = src[2];
        } while (count != 0);
    }

    self->end = dst;
    return first;
}