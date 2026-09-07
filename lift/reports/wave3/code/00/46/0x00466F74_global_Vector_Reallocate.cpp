#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct VectorElement12 {
    std::uint32_t word0;
    std::uint32_t word1;
    std::uint32_t word2;
};
static_assert(sizeof(VectorElement12) == 0x0C);

struct VectorStorage32 {
    std::uint32_t begin;
    std::uint32_t end;
    std::uint32_t capacityEnd;
};
static_assert(offsetof(VectorStorage32, begin) == 0x00);
static_assert(offsetof(VectorStorage32, end) == 0x04);
static_assert(offsetof(VectorStorage32, capacityEnd) == 0x08);
static_assert(sizeof(VectorStorage32) == 0x0C);

void __thiscall Vector_Reallocate(
    VectorStorage32* self,
    VectorElement12* insertPos,
    const VectorElement12* value,
    std::uint32_t /*unused*/,
    std::uint32_t insertCount,
    char skipTailCopy
) {
    auto* const oldBegin =
        reinterpret_cast<VectorElement12*>(static_cast<std::uintptr_t>(self->begin));
    auto* const oldEnd =
        reinterpret_cast<VectorElement12*>(static_cast<std::uintptr_t>(self->end));

    std::uint32_t oldCount = static_cast<std::uint32_t>(
        (static_cast<std::uintptr_t>(self->end) - static_cast<std::uintptr_t>(self->begin)) / 0x0C);

    std::uint32_t* chosenCount = &insertCount;
    if (insertCount <= oldCount) {
        chosenCount = &oldCount;
    }

    const std::uint32_t newCount = *chosenCount + oldCount;

    VectorElement12* const newBegin =
        (newCount == 0)
            ? nullptr
            : static_cast<VectorElement12*>(std::malloc(static_cast<std::size_t>(newCount) * 0x0C));

    VectorElement12* write = newBegin;

    for (VectorElement12* read = oldBegin; read != insertPos; ++read) {
        if (write != nullptr) {
            write->word0 = read->word0;
            write->word1 = read->word1;
            write->word2 = read->word2;
        }
        ++write;
    }

    std::uint32_t remainingInsertCount = insertCount;
    if (insertCount == 1) {
        if (write != nullptr) {
            write->word0 = value->word0;
            write->word1 = value->word1;
            write->word2 = value->word2;
        }
        ++write;
    } else {
        while (remainingInsertCount != 0) {
            if (write != nullptr) {
                write->word0 = value->word0;
                write->word1 = value->word1;
                write->word2 = value->word2;
            }
            ++write;
            --remainingInsertCount;
        }
    }

    if (skipTailCopy == '\0' && insertPos != oldEnd) {
        const auto delta =
            reinterpret_cast<std::uintptr_t>(insertPos) - reinterpret_cast<std::uintptr_t>(write);

        do {
            if (write != nullptr) {
                auto* const read =
                    reinterpret_cast<VectorElement12*>(reinterpret_cast<std::uintptr_t>(write) + delta);
                write->word0 = read->word0;
                write->word1 = read->word1;
                write->word2 = read->word2;
            }
            ++write;
        } while (
            reinterpret_cast<VectorElement12*>(reinterpret_cast<std::uintptr_t>(write) + delta) != oldEnd);
    }

    if (oldBegin != nullptr) {
        std::free(oldBegin);
    }

    self->begin = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(newBegin));
    self->end = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(write));
    self->capacityEnd = static_cast<std::uint32_t>(
        reinterpret_cast<std::uintptr_t>(newBegin) +
        static_cast<std::uintptr_t>(newCount) * sizeof(VectorElement12));
}