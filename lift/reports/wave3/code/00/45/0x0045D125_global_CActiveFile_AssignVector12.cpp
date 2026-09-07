#include <cstdint>
#include <cstdlib>

void __stdcall CActiveFile_AssignVector12(
    int count,
    std::uint32_t* first,
    std::uint32_t* last)
{
    void* buffer;
    int delta;

    if (count == 0) {
        buffer = nullptr;
    } else {
        buffer = std::malloc(count * 0x0C);
    }

    if (first != last) {
        delta =
            static_cast<int>(reinterpret_cast<std::uintptr_t>(buffer)) -
            static_cast<int>(reinterpret_cast<std::uintptr_t>(first));

        do {
            if (reinterpret_cast<std::uint32_t*>(
                    static_cast<std::uintptr_t>(
                        delta + static_cast<int>(reinterpret_cast<std::uintptr_t>(first)))) != nullptr) {
                *reinterpret_cast<std::uint32_t*>(
                    static_cast<std::uintptr_t>(
                        delta + static_cast<int>(reinterpret_cast<std::uintptr_t>(first)))) = first[0];
                *reinterpret_cast<std::uint32_t*>(
                    static_cast<std::uintptr_t>(
                        delta + 4 + static_cast<int>(reinterpret_cast<std::uintptr_t>(first)))) = first[1];
                *reinterpret_cast<std::uint32_t*>(
                    static_cast<std::uintptr_t>(
                        delta + 8 + static_cast<int>(reinterpret_cast<std::uintptr_t>(first)))) = first[2];
            }

            first += 3;
        } while (first != last);
    }
}