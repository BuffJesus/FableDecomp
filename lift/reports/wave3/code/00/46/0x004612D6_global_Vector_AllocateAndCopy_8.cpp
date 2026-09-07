#include <cstdint>
#include <cstdlib>

// Corrected from Ghidra's inferred `void`: the assembly preserves the malloc result in EAX
// through function exit, so the binary returns the allocated buffer pointer.
void* __stdcall Vector_AllocateAndCopy_8(
    int count,
    const std::uint32_t* first,
    const std::uint32_t* last)
{
    void* buffer;
    if (count == 0) {
        buffer = nullptr;
    } else {
        buffer = std::malloc(count << 3);
    }

    if (first != last) {
        const auto delta =
            reinterpret_cast<std::intptr_t>(buffer) - reinterpret_cast<std::intptr_t>(first);
        do {
            auto* dst =
                reinterpret_cast<std::uint32_t*>(delta + reinterpret_cast<std::intptr_t>(first));
            if (dst != nullptr) {
                *dst = *first;
                dst[1] = first[1];
            }
            first += 2;
        } while (first != last);
    }

    return buffer;
}