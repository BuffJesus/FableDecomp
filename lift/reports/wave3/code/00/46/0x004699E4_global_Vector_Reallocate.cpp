#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct VectorOverlay {
    std::uint32_t* field_0;
    std::uint32_t* field_4;
    std::uint32_t* field_8;
};

static_assert(offsetof(VectorOverlay, field_0) == 0x0);
static_assert(offsetof(VectorOverlay, field_4) == 0x4);
static_assert(offsetof(VectorOverlay, field_8) == 0x8);
static_assert(sizeof(VectorOverlay) == 0xC);

void __thiscall Vector_Reallocate(
    VectorOverlay* self,
    std::uint32_t* insertPos,
    const std::uint32_t* value,
    std::uint32_t unused,
    std::uint32_t insertCount,
    char skipTailCopy)
{
    (void)unused;

    const std::uint32_t oldCount =
        static_cast<std::uint32_t>(self->field_4 - self->field_0);

    const std::uint32_t growth = (insertCount <= oldCount) ? oldCount : insertCount;
    const std::uint32_t newCapacity = growth + oldCount;

    std::uint32_t* newBuffer;
    if (newCapacity == 0) {
        newBuffer = nullptr;
    } else {
        newBuffer = static_cast<std::uint32_t*>(std::malloc(newCapacity * 4));
    }

    std::uint32_t* write = newBuffer;
    std::uint32_t* read = self->field_0;
    while (read != insertPos) {
        if (write != nullptr) {
            *write = *read;
        }
        ++read;
        ++write;
    }

    if (insertCount == 1) {
        if (write != nullptr) {
            *write = *value;
        }
        ++write;
    } else {
        for (std::uint32_t remaining = insertCount; remaining != 0; --remaining) {
            if (write != nullptr) {
                *write = *value;
            }
            ++write;
        }
    }

    if (skipTailCopy == '\0') {
        std::uint32_t* oldEnd = self->field_4;
        if (insertPos != oldEnd) {
            while (insertPos != oldEnd) {
                if (write != nullptr) {
                    *write = *insertPos;
                }
                ++write;
                ++insertPos;
            }
        }
    }

    if (self->field_0 != nullptr) {
        std::free(self->field_0);
    }

    self->field_0 = newBuffer;
    self->field_4 = write;
    self->field_8 = newBuffer + newCapacity;
}