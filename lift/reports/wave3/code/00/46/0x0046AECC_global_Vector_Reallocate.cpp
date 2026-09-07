#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct VectorReallocateEntry20 {
    std::uint32_t words[5];
};
static_assert(sizeof(VectorReallocateEntry20) == 0x14);

struct VectorReallocateOverlay {
    VectorReallocateEntry20* begin;    // 0x00
    VectorReallocateEntry20* end;      // 0x04
    VectorReallocateEntry20* capacity; // 0x08
};
static_assert(offsetof(VectorReallocateOverlay, begin) == 0x00);
static_assert(offsetof(VectorReallocateOverlay, end) == 0x04);
static_assert(offsetof(VectorReallocateOverlay, capacity) == 0x08);
static_assert(sizeof(VectorReallocateOverlay) == 0x0C);

void __thiscall Vector_Reallocate(
    VectorReallocateOverlay* self,
    VectorReallocateEntry20* split,
    std::uint32_t* source,
    std::uint32_t /*unused_param_4*/,
    std::uint32_t insertCount,
    std::uint32_t preserveTailFlag)
{
    const std::uint32_t oldCount =
        static_cast<std::uint32_t>(
            (reinterpret_cast<std::uintptr_t>(self->end) - reinterpret_cast<std::uintptr_t>(self->begin)) / 0x14);

    const std::uint32_t growth = (insertCount <= oldCount) ? oldCount : insertCount;
    const std::uint32_t newCapacityCount = growth + oldCount;

    auto* const newBuffer = (newCapacityCount == 0)
        ? nullptr
        : static_cast<VectorReallocateEntry20*>(std::malloc(newCapacityCount * 0x14));

    auto* write = newBuffer;

    for (auto* read = self->begin; read != split; ++read) {
        if (write != nullptr) {
            for (int i = 0; i != 5; ++i) {
                write->words[i] = read->words[i];
            }
        }
        ++write;
    }

    if (insertCount == 1) {
        if (write != nullptr) {
            for (int i = 0; i != 5; ++i) {
                reinterpret_cast<std::uint32_t*>(write)[i] = *source;
                ++source;
            }
        }
        ++write;
    } else {
        for (std::uint32_t remaining = insertCount; remaining != 0; --remaining) {
            if (write != nullptr) {
                for (int i = 0; i != 5; ++i) {
                    reinterpret_cast<std::uint32_t*>(write)[i] = source[i];
                }
            }
            ++write;
        }
    }

    if ((static_cast<std::uint8_t>(preserveTailFlag) == 0) && (split != self->end)) {
        const auto byteDelta =
            reinterpret_cast<std::intptr_t>(split) - reinterpret_cast<std::intptr_t>(write);
        VectorReallocateEntry20* const oldEnd = self->end;

        do {
            if (write != nullptr) {
                auto* const read =
                    reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::intptr_t>(write) + byteDelta);
                for (int i = 0; i != 5; ++i) {
                    reinterpret_cast<std::uint32_t*>(write)[i] = read[i];
                }
            }
            ++write;
        } while (reinterpret_cast<VectorReallocateEntry20*>(
                     reinterpret_cast<std::intptr_t>(write) + byteDelta) != oldEnd);
    }

    if (self->begin != nullptr) {
        std::free(self->begin);
    }

    self->begin = newBuffer;
    self->end = write;
    self->capacity = newBuffer + newCapacityCount;
}