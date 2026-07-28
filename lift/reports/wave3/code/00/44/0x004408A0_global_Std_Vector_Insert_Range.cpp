#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <type_traits>

struct StdVectorIntOverlay {
    std::uint32_t* first;
    std::uint32_t* last;
    std::uint32_t* end;
};

static_assert(offsetof(StdVectorIntOverlay, first) == 0x0);
static_assert(offsetof(StdVectorIntOverlay, last)  == 0x4);
static_assert(offsetof(StdVectorIntOverlay, end)   == 0x8);
static_assert(sizeof(StdVectorIntOverlay) == 0xC);

void __thiscall Std_Vector_Insert_Range(
    StdVectorIntOverlay* self,
    void* insertPos,
    const std::uint32_t* valueSrc,
    std::uint32_t /*unusedParam4*/,
    std::uint32_t count,
    char skipTailCopy)
{
    std::uint32_t currentSize = static_cast<std::uint32_t>(
        (reinterpret_cast<std::uintptr_t>(self->last) - reinterpret_cast<std::uintptr_t>(self->first)) >> 2);

    std::uint32_t growth = count;
    if (count <= currentSize) {
        growth = currentSize;
    }

    std::uint32_t newCount = growth + currentSize;

    std::uint32_t* newBuffer;
    if (newCount == 0) {
        newBuffer = nullptr;
    } else {
        newBuffer = static_cast<std::uint32_t*>(std::malloc(newCount * 4));
    }

    void* oldBegin = self->first;
    std::uint32_t* dst = newBuffer;

    if (insertPos != oldBegin) {
        void* moved = std::memmove(
            newBuffer,
            oldBegin,
            static_cast<std::size_t>(reinterpret_cast<std::uintptr_t>(insertPos) - reinterpret_cast<std::uintptr_t>(oldBegin)));
        dst = reinterpret_cast<std::uint32_t*>(
            reinterpret_cast<std::uintptr_t>(moved) +
            (reinterpret_cast<std::uintptr_t>(insertPos) - reinterpret_cast<std::uintptr_t>(oldBegin)));
    }

    for (std::uint32_t remaining = count; remaining != 0; --remaining) {
        *dst = *valueSrc;
        ++dst;
    }

    if ((skipTailCopy == '\0') && (self->last != insertPos)) {
        const std::size_t tailSize = static_cast<std::size_t>(
            reinterpret_cast<std::uintptr_t>(self->last) - reinterpret_cast<std::uintptr_t>(insertPos));
        void* moved = std::memmove(dst, insertPos, tailSize);
        dst = reinterpret_cast<std::uint32_t*>(
            reinterpret_cast<std::uintptr_t>(moved) + tailSize);
    }

    if (self->first != nullptr) {
        std::free(self->first);
    }

    self->last = dst;
    self->first = newBuffer;
    self->end = newBuffer + newCount;
}