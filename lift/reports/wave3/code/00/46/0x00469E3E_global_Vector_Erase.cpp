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
    std::uint32_t* erase_from)
{
    std::int32_t remaining =
        (static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(self->end)) -
         static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(erase_from))) >> 3;

    std::uint32_t* current = erase_at;

    if (0 < remaining) {
        const std::int32_t copy_offset =
            static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(erase_from)) -
            static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(erase_at));

        do {
            *current = *reinterpret_cast<std::uint32_t*>(
                static_cast<std::uint32_t>(
                    copy_offset +
                    static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(current))));
            current[1] = *reinterpret_cast<std::uint32_t*>(
                static_cast<std::uint32_t>(
                    copy_offset + 4 +
                    static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(current))));
            current += 2;
            remaining = remaining + -1;
        } while (remaining != 0);
    }

    self->end = current;
    return erase_at;
}