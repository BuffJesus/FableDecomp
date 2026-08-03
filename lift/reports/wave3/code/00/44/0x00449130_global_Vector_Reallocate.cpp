#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct CActiveEntityScriptBase;

struct RefCountedActiveEntityScriptBaseOverlay {
    std::int32_t refCount;
};
static_assert(offsetof(RefCountedActiveEntityScriptBaseOverlay, refCount) == 0x0);

struct ActiveEntityScriptPairOverlay {
    std::uint32_t keyLow;
    std::uint32_t keyHigh;
    CActiveEntityScriptBase* ptr;
};
static_assert(sizeof(ActiveEntityScriptPairOverlay) == 0x0C);
static_assert(offsetof(ActiveEntityScriptPairOverlay, keyLow) == 0x00);
static_assert(offsetof(ActiveEntityScriptPairOverlay, keyHigh) == 0x04);
static_assert(offsetof(ActiveEntityScriptPairOverlay, ptr) == 0x08);

struct VectorOverlay {
    ActiveEntityScriptPairOverlay* begin;
    ActiveEntityScriptPairOverlay* end;
    ActiveEntityScriptPairOverlay* capacityEnd;
};
static_assert(sizeof(VectorOverlay) == 0x0C);
static_assert(offsetof(VectorOverlay, begin) == 0x00);
static_assert(offsetof(VectorOverlay, end) == 0x04);
static_assert(offsetof(VectorOverlay, capacityEnd) == 0x08);

extern ActiveEntityScriptPairOverlay* _Uninit_copy(
    const ActiveEntityScriptPairOverlay* first,
    const ActiveEntityScriptPairOverlay* last,
    ActiveEntityScriptPairOverlay* dest,
    char* state);

extern ActiveEntityScriptPairOverlay* _Uninit_fill_n(
    ActiveEntityScriptPairOverlay* dest,
    unsigned int count,
    const ActiveEntityScriptPairOverlay* value,
    char* state);

extern void _Destroy_range(
    ActiveEntityScriptPairOverlay* first,
    ActiveEntityScriptPairOverlay* last,
    char* state);

void Vector_Reallocate(
    VectorOverlay* self,
    ActiveEntityScriptPairOverlay* insertPos,
    const ActiveEntityScriptPairOverlay* value,
    std::uint32_t /*unused*/,
    unsigned int count,
    char state)
{
    const unsigned int currentCount =
        static_cast<unsigned int>((reinterpret_cast<std::uintptr_t>(self->end) -
                                   reinterpret_cast<std::uintptr_t>(self->begin)) / 0x0Cu);

    const unsigned int* growthSource = &count;
    unsigned int currentCountCopy = currentCount;
    if (count <= currentCount) {
        growthSource = &currentCountCopy;
    }

    const unsigned int newCount = *growthSource + currentCount;

    void* newStorage;
    if (newCount == 0) {
        newStorage = nullptr;
    } else {
        newStorage = std::malloc(static_cast<std::size_t>(newCount) * 0x0Cu);
    }

    ActiveEntityScriptPairOverlay* newInsertPos =
        _Uninit_copy(self->begin, insertPos, static_cast<ActiveEntityScriptPairOverlay*>(newStorage), &state);

    if (count == 1) {
        if (newInsertPos != nullptr) {
            newInsertPos->keyLow = value->keyLow;
            newInsertPos->keyHigh = value->keyHigh;
            CActiveEntityScriptBase* const copiedPtr = value->ptr;
            newInsertPos->ptr = copiedPtr;
            if (copiedPtr != nullptr) {
                auto* const refCounted =
                    reinterpret_cast<RefCountedActiveEntityScriptBaseOverlay*>(copiedPtr);
                refCounted->refCount = refCounted->refCount + 1;
            }
        }

        newInsertPos = reinterpret_cast<ActiveEntityScriptPairOverlay*>(
            reinterpret_cast<std::uint8_t*>(newInsertPos) + 0x0C);
    } else {
        newInsertPos = _Uninit_fill_n(newInsertPos, count, value, &state);
    }

    if (state == '\0') {
        newInsertPos = _Uninit_copy(insertPos, self->end, newInsertPos, &state);
    }

    _Destroy_range(self->begin, self->end, &state);

    if (self->begin != nullptr) {
        std::free(self->begin);
    }

    self->end = newInsertPos;
    self->begin = static_cast<ActiveEntityScriptPairOverlay*>(newStorage);
    self->capacityEnd = reinterpret_cast<ActiveEntityScriptPairOverlay*>(
        reinterpret_cast<std::uint8_t*>(newStorage) + static_cast<std::size_t>(newCount) * 0x0Cu);
}