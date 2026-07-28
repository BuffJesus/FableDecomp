#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct StdVectorStorage
{
    std::uint32_t first; // 0x00
    std::uint32_t last;  // 0x04
    std::uint32_t end;   // 0x08
};

static_assert(offsetof(StdVectorStorage, first) == 0x00);
static_assert(offsetof(StdVectorStorage, last) == 0x04);
static_assert(offsetof(StdVectorStorage, end) == 0x08);

static_assert(sizeof(CWideString) == 4);
static_assert(sizeof(CCharString) == 4);

void __thiscall Std_Vector_Insert_Range_Copies(
    StdVectorStorage* self,
    CWideString* insertPos,
    CWideString* value,
    std::uint32_t /*unused*/,
    std::uint32_t count,
    char copyTailFlag)
{
    const auto to_addr = [](const void* p) -> std::uint32_t
    {
        return static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(p));
    };

    const auto wide_from_addr = [](std::uint32_t addr) -> CWideString*
    {
        return reinterpret_cast<CWideString*>(static_cast<std::uintptr_t>(addr));
    };

    const auto char_from_addr = [](std::uint32_t addr) -> CCharString*
    {
        return reinterpret_cast<CCharString*>(static_cast<std::uintptr_t>(addr));
    };

    std::uint32_t newCount = (self->last - self->first) >> 2;
    const std::uint32_t* selectedCount = &count;
    if (count <= newCount) {
        selectedCount = &newCount;
    }
    newCount = *selectedCount + newCount;

    std::uint32_t newFirst = 0;
    if (newCount != 0) {
        newFirst = to_addr(std::malloc(newCount * 4));
    }

    std::uint32_t cursor = newFirst;

    for (std::uint32_t src = self->first; src != to_addr(insertPos); src += 4) {
        if (cursor != 0) {
            ::new (static_cast<void*>(wide_from_addr(cursor))) CWideString(*wide_from_addr(src));
        }
        cursor += 4;
    }

    if (count == 1) {
        if (cursor != 0) {
            ::new (static_cast<void*>(wide_from_addr(cursor))) CWideString(*value);
        }
        cursor += 4;
    } else {
        for (std::uint32_t remaining = count; remaining != 0; --remaining) {
            if (cursor != 0) {
                ::new (static_cast<void*>(wide_from_addr(cursor))) CWideString(*value);
            }
            cursor += 4;
        }
    }

    if (copyTailFlag == '\0') {
        cursor = to_addr(std::_Uninit_copy(
            insertPos,
            wide_from_addr(self->last),
            wide_from_addr(cursor),
            &copyTailFlag));
    }

    for (std::uint32_t elem = self->first; elem != self->last; elem += 4) {
        char_from_addr(elem)->~CCharString();
    }

    if (self->first != 0) {
        std::free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(self->first)));
    }

    self->last = cursor;
    self->first = newFirst;
    self->end = newFirst + newCount * 4;

    return;
}