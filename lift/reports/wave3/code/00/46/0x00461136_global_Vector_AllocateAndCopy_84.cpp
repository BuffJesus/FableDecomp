#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct Element84
{
    std::uint32_t dwords[0x15];
};

static_assert(sizeof(Element84) == 0x54);

Element84* __stdcall Vector_AllocateAndCopy_84(
    int count,
    const Element84* first,
    const Element84* last)
{
    Element84* allocated;

    if (count == 0) {
        allocated = nullptr;
    } else {
        allocated = static_cast<Element84*>(std::malloc(count * 0x54));
    }

    if (first != last) {
        const auto delta =
            reinterpret_cast<std::uintptr_t>(allocated) -
            reinterpret_cast<std::uintptr_t>(first);

        do {
            auto* destination = reinterpret_cast<Element84*>(
                delta + reinterpret_cast<std::uintptr_t>(first));

            if (destination != nullptr) {
                const std::uint32_t* sourceWord = first->dwords;
                std::uint32_t* destinationWord = destination->dwords;

                for (int remaining = 0x15; remaining != 0; --remaining) {
                    *destinationWord = *sourceWord;
                    ++sourceWord;
                    ++destinationWord;
                }
            }

            ++first;
        } while (first != last);
    }

    return allocated;
}