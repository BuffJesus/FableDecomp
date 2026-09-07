#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct Vector12Storage {
    std::uint32_t* begin;
    std::uint32_t* end;
    std::uint32_t* capacityEnd;
};

static_assert(offsetof(Vector12Storage, begin) == 0x0);
static_assert(offsetof(Vector12Storage, end) == 0x4);
static_assert(offsetof(Vector12Storage, capacityEnd) == 0x8);
static_assert(sizeof(Vector12Storage) == 0xC);

struct Vector12Element {
    std::uint32_t value0;
    std::uint32_t value1;
    std::uint32_t value2;
};

static_assert(sizeof(Vector12Element) == 0xC);

void __thiscall Vector_Reallocate(
    Vector12Storage* self,
    std::uint32_t* insertPos,
    const std::uint32_t* value,
    std::uint32_t /*unused*/,
    std::uint32_t insertCount,
    char skipTailCopy
) {
    std::uint32_t oldCount = static_cast<std::uint32_t>(
        (reinterpret_cast<std::uintptr_t>(self->end) - reinterpret_cast<std::uintptr_t>(self->begin)) / 0xCu);

    std::uint32_t growth = insertCount;
    if (insertCount <= oldCount) {
        growth = oldCount;
    }

    std::uint32_t newCount = growth + oldCount;

    std::uint32_t* newBegin;
    if (newCount == 0) {
        newBegin = nullptr;
    } else {
        newBegin = static_cast<std::uint32_t*>(std::malloc(newCount * 0xCu));
    }

    std::uint32_t newBaseAddr =
        static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(newBegin));
    std::uint32_t writeAddr = newBaseAddr;

    for (std::uint32_t* readPtr = self->begin; readPtr != insertPos; readPtr += 3) {
        if (writeAddr != 0) {
            auto* const writePtr =
                reinterpret_cast<std::uint32_t*>(static_cast<std::uintptr_t>(writeAddr));
            writePtr[0] = readPtr[0];
            writePtr[1] = readPtr[1];
            writePtr[2] = readPtr[2];
        }
        writeAddr += 0xCu;
    }

    std::uint32_t remaining = insertCount;
    if (insertCount == 1) {
        if (writeAddr != 0) {
            auto* const writePtr =
                reinterpret_cast<std::uint32_t*>(static_cast<std::uintptr_t>(writeAddr));
            writePtr[0] = value[0];
            writePtr[1] = value[1];
            writePtr[2] = value[2];
        }
        writeAddr += 0xCu;
    } else {
        for (; remaining != 0; --remaining) {
            if (writeAddr != 0) {
                auto* const writePtr =
                    reinterpret_cast<std::uint32_t*>(static_cast<std::uintptr_t>(writeAddr));
                writePtr[0] = value[0];
                writePtr[1] = value[1];
                writePtr[2] = value[2];
            }
            writeAddr += 0xCu;
        }
    }

    if (skipTailCopy == '\0') {
        std::uint32_t* oldEnd = self->end;
        if (insertPos != oldEnd) {
            const std::int32_t delta = static_cast<std::int32_t>(
                static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(insertPos)) - writeAddr);

            do {
                if (writeAddr != 0) {
                    auto* const writePtr =
                        reinterpret_cast<std::uint32_t*>(static_cast<std::uintptr_t>(writeAddr));
                    auto* const src = reinterpret_cast<const std::uint32_t*>(
                        static_cast<std::uintptr_t>(delta + static_cast<std::int32_t>(writeAddr)));
                    writePtr[0] = src[0];
                    writePtr[1] = src[1];
                    writePtr[2] = src[2];
                }
                writeAddr += 0xCu;
            } while (
                reinterpret_cast<std::uint32_t*>(
                    static_cast<std::uintptr_t>(delta + static_cast<std::int32_t>(writeAddr))) != oldEnd);
        }
    }

    if (self->begin != nullptr) {
        std::free(self->begin);
    }

    self->begin = newBegin;
    self->end = reinterpret_cast<std::uint32_t*>(static_cast<std::uintptr_t>(writeAddr));
    self->capacityEnd = reinterpret_cast<std::uint32_t*>(
        static_cast<std::uintptr_t>(newBaseAddr + newCount * 0xCu));
}