#include <cstdint>
#include <cstdlib>

std::uint32_t* __cdecl Vector_AllocateAndCopy_12(
    int count,
    const std::uint32_t* begin,
    const std::uint32_t* end)
{
    std::uint32_t* result;

    if (count == 0) {
        result = nullptr;
    } else {
        result = static_cast<std::uint32_t*>(std::malloc(count * 0xCu));
    }

    if (begin != end) {
        const auto delta =
            reinterpret_cast<std::uintptr_t>(result) - reinterpret_cast<std::uintptr_t>(begin);

        do {
            auto* const dst = reinterpret_cast<std::uint32_t*>(
                reinterpret_cast<std::uintptr_t>(begin) + delta);

            if (dst != nullptr) {
                dst[0] = begin[0];
                dst[1] = begin[1];
                dst[2] = begin[2];
            }

            begin += 3;
        } while (begin != end);
    }

    return result;
}