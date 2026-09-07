#include <cstdint>
#include <cstdlib>

std::uint32_t* __stdcall Vector_AllocateAndCopy_4(
    int count,
    const std::uint32_t* first,
    const std::uint32_t* last)
{
    std::uint32_t* result;

    if (count == 0) {
        result = nullptr;
    } else {
        result = static_cast<std::uint32_t*>(std::malloc(count << 2));
    }

    if (first != last) {
        const auto delta =
            reinterpret_cast<std::uintptr_t>(result) -
            reinterpret_cast<std::uintptr_t>(first);

        do {
            auto* const dst = reinterpret_cast<std::uint32_t*>(
                delta + reinterpret_cast<std::uintptr_t>(first));

            if (dst != nullptr) {
                *dst = *first;
            }

            ++first;
        } while (first != last);
    }

    return result;
}