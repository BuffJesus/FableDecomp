#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct CMemoryAllocatorVariableSize;

struct CMemoryAllocatorVariableSize_Overlay
{
    std::uint32_t* begin;    // 0x00
    std::uint32_t* current;  // 0x04
    std::uint32_t* end;      // 0x08
    std::byte pad0C[0x04];
    std::uint32_t areaValue; // 0x10
};

static_assert(offsetof(CMemoryAllocatorVariableSize_Overlay, begin) == 0x00);
static_assert(offsetof(CMemoryAllocatorVariableSize_Overlay, current) == 0x04);
static_assert(offsetof(CMemoryAllocatorVariableSize_Overlay, end) == 0x08);
static_assert(offsetof(CMemoryAllocatorVariableSize_Overlay, areaValue) == 0x10);

struct CMemoryAllocatorVariableSize
{
    CMemoryAllocatorVariableSize* GetNoAllocatedAreas();
};

void __thiscall CMemoryAllocatorVariableSize_InitFromRange(
    CMemoryAllocatorVariableSize* self,
    CMemoryAllocatorVariableSize* first,
    CMemoryAllocatorVariableSize* last)
{
    int count = 0;
    CMemoryAllocatorVariableSize* iter = first;
    CMemoryAllocatorVariableSize* walk = self;

    if (first != last)
    {
        do
        {
            CMemoryAllocatorVariableSize* const previous = iter;
            iter = walk->GetNoAllocatedAreas();
            walk = previous;
            ++count;
        } while (iter != last);
    }

    std::uint32_t* storage = nullptr;
    if (count != 0)
    {
        storage = static_cast<std::uint32_t*>(std::malloc(count * 4));
    }

    auto* const selfFields = reinterpret_cast<CMemoryAllocatorVariableSize_Overlay*>(self);
    selfFields->end = storage + count;
    selfFields->begin = storage;

    while (first != last)
    {
        if (storage != nullptr)
        {
            const auto* const firstFields =
                reinterpret_cast<const CMemoryAllocatorVariableSize_Overlay*>(first);
            *storage = firstFields->areaValue;
        }

        first = first->GetNoAllocatedAreas();
        ++storage;
    }

    selfFields->current = storage;
}