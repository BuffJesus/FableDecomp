#include <algorithm>
#include <cstddef>
#include <cstdint>

struct C3DClothQuad {
    std::uint32_t dword0;
    std::uint32_t dword4;
    std::uint32_t dword8;
    std::uint32_t dwordC;
};
static_assert(sizeof(C3DClothQuad) == 0x10);

struct ClothQuadVectorOverlay {
    C3DClothQuad* begin;
    C3DClothQuad* end;
    C3DClothQuad* capacityEnd;
};
static_assert(offsetof(ClothQuadVectorOverlay, begin) == 0x0);
static_assert(offsetof(ClothQuadVectorOverlay, end) == 0x4);
static_assert(offsetof(ClothQuadVectorOverlay, capacityEnd) == 0x8);

extern void Vector_Reallocate(
    C3DClothQuad* insertPos,
    const C3DClothQuad* value,
    void* countBytePtrPlus3,
    std::uint32_t originalCount,
    int zeroArg);

void __thiscall Vector_Insert(
    ClothQuadVectorOverlay* self,
    C3DClothQuad* insertPos,
    std::uint32_t count,
    const C3DClothQuad* value)
{
    const std::uint32_t originalCount = count;
    if (count == 0) {
        return;
    }

    if (static_cast<std::uint32_t>(self->capacityEnd - self->end) < count) {
        count &= 0x00FFFFFFu;
        Vector_Reallocate(
            insertPos,
            value,
            reinterpret_cast<std::uint8_t*>(&count) + 3,
            originalCount,
            0);
        return;
    }

    const std::uint32_t saved0 = value->dword0;
    const std::uint32_t saved4 = value->dword4;
    const std::uint32_t saved8 = value->dword8;
    const std::uint32_t savedC = value->dwordC;
    const C3DClothQuad savedValue{saved0, saved4, saved8, savedC};

    const std::uint32_t trailingCount =
        static_cast<std::uint32_t>(self->end - insertPos);
    C3DClothQuad* oldEnd = self->end;

    if (count < trailingCount) {
        C3DClothQuad* tailStart = oldEnd - count;
        C3DClothQuad* writePtr = oldEnd;

        for (C3DClothQuad* readPtr = tailStart; readPtr != oldEnd; ++readPtr) {
            if (writePtr != nullptr) {
                writePtr->dword0 = readPtr->dword0;
                writePtr->dword4 = readPtr->dword4;
                writePtr->dword8 = readPtr->dword8;
                writePtr->dwordC = readPtr->dwordC;
            }
            ++writePtr;
        }

        self->end += count;

        int middleCount = static_cast<int>(tailStart - insertPos);
        if (middleCount > 0) {
            C3DClothQuad* backwardSrc = tailStart;
            C3DClothQuad* backwardDst = oldEnd;
            do {
                --middleCount;
                backwardDst[-1].dword0 = backwardSrc[-1].dword0;
                backwardDst[-1].dword4 = backwardSrc[-1].dword4;
                backwardDst[-1].dword8 = backwardSrc[-1].dword8;
                backwardDst[-1].dwordC = backwardSrc[-1].dwordC;
                --backwardSrc;
                --backwardDst;
            } while (middleCount != 0);
        }

        C3DClothQuad* fillEnd = insertPos + count;
        while (insertPos != fillEnd) {
            insertPos->dword0 = saved0;
            insertPos->dword4 = saved4;
            insertPos->dword8 = saved8;
            insertPos->dwordC = savedC;
            ++insertPos;
        }
    } else {
        const std::uint32_t extraCount = count - trailingCount;
        C3DClothQuad* originalEnd = oldEnd;

        std::fill_n(oldEnd, extraCount, savedValue);
        self->end += extraCount;

        C3DClothQuad* writePtr = self->end;
        for (C3DClothQuad* readPtr = insertPos; readPtr != originalEnd; ++readPtr) {
            if (writePtr != nullptr) {
                writePtr->dword0 = readPtr->dword0;
                writePtr->dword4 = readPtr->dword4;
                writePtr->dword8 = readPtr->dword8;
                writePtr->dwordC = readPtr->dwordC;
            }
            ++writePtr;
        }

        self->end += trailingCount;

        if (insertPos != originalEnd) {
            do {
                insertPos->dword0 = saved0;
                insertPos->dword4 = saved4;
                insertPos->dword8 = saved8;
                insertPos->dwordC = savedC;
                ++insertPos;
            } while (insertPos != originalEnd);
        }
    }
}