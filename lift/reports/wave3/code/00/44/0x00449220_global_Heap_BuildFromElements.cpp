#include <cstddef>
#include <cstdint>

struct HeapElementOverlay
{
    std::uint32_t value0;   // +0x00
    std::uint32_t value4;   // +0x04
    int* refCount;          // +0x08
};

static_assert(offsetof(HeapElementOverlay, value0) == 0x00);
static_assert(offsetof(HeapElementOverlay, value4) == 0x04);
static_assert(offsetof(HeapElementOverlay, refCount) == 0x08);
static_assert(sizeof(HeapElementOverlay) == 0x0C);

extern void Vector_InsertWithMove(
    std::uint32_t value0,
    std::uint32_t value4,
    int* refCount,
    std::uint32_t heapContext);

void __fastcall Heap_BuildFromElements(
    HeapElementOverlay* first,
    HeapElementOverlay* last,
    std::uint32_t heapContext)
{
    if (first != last)
    {
        for (HeapElementOverlay* it = first + 1; it != last; ++it)
        {
            const std::uint32_t value0 = it->value0;
            const std::uint32_t value4 = it->value4;
            int* const refCount = it->refCount;

            if (refCount != nullptr)
            {
                *refCount = *refCount + 1;
            }

            Vector_InsertWithMove(value0, value4, refCount, heapContext);
        }
    }
}