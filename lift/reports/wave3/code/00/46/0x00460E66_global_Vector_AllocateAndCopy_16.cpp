#include <cstddef>
#include <cstdint>
#include <cstdlib>

std::uint32_t* __stdcall Vector_AllocateAndCopy_16(
    int count,
    const std::uint32_t* first,
    const std::uint32_t* last)
{
    std::uint32_t* result;

    if (count == 0) {
        result = nullptr;
    } else {
        result = static_cast<std::uint32_t*>(std::malloc(count << 4));
    }

    if (first != last) {
        const auto byte_delta =
            reinterpret_cast<std::uintptr_t>(result) - reinterpret_cast<std::uintptr_t>(first);

        do {
            auto* dst = reinterpret_cast<std::uint32_t*>(
                byte_delta + reinterpret_cast<std::uintptr_t>(first));

            if (dst != nullptr) {
                dst[0] = first[0];
                dst[1] = first[1];
                dst[2] = first[2];
                dst[3] = first[3];
            }

            first += 4;
        } while (first != last);
    }

    return result;
}