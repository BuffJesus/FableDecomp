#include <cstdint>
#include <cstdlib>

void* __stdcall Vector_AllocateAndCopy_8(
    int count,
    const std::uint32_t* first,
    const std::uint32_t* last)
{
    void* result;
    if (count == 0) {
        result = nullptr;
    } else {
        result = std::malloc(count << 3);
    }

    if (first != last) {
        const std::int32_t delta =
            static_cast<std::int32_t>(reinterpret_cast<std::intptr_t>(result)) -
            static_cast<std::int32_t>(reinterpret_cast<std::intptr_t>(first));

        do {
            auto* const dst = reinterpret_cast<std::uint32_t*>(
                delta + static_cast<std::int32_t>(reinterpret_cast<std::intptr_t>(first)));
            if (dst != nullptr) {
                *dst = *first;
                *reinterpret_cast<std::uint32_t*>(
                    delta + 4 + static_cast<std::int32_t>(reinterpret_cast<std::intptr_t>(first))) =
                    first[1];
            }
            first += 2;
        } while (first != last);
    }

    return result;
}