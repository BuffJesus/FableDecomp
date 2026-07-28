#include <cstddef>
#include <cstdint>

struct VectorDestructStringRangeElement
{
    CCharString value0; // 0x00
    CCharString value4; // 0x04
    CCharString value8; // 0x08
    CCharString valueC; // 0x0C
    std::byte _pad10[0x1C - 0x10];
};

static_assert(offsetof(VectorDestructStringRangeElement, value0) == 0x00);
static_assert(offsetof(VectorDestructStringRangeElement, value4) == 0x04);
static_assert(offsetof(VectorDestructStringRangeElement, value8) == 0x08);
static_assert(offsetof(VectorDestructStringRangeElement, valueC) == 0x0C);
static_assert(sizeof(VectorDestructStringRangeElement) == 0x1C);

struct VectorDestructStringRangePageCursor
{
    std::byte _pad0[0x4];
    VectorDestructStringRangeElement* nextPage; // 0x04
};

static_assert(offsetof(VectorDestructStringRangePageCursor, nextPage) == 0x04);

void __stdcall Vector_DestructStringRange(
    VectorDestructStringRangeElement* current,
    std::uint32_t /*unused*/,
    VectorDestructStringRangeElement* pageEnd,
    VectorDestructStringRangePageCursor* pageCursor,
    VectorDestructStringRangeElement* last)
{
    while (current != last)
    {
        current->valueC.~CCharString();
        current->value8.~CCharString();
        current->value4.~CCharString();
        current->value0.~CCharString();

        current = current + 1;
        if (current == pageEnd)
        {
            current = pageCursor->nextPage;
            pageCursor = reinterpret_cast<VectorDestructStringRangePageCursor*>(
                reinterpret_cast<std::byte*>(pageCursor) + 0x4);
            pageEnd = reinterpret_cast<VectorDestructStringRangeElement*>(
                reinterpret_cast<std::byte*>(current) + 0x70);
        }
    }
}