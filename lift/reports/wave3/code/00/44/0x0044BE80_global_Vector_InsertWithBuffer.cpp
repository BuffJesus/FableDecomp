#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <type_traits>

struct VectorInsertWithBuffer_Overlay {
    std::uint32_t* begin;
    std::uint32_t* end;
    std::uint32_t* capacityEnd;
};

static_assert(std::is_standard_layout_v<VectorInsertWithBuffer_Overlay>);
static_assert(offsetof(VectorInsertWithBuffer_Overlay, begin) == 0x0);
static_assert(offsetof(VectorInsertWithBuffer_Overlay, end) == 0x4);
static_assert(offsetof(VectorInsertWithBuffer_Overlay, capacityEnd) == 0x8);
static_assert(sizeof(VectorInsertWithBuffer_Overlay) == 0xC);

void __thiscall Vector_InsertWithBuffer(
    VectorInsertWithBuffer_Overlay* self,
    void* insertPos,
    std::uint32_t* value,
    std::uint32_t /*unused*/,
    std::uint32_t insertCount,
    char skipSuffixMove)
{
    std::uint32_t oldCount =
        static_cast<std::uint32_t>((reinterpret_cast<std::uintptr_t>(self->end) -
                                    reinterpret_cast<std::uintptr_t>(self->begin)) >>
                                   2);

    std::uint32_t growthBase = insertCount;
    if (insertCount <= oldCount) {
        growthBase = oldCount;
    }

    std::uint32_t newCapacity = growthBase + oldCount;

    std::uint32_t* newBuffer;
    if (newCapacity == 0) {
        newBuffer = nullptr;
    } else {
        newBuffer = static_cast<std::uint32_t*>(std::malloc(newCapacity * 4));
    }

    void* const oldBegin = self->begin;
    std::uint32_t* writePtr = newBuffer;

    if (insertPos != oldBegin) {
        const int prefixBytes =
            static_cast<int>(reinterpret_cast<std::uintptr_t>(insertPos) -
                             reinterpret_cast<std::uintptr_t>(oldBegin));
        void* moved = std::memmove(newBuffer, oldBegin, static_cast<std::size_t>(prefixBytes));
        writePtr = reinterpret_cast<std::uint32_t*>(
            reinterpret_cast<std::uintptr_t>(moved) + static_cast<std::uintptr_t>(prefixBytes));
    }

    for (std::uint32_t remaining = insertCount; remaining != 0; --remaining) {
        *writePtr = *value;
        ++writePtr;
    }

    if ((skipSuffixMove == '\0') && (self->end != insertPos)) {
        const std::size_t suffixBytes =
            reinterpret_cast<std::uintptr_t>(self->end) -
            reinterpret_cast<std::uintptr_t>(insertPos);
        void* moved = std::memmove(writePtr, insertPos, suffixBytes);
        writePtr = reinterpret_cast<std::uint32_t*>(
            reinterpret_cast<std::uintptr_t>(moved) + suffixBytes);
    }

    if (self->begin != nullptr) {
        std::free(self->begin);
    }

    self->end = writePtr;
    self->begin = newBuffer;
    self->capacityEnd = newBuffer + newCapacity;
}