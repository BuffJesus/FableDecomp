#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct VectorReallocateElement
{
    std::uint32_t dword0;
    std::uint32_t dword1;
    std::uint32_t dword2;
    std::uint32_t dword3;
};
static_assert(sizeof(VectorReallocateElement) == 0x10);

struct VectorReallocateOverlay
{
    VectorReallocateElement* begin;
    VectorReallocateElement* end;
    VectorReallocateElement* capacityEnd;
};
static_assert(offsetof(VectorReallocateOverlay, begin) == 0x0);
static_assert(offsetof(VectorReallocateOverlay, end) == 0x4);
static_assert(offsetof(VectorReallocateOverlay, capacityEnd) == 0x8);
static_assert(sizeof(VectorReallocateOverlay) == 0xC);
static_assert(sizeof(void*) == 4);

void __thiscall Vector_Reallocate(
    VectorReallocateOverlay* self,
    VectorReallocateElement* insertPos,
    const VectorReallocateElement* value,
    std::uint32_t /*unused*/,
    std::uint32_t insertCount,
    char skipTailCopy)
{
    const auto ptr_to_addr32 = [](const void* p) -> std::uint32_t
    {
        return static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(p));
    };

    const auto addr32_to_ptr = [](std::uint32_t addr) -> VectorReallocateElement*
    {
        return reinterpret_cast<VectorReallocateElement*>(static_cast<std::uintptr_t>(addr));
    };

    const auto copy_element = [&](std::uint32_t dstAddr, std::uint32_t srcAddr) -> void
    {
        VectorReallocateElement* const dst = addr32_to_ptr(dstAddr);
        const VectorReallocateElement* const src =
            reinterpret_cast<const VectorReallocateElement*>(static_cast<std::uintptr_t>(srcAddr));

        dst->dword0 = src->dword0;
        dst->dword1 = src->dword1;
        dst->dword2 = src->dword2;
        dst->dword3 = src->dword3;
    };

    const std::uint32_t beginAddr = ptr_to_addr32(self->begin);
    const std::uint32_t endAddr = ptr_to_addr32(self->end);
    const std::uint32_t insertPosAddr = ptr_to_addr32(insertPos);
    const std::uint32_t valueAddr = ptr_to_addr32(value);

    const std::uint32_t oldSize =
        static_cast<std::uint32_t>((static_cast<std::int32_t>(endAddr - beginAddr)) >> 4);

    const std::uint32_t growth = (insertCount <= oldSize) ? oldSize : insertCount;
    const std::uint32_t newCapacity = growth + oldSize;

    std::uint32_t newBeginAddr;
    if (newCapacity == 0)
    {
        newBeginAddr = 0;
    }
    else
    {
        newBeginAddr = ptr_to_addr32(std::malloc(static_cast<std::size_t>(newCapacity) * 0x10));
    }

    std::uint32_t writeAddr = newBeginAddr;
    for (std::uint32_t readAddr = beginAddr; readAddr != insertPosAddr; readAddr += 0x10)
    {
        if (writeAddr != 0)
        {
            copy_element(writeAddr, readAddr);
        }
        writeAddr += 0x10;
    }

    std::uint32_t remainingInsertCount = insertCount;
    if (insertCount == 1)
    {
        if (writeAddr != 0)
        {
            copy_element(writeAddr, valueAddr);
        }
        writeAddr += 0x10;
    }
    else
    {
        for (; remainingInsertCount != 0; --remainingInsertCount)
        {
            if (writeAddr != 0)
            {
                copy_element(writeAddr, valueAddr);
            }
            writeAddr += 0x10;
        }
    }

    if ((skipTailCopy == '\0') && (insertPosAddr != endAddr))
    {
        const std::uint32_t delta = insertPosAddr - writeAddr;
        do
        {
            if (writeAddr != 0)
            {
                copy_element(writeAddr, delta + writeAddr);
            }
            writeAddr += 0x10;
        } while ((delta + writeAddr) != endAddr);
    }

    if (self->begin != nullptr)
    {
        std::free(self->begin);
    }

    self->capacityEnd = addr32_to_ptr(newBeginAddr + (newCapacity << 4));
    self->end = addr32_to_ptr(writeAddr);
    self->begin = addr32_to_ptr(newBeginAddr);
}