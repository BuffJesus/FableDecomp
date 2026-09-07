#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <type_traits>

struct VectorBufferOverlay {
    std::uint8_t* begin;
    std::uint8_t* end;
    std::uint8_t* capacityEnd;
};

static_assert(std::is_standard_layout_v<VectorBufferOverlay>);
static_assert(offsetof(VectorBufferOverlay, begin) == 0x0);
static_assert(offsetof(VectorBufferOverlay, end) == 0x4);
static_assert(offsetof(VectorBufferOverlay, capacityEnd) == 0x8);
static_assert(sizeof(VectorBufferOverlay) == 0xC);

void __thiscall VectorInsert_ExpandWithFill(
    VectorBufferOverlay* self,
    void* insertPos,
    const std::uint8_t* fillValue,
    std::uint32_t /*unused*/,
    std::uint32_t fillCount,
    char skipTailCopy
) {
    const auto addr32 = [](const void* p) -> std::uint32_t {
        return static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(p));
    };

    std::uint32_t oldSize = addr32(self->end) - addr32(self->begin);

    std::uint32_t growth = fillCount;
    if (fillCount <= oldSize) {
        growth = oldSize;
    }

    std::uint32_t newCapacity = growth + oldSize;

    std::uint8_t* newBuffer;
    if (newCapacity == 0) {
        newBuffer = nullptr;
    } else {
        newBuffer = static_cast<std::uint8_t*>(std::malloc(static_cast<std::size_t>(newCapacity)));
    }

    void* oldBegin = self->begin;
    std::uint8_t* writePtr = newBuffer;
    std::uint32_t remainingFill = fillCount;

    if (insertPos != oldBegin) {
        void* moved = std::memmove(
            newBuffer,
            oldBegin,
            static_cast<std::size_t>(addr32(insertPos) - addr32(oldBegin))
        );
        writePtr = static_cast<std::uint8_t*>(moved) + (addr32(insertPos) - addr32(oldBegin));
        remainingFill = fillCount;
    }

    for (; remainingFill != 0; --remainingFill) {
        *writePtr = *fillValue;
        ++writePtr;
    }

    if ((skipTailCopy == '\0') && (self->end != insertPos)) {
        std::uint32_t tailSize = addr32(self->end) - addr32(insertPos);
        void* moved = std::memmove(writePtr, insertPos, static_cast<std::size_t>(tailSize));
        writePtr = static_cast<std::uint8_t*>(moved) + tailSize;
    }

    if (self->begin != nullptr) {
        std::free(self->begin);
    }

    self->begin = newBuffer;
    self->end = writePtr;
    self->capacityEnd = (newCapacity == 0)
        ? nullptr
        : reinterpret_cast<std::uint8_t*>(static_cast<std::uintptr_t>(addr32(newBuffer) + newCapacity));
}