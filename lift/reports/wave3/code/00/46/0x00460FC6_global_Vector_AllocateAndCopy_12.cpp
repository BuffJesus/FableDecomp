#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct Vector12Overlay {
    std::uint32_t word0;
    std::uint32_t word1;
    std::uint32_t word2;
};

static_assert(sizeof(Vector12Overlay) == 0xC);

void* __stdcall Vector_AllocateAndCopy_12(
    int count,
    const Vector12Overlay* first,
    const Vector12Overlay* last)
{
    void* allocation;

    if (count == 0) {
        allocation = nullptr;
    } else {
        allocation = std::malloc(count * 0xC);
    }

    if (first != last) {
        const auto delta =
            reinterpret_cast<std::uintptr_t>(allocation) -
            reinterpret_cast<std::uintptr_t>(first);

        do {
            auto* const destination = reinterpret_cast<Vector12Overlay*>(
                delta + reinterpret_cast<std::uintptr_t>(first));

            if (destination != nullptr) {
                destination->word0 = first->word0;
                destination->word1 = first->word1;
                destination->word2 = first->word2;
            }

            ++first;
        } while (first != last);
    }

    return allocation;
}