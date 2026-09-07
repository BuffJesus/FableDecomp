#include <cstdint>
#include <cstdlib>

std::uint32_t* __stdcall Vector_AllocateAndCopy_8(
    int count,
    std::uint32_t* first,
    std::uint32_t* last)
{
    std::uint32_t* allocated;
    if (count == 0) {
        allocated = nullptr;
    } else {
        allocated = static_cast<std::uint32_t*>(std::malloc(static_cast<std::size_t>(count) << 3));
    }

    if (first != last) {
        const std::intptr_t delta =
            reinterpret_cast<std::intptr_t>(allocated) -
            reinterpret_cast<std::intptr_t>(first);

        do {
            auto* dst = reinterpret_cast<std::uint32_t*>(
                delta + reinterpret_cast<std::intptr_t>(first));

            if (dst != nullptr) {
                dst[0] = first[0];
                dst[1] = first[1];
            }

            first += 2;
        } while (first != last);
    }

    return allocated;
}