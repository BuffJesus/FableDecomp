#include <cstddef>
#include <cstdlib>
#include <algorithm>
#include <type_traits>

struct StdVectorAssignElement {
    std::byte raw[0x1C];
};
static_assert(sizeof(StdVectorAssignElement) == 0x1C);

struct StdVectorAssignOverlay {
    StdVectorAssignElement* begin;       // 0x00
    StdVectorAssignElement* end;         // 0x04
    StdVectorAssignElement* capacityEnd; // 0x08
};

static_assert(offsetof(StdVectorAssignOverlay, begin) == 0x00);
static_assert(offsetof(StdVectorAssignOverlay, end) == 0x04);
static_assert(offsetof(StdVectorAssignOverlay, capacityEnd) == 0x08);
static_assert(sizeof(StdVectorAssignOverlay) == 0x0C);

StdVectorAssignOverlay* __thiscall std_vector_assign(
    StdVectorAssignOverlay* self,
    const StdVectorAssignOverlay* other)
{
    if (other != self) {
        StdVectorAssignElement* const srcEnd = other->end;
        StdVectorAssignElement* const srcBegin = other->begin;
        const std::size_t srcCount = static_cast<std::size_t>(srcEnd - srcBegin);

        StdVectorAssignElement* const dstBegin = self->begin;
        const std::size_t dstCapacity =
            static_cast<std::size_t>(self->capacityEnd - dstBegin);

        if (dstCapacity < srcCount) {
            void* newStorage;
            if (srcCount == 0) {
                newStorage = nullptr;
            } else {
                newStorage = std::malloc(srcCount * sizeof(StdVectorAssignElement));
            }

            std::copy(srcBegin, srcEnd,
                      static_cast<StdVectorAssignElement*>(newStorage));

            if (self->begin != nullptr) {
                std::free(self->begin);
            }

            self->capacityEnd =
                static_cast<StdVectorAssignElement*>(newStorage) + srcCount;
            self->end =
                static_cast<StdVectorAssignElement*>(newStorage) + srcCount;
            self->begin =
                static_cast<StdVectorAssignElement*>(newStorage);
            return self;
        }

        const std::size_t dstSize =
            static_cast<std::size_t>(self->end - dstBegin);

        if (srcCount <= dstSize) {
            std::copy(srcBegin, srcEnd, dstBegin);
            self->end = self->begin + srcCount;
            return self;
        }

        std::copy(srcBegin, srcBegin + dstSize, dstBegin);
        std::copy(other->begin + dstSize, other->end, self->end);
        self->end = self->begin + srcCount;
    }

    return self;
}