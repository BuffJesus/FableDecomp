#include <cstddef>
#include <cstdint>

struct VectorEraseOverlay {
    std::uint32_t* begin;
    std::uint32_t* end;
};

static_assert(offsetof(VectorEraseOverlay, end) == 0x4);

std::uint32_t* __thiscall Vector_Erase(
    VectorEraseOverlay* self,
    std::uint32_t* erase_at,
    std::intptr_t tail_begin)
{
    int remaining = (static_cast<int>(reinterpret_cast<std::intptr_t>(self->end)) -
                     static_cast<int>(tail_begin)) / 0xC;

    std::uint32_t* dst = erase_at;
    if (0 < remaining) {
        do {
            std::uint32_t* src =
                reinterpret_cast<std::uint32_t*>((tail_begin - reinterpret_cast<std::intptr_t>(erase_at)) +
                                                 reinterpret_cast<std::intptr_t>(dst));

            *dst = *src;
            std::uint32_t* dst_word2 = dst + 2;
            dst[1] = src[1];
            dst = dst + 3;
            remaining = remaining + -1;
            *dst_word2 = src[2];
        } while (remaining != 0);
    }

    self->end = dst;
    return erase_at;
}