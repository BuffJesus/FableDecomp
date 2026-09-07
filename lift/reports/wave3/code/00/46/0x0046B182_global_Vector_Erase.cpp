#include <cstddef>
#include <cstdint>

struct Vector_Erase_Overlay {
    std::byte pad_0[4];
    std::uint32_t* end;
};

static_assert(offsetof(Vector_Erase_Overlay, end) == 0x4);

std::uint32_t* __thiscall Vector_Erase(
    Vector_Erase_Overlay* self,
    std::uint32_t* first,
    std::uint32_t* next)
{
    int count = static_cast<int>(
        (reinterpret_cast<std::uintptr_t>(self->end) -
         reinterpret_cast<std::uintptr_t>(next)) >>
        3);

    std::uint32_t* dst = first;
    if (0 < count) {
        const std::intptr_t delta =
            reinterpret_cast<std::intptr_t>(next) -
            reinterpret_cast<std::intptr_t>(first);

        do {
            *dst = *reinterpret_cast<std::uint32_t*>(
                delta + reinterpret_cast<std::intptr_t>(dst));
            dst[1] = *reinterpret_cast<std::uint32_t*>(
                delta + 4 + reinterpret_cast<std::intptr_t>(dst));
            dst += 2;
            count = count + -1;
        } while (count != 0);
    }

    self->end = dst;
    return first;
}