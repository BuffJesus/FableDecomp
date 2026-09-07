#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct RawVector12 {
    std::uint32_t* begin;        // 0x00
    std::uint32_t* end;          // 0x04
    std::uint32_t* capacityEnd;  // 0x08
};

static_assert(offsetof(RawVector12, begin) == 0x00);
static_assert(offsetof(RawVector12, end) == 0x04);
static_assert(offsetof(RawVector12, capacityEnd) == 0x08);
static_assert(sizeof(RawVector12) == 0x0C);

static inline std::uint32_t RawPtr32(const void* ptr) {
    return static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(ptr));
}

static inline std::uint32_t* Ptr32(std::uint32_t raw) {
    return reinterpret_cast<std::uint32_t*>(static_cast<std::uintptr_t>(raw));
}

void __thiscall Vector_Reallocate(
    RawVector12* self,
    std::uint32_t* insertPos,
    const std::uint32_t* value,
    std::uint32_t /*unused*/,
    std::uint32_t count,
    char preserveGapOnly)
{
    const std::uint32_t beginRaw = RawPtr32(self->begin);
    const std::uint32_t endRaw = RawPtr32(self->end);
    const std::uint32_t oldSpanRaw = endRaw - beginRaw;
    const std::int32_t oldCountSigned = static_cast<std::int32_t>(oldSpanRaw) / 0x0C;
    const std::uint32_t oldCountRaw = static_cast<std::uint32_t>(oldCountSigned);

    const std::uint32_t growthBase = (count <= oldCountRaw) ? oldCountRaw : count;
    const std::uint32_t newCountRaw = growthBase + oldCountRaw;

    std::uint32_t* newBuffer;
    if (newCountRaw == 0) {
        newBuffer = nullptr;
    } else {
        newBuffer = static_cast<std::uint32_t*>(std::malloc(static_cast<std::uint32_t>(newCountRaw * 0x0Cu)));
    }

    const std::uint32_t newBufferRaw = RawPtr32(newBuffer);
    std::uint32_t dstRaw = newBufferRaw;

    for (const std::uint32_t* src = self->begin; src != insertPos; src += 3) {
        if (dstRaw != 0) {
            std::uint32_t* const dst = Ptr32(dstRaw);
            dst[0] = src[0];
            dst[1] = src[1];
            dst[2] = src[2];
        }
        dstRaw += 0x0C;
    }

    std::uint32_t remaining = count;
    if (count == 1) {
        if (dstRaw != 0) {
            std::uint32_t* const dst = Ptr32(dstRaw);
            dst[0] = value[0];
            dst[1] = value[1];
            dst[2] = value[2];
        }
        dstRaw += 0x0C;
    } else {
        for (; remaining != 0; --remaining) {
            if (dstRaw != 0) {
                std::uint32_t* const dst = Ptr32(dstRaw);
                dst[0] = value[0];
                dst[1] = value[1];
                dst[2] = value[2];
            }
            dstRaw += 0x0C;
        }
    }

    remaining = count;
    if (preserveGapOnly == '\0') {
        const std::uint32_t oldEndRaw = RawPtr32(self->end);
        if (RawPtr32(insertPos) != oldEndRaw) {
            const std::uint32_t deltaRaw = RawPtr32(insertPos) - dstRaw;
            do {
                if (dstRaw != 0) {
                    const std::uint32_t* const src = Ptr32(dstRaw + deltaRaw);
                    std::uint32_t* const dst = Ptr32(dstRaw);
                    dst[0] = src[0];
                    dst[1] = src[1];
                    dst[2] = src[2];
                }
                dstRaw += 0x0C;
            } while (dstRaw + deltaRaw != oldEndRaw);
        }
    }

    if (self->begin != nullptr) {
        std::free(self->begin);
    }

    self->begin = newBuffer;
    self->end = Ptr32(dstRaw);
    self->capacityEnd = Ptr32(newBufferRaw + newCountRaw * 0x0C);
}