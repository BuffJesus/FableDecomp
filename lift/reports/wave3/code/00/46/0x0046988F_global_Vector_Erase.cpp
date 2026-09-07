#include <cstddef>
#include <cstdint>

struct VectorEraseOverlay {
    std::byte pad_00[0x4];
    std::uint32_t* finish;
};

static_assert(offsetof(VectorEraseOverlay, finish) == 0x4);

std::uint32_t* __thiscall Vector_Erase(void* self_raw, std::uint32_t* erase_begin, std::uint32_t* erase_end)
{
    auto* const self = static_cast<VectorEraseOverlay*>(self_raw);

    int count = static_cast<int>(
        (reinterpret_cast<std::intptr_t>(self->finish) - reinterpret_cast<std::intptr_t>(erase_end)) >> 2);

    std::uint32_t* current = erase_begin;
    if (0 < count) {
        do {
            *current = *reinterpret_cast<std::uint32_t*>(
                (reinterpret_cast<std::intptr_t>(erase_end) - reinterpret_cast<std::intptr_t>(erase_begin)) +
                reinterpret_cast<std::intptr_t>(current));
            ++current;
            count = count + -1;
        } while (count != 0);
    }

    self->finish = current;
    return erase_begin;
}