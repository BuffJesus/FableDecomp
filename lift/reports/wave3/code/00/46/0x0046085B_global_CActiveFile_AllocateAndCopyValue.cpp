#include <cstdint>
#include <cstdlib>

void __cdecl CActiveFile_AllocateAndCopyValue(
    int count,
    std::uint32_t* first,
    std::uint32_t* last)
{
    void* allocation;
    std::uint32_t* current;
    int delta;

    if (count == 0) {
        allocation = nullptr;
    } else {
        allocation = std::malloc(count << 2);
    }

    if (first != last) {
        delta = static_cast<int>(reinterpret_cast<std::uintptr_t>(allocation)) -
                static_cast<int>(reinterpret_cast<std::uintptr_t>(first));

        do {
            current = reinterpret_cast<std::uint32_t*>(
                delta + static_cast<int>(reinterpret_cast<std::uintptr_t>(first)));

            if (current != nullptr) {
                *current = *first;
            }

            ++first;
        } while (first != last);
    }
}