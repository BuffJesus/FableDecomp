#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct VectorOverlay
{
    std::uint32_t* begin;    // 0x00
    std::uint32_t* current;  // 0x04
    std::uint32_t* end;      // 0x08
};

static_assert(offsetof(VectorOverlay, begin) == 0x00);
static_assert(offsetof(VectorOverlay, current) == 0x04);
static_assert(offsetof(VectorOverlay, end) == 0x08);
static_assert(sizeof(VectorOverlay) == 0x0C);

void __thiscall Vector_Reallocate(
    VectorOverlay* self,
    std::uint32_t* insertPos,
    std::uint32_t* valuePtr,
    std::uint32_t /*unused*/,
    std::uint32_t count,
    char skipTailCopy)
{
    std::uint32_t* copyDst;
    std::uint32_t insertedCount;
    std::uint32_t* copySrc;
    std::uint32_t* oldCurrent;
    int tailDelta;
    int newCapacity;
    std::uint32_t local_8;

    insertedCount = count;
    local_8 = static_cast<std::uint32_t>(self->current - self->begin);

    std::uint32_t* growthPtr = &count;
    if (count <= local_8)
    {
        growthPtr = &local_8;
    }

    newCapacity = static_cast<int>(*growthPtr + local_8);
    std::uint32_t* newBuffer;
    if (newCapacity == 0)
    {
        newBuffer = nullptr;
    }
    else
    {
        newBuffer = static_cast<std::uint32_t*>(std::malloc(static_cast<std::size_t>(newCapacity) * 4));
    }

    copyDst = newBuffer;
    for (copySrc = self->begin; copySrc != insertPos; ++copySrc)
    {
        if (copyDst != nullptr)
        {
            *copyDst = *copySrc;
        }
        ++copyDst;
    }

    if (insertedCount == 1)
    {
        if (copyDst != nullptr)
        {
            *copyDst = *valuePtr;
        }
        ++copyDst;
    }
    else
    {
        for (; insertedCount != 0; --insertedCount)
        {
            if (copyDst != nullptr)
            {
                *copyDst = *valuePtr;
            }
            ++copyDst;
        }
    }

    if (skipTailCopy == '\0')
    {
        oldCurrent = self->current;
        if (insertPos != oldCurrent)
        {
            tailDelta = static_cast<int>(
                reinterpret_cast<std::uintptr_t>(insertPos) -
                reinterpret_cast<std::uintptr_t>(copyDst));

            do
            {
                if (copyDst != nullptr)
                {
                    *copyDst = *reinterpret_cast<std::uint32_t*>(
                        reinterpret_cast<std::uintptr_t>(copyDst) + tailDelta);
                }
                ++copyDst;
            } while (reinterpret_cast<std::uint32_t*>(
                         reinterpret_cast<std::uintptr_t>(copyDst) + tailDelta) != oldCurrent);
        }
    }

    if (self->begin != nullptr)
    {
        std::free(self->begin);
    }

    self->begin = newBuffer;
    self->current = copyDst;
    self->end = newBuffer + newCapacity;
}