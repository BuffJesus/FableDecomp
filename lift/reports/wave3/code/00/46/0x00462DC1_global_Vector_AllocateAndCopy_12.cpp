#include <cstdint>
#include <cstdlib>

void* __stdcall Vector_AllocateAndCopy_12(int count, const std::uint32_t* srcBegin, const std::uint32_t* srcEnd)
{
    void* allocated;

    if (count == 0) {
        allocated = nullptr;
    } else {
        allocated = std::malloc(count * 0x0C);
    }

    if (srcBegin != srcEnd) {
        const std::intptr_t delta =
            reinterpret_cast<std::intptr_t>(allocated) - reinterpret_cast<std::intptr_t>(srcBegin);

        do {
            auto* dst = reinterpret_cast<std::uint32_t*>(delta + reinterpret_cast<std::intptr_t>(srcBegin));
            if (dst != nullptr) {
                dst[0] = srcBegin[0];
                dst[1] = srcBegin[1];
                dst[2] = srcBegin[2];
            }
            srcBegin += 3;
        } while (srcBegin != srcEnd);
    }

    return allocated;
}