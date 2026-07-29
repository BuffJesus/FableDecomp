#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct VectorReallocateDoubleBuffer {
    std::int32_t begin;
    std::int32_t end;
    std::int32_t capacityEnd;
};

static_assert(offsetof(VectorReallocateDoubleBuffer, begin) == 0x0);
static_assert(offsetof(VectorReallocateDoubleBuffer, end) == 0x4);
static_assert(offsetof(VectorReallocateDoubleBuffer, capacityEnd) == 0x8);

void __thiscall VectorReallocateDouble(
    VectorReallocateDoubleBuffer* self,
    std::uint32_t* insertPos,
    const std::uint32_t* value,
    std::uint32_t /*unused*/,
    unsigned int insertCount,
    char skipTailCopy)
{
    unsigned int requestedCount = insertCount;
    const std::int32_t currentCountSigned = (self->end - self->begin) >> 3;
    unsigned int currentCount = static_cast<unsigned int>(currentCountSigned);

    const unsigned int* growthBase = &insertCount;
    if (insertCount <= currentCount) {
        growthBase = &currentCount;
    }

    insertCount = *growthBase + currentCount;

    std::uint32_t* newBegin;
    if (insertCount == 0) {
        newBegin = nullptr;
    } else {
        newBegin = static_cast<std::uint32_t*>(std::malloc(insertCount * 8));
    }

    std::int32_t writeAddr = static_cast<std::int32_t>(
        reinterpret_cast<std::uintptr_t>(newBegin));
    std::int32_t readAddr = self->begin;
    const std::int32_t insertAddr = static_cast<std::int32_t>(
        reinterpret_cast<std::uintptr_t>(insertPos));

    for (; readAddr != insertAddr; readAddr += 8) {
        if (writeAddr != 0) {
            auto* const write = reinterpret_cast<std::uint32_t*>(static_cast<std::uintptr_t>(writeAddr));
            const auto* const read = reinterpret_cast<const std::uint32_t*>(static_cast<std::uintptr_t>(readAddr));
            write[0] = read[0];
            write[1] = read[1];
        }
        writeAddr += 8;
    }

    if (requestedCount == 1) {
        if (writeAddr != 0) {
            auto* const write = reinterpret_cast<std::uint32_t*>(static_cast<std::uintptr_t>(writeAddr));
            write[0] = value[0];
            write[1] = value[1];
        }
        writeAddr += 8;
    } else {
        for (; requestedCount != 0; --requestedCount) {
            if (writeAddr != 0) {
                auto* const write = reinterpret_cast<std::uint32_t*>(static_cast<std::uintptr_t>(writeAddr));
                write[0] = value[0];
                write[1] = value[1];
            }
            writeAddr += 8;
        }
    }

    if ((skipTailCopy == '\0') && (insertAddr != self->end)) {
        const std::int32_t delta = insertAddr - writeAddr;
        do {
            if (writeAddr != 0) {
                auto* const write = reinterpret_cast<std::uint32_t*>(static_cast<std::uintptr_t>(writeAddr));
                write[0] = *reinterpret_cast<const std::uint32_t*>(
                    static_cast<std::uintptr_t>(delta + writeAddr));
                write[1] = *reinterpret_cast<const std::uint32_t*>(
                    static_cast<std::uintptr_t>(delta + 4 + writeAddr));
            }
            writeAddr += 8;
        } while (delta + writeAddr != self->end);
    }

    if (self->begin != 0) {
        std::free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(self->begin)));
    }

    self->end = writeAddr;
    self->begin = static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(newBegin));
    self->capacityEnd = self->begin + static_cast<std::int32_t>(insertCount * 8);
}