#include <cstddef>
#include <cstdint>

struct VectorEraseOverlay {
    std::byte pad_0[4];
    std::uint32_t* end;
};
static_assert(offsetof(VectorEraseOverlay, end) == 0x4);

std::uint32_t* __thiscall Vector_Erase(
    VectorEraseOverlay* self,
    std::uint32_t* erase_at,
    std::int32_t next_after_erased)
{
    std::int32_t remaining_count =
        (static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(self->end)) - next_after_erased) / 0xC;

    std::uint32_t* current = erase_at;
    if (0 < remaining_count) {
        do {
            const std::int32_t source_address =
                (next_after_erased - static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(erase_at))) +
                static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(current));
            auto* source = reinterpret_cast<std::uint32_t*>(static_cast<std::uintptr_t>(source_address));

            current[0] = source[0];
            std::uint32_t* third_word = current + 2;
            current[1] = source[1];
            current += 3;
            --remaining_count;
            *third_word = source[2];
        } while (remaining_count != 0);
    }

    self->end = current;
    return erase_at;
}