#include <cstddef>
#include <cstdint>

struct Vector_CopyElements_Overlay {
    std::byte pad_0[0x4];
    std::uint32_t* end;
};

static_assert(offsetof(Vector_CopyElements_Overlay, end) == 0x4);

void __thiscall Vector_CopyElements(
    Vector_CopyElements_Overlay* self,
    std::uint32_t* dest,
    std::int32_t src)
{
    std::int32_t count =
        (static_cast<std::int32_t>(reinterpret_cast<std::intptr_t>(self->end)) - src) >> 3;

    if (0 < count) {
        src = src - static_cast<std::int32_t>(reinterpret_cast<std::intptr_t>(dest));
        do {
            *dest = *reinterpret_cast<std::uint32_t*>(
                src + static_cast<std::int32_t>(reinterpret_cast<std::intptr_t>(dest)));
            dest[1] = *reinterpret_cast<std::uint32_t*>(
                src + 4 + static_cast<std::int32_t>(reinterpret_cast<std::intptr_t>(dest)));
            dest = dest + 2;
            count = count + -1;
        } while (count != 0);
    }

    self->end = dest;
}