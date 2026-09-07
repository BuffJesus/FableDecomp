#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct Vector20WordGroup {
    std::uint32_t word0;
    std::uint32_t word1;
    std::uint32_t word2;
    std::uint32_t word3;
    std::uint32_t word4;
};

static_assert(sizeof(Vector20WordGroup) == 0x14);

void __cdecl CActiveFile_AssignVector20(
    int count,
    Vector20WordGroup* first,
    Vector20WordGroup* last)
{
    void* allocated;

    if (count == 0) {
        allocated = nullptr;
    } else {
        allocated = std::malloc(count * 0x14);
    }

    if (first != last) {
        const std::ptrdiff_t delta =
            reinterpret_cast<char*>(allocated) - reinterpret_cast<char*>(first);

        do {
            auto* const dst =
                reinterpret_cast<Vector20WordGroup*>(
                    reinterpret_cast<char*>(first) + delta);

            if (dst != nullptr) {
                dst->word0 = first->word0;
                dst->word1 = first->word1;
                dst->word2 = first->word2;
                dst->word3 = first->word3;
                dst->word4 = first->word4;
            }

            ++first;
        } while (first != last);
    }
}