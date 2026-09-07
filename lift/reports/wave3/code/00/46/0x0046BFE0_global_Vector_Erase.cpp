#include <cstddef>
#include <cstdint>

struct VectorOverlay
{
    std::byte* data;
    std::byte* end;
};

static_assert(offsetof(VectorOverlay, end) == 0x4);

std::uint32_t* __thiscall Vector_Erase(VectorOverlay* self, std::uint32_t* first, std::uint32_t* last)
{
    int remaining = static_cast<int>((reinterpret_cast<std::uintptr_t>(self->end) -
                                      reinterpret_cast<std::uintptr_t>(last)) / 0x54);
    std::uint32_t* currentEnd = first;

    if (0 < remaining)
    {
        std::uint32_t* dst = first;
        do
        {
            currentEnd = dst + 0x15;
            remaining = remaining + -1;

            std::uint32_t* src = reinterpret_cast<std::uint32_t*>(
                (reinterpret_cast<std::uintptr_t>(last) - reinterpret_cast<std::uintptr_t>(first)) +
                reinterpret_cast<std::uintptr_t>(dst));

            for (int count = 0x15; count != 0; count = count + -1)
            {
                *dst = *src;
                src = src + 1;
                dst = dst + 1;
            }

            dst = currentEnd;
        } while (remaining != 0);
    }

    self->end = reinterpret_cast<std::byte*>(currentEnd);
    return first;
}