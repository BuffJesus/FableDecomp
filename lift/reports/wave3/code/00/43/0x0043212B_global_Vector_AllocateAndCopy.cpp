#include <cstdint>
#include <cstdlib>

void Vector_AllocateAndCopy(int count, std::uint32_t* begin, std::uint32_t* end)
{
    void* allocation;
    int delta;

    if (count == 0) {
        allocation = nullptr;
    } else {
        allocation = std::malloc(count * 0x0C);
    }

    if (begin != end) {
        delta = static_cast<int>(reinterpret_cast<std::uintptr_t>(allocation)) -
                static_cast<int>(reinterpret_cast<std::uintptr_t>(begin));

        do {
            auto* const dst =
                reinterpret_cast<std::uint32_t*>(delta + static_cast<int>(reinterpret_cast<std::uintptr_t>(begin)));

            if (dst != nullptr) {
                dst[0] = begin[0];
                *reinterpret_cast<std::uint32_t*>(delta + 4 + static_cast<int>(reinterpret_cast<std::uintptr_t>(begin))) = begin[1];
                *reinterpret_cast<std::uint32_t*>(delta + 8 + static_cast<int>(reinterpret_cast<std::uintptr_t>(begin))) = begin[2];
            }

            begin += 3;
        } while (begin != end);
    }
}