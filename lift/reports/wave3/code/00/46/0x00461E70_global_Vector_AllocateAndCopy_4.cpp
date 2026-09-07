#include <cstdint>
#include <cstdlib>

void __stdcall Vector_AllocateAndCopy_4(
    int count,
    std::uint32_t* first,
    std::uint32_t* last)
{
    void* allocated;

    if (count == 0) {
        allocated = nullptr;
    } else {
        allocated = std::malloc(count << 2);
    }

    if (first != last) {
        const std::int32_t delta =
            static_cast<std::int32_t>(
                static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(allocated))) -
            static_cast<std::int32_t>(
                static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(first)));

        do {
            auto* const target = reinterpret_cast<std::uint32_t*>(
                static_cast<std::uintptr_t>(
                    static_cast<std::uint32_t>(
                        delta +
                        static_cast<std::int32_t>(
                            static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(first))))));

            if (target != nullptr) {
                *target = *first;
            }

            ++first;
        } while (first != last);
    }
}