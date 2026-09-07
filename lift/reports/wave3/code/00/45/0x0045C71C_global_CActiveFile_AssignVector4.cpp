#include <cstdint>
#include <cstdlib>

void __cdecl CActiveFile_AssignVector4(
    int count,
    std::uint32_t* begin,
    std::uint32_t* end)
{
    void* allocated;
    std::uint32_t* current;
    std::intptr_t delta;

    if (count == 0) {
        allocated = nullptr;
    } else {
        allocated = std::malloc(static_cast<std::size_t>(count) << 2);
    }

    if (begin != end) {
        delta = reinterpret_cast<std::intptr_t>(allocated) -
                reinterpret_cast<std::intptr_t>(begin);

        do {
            current = reinterpret_cast<std::uint32_t*>(
                delta + reinterpret_cast<std::intptr_t>(begin));

            if (current != nullptr) {
                *current = *begin;
            }

            ++begin;
        } while (begin != end);
    }
}