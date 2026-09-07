#include <cstddef>
#include <cstdint>
#include <cstdlib>

void __stdcall CActiveFile_AssignVector24(
    int count,
    std::uint32_t* begin,
    std::uint32_t* end)
{
    void* allocation;

    if (count == 0) {
        allocation = nullptr;
    } else {
        allocation = std::malloc(count * 0x18);
    }

    if (begin != end) {
        const auto delta =
            reinterpret_cast<std::uintptr_t>(allocation) -
            reinterpret_cast<std::uintptr_t>(begin);

        do {
            auto* dest = reinterpret_cast<std::uint32_t*>(
                delta + reinterpret_cast<std::uintptr_t>(begin));

            if (dest != nullptr) {
                auto* src = begin;
                for (int remaining = 6; remaining != 0; --remaining) {
                    *dest = *src;
                    ++src;
                    ++dest;
                }
            }

            begin += 6;
        } while (begin != end);
    }
}