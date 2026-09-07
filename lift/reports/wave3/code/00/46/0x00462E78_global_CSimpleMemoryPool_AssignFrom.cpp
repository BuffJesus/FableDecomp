#include <cstddef>
#include <cstdint>
#include <cstring>

namespace
{
struct CSimpleMemoryPool_Overlay
{
    char* base;    // +0x00
    char* current; // +0x04
    char* end;     // +0x08
};

static_assert(offsetof(CSimpleMemoryPool_Overlay, base) == 0x00);
static_assert(offsetof(CSimpleMemoryPool_Overlay, current) == 0x04);
static_assert(offsetof(CSimpleMemoryPool_Overlay, end) == 0x08);
static_assert(sizeof(CSimpleMemoryPool_Overlay) == 0x0C);
}

CSimpleMemoryPool* __thiscall CSimpleMemoryPool_AssignFrom(
    CSimpleMemoryPool* self,
    CSimpleMemoryPool* other)
{
    auto& dst = *reinterpret_cast<CSimpleMemoryPool_Overlay*>(self);
    auto& src = *reinterpret_cast<CSimpleMemoryPool_Overlay*>(other);

    if (other != self)
    {
        char* const srcCurrent = src.current;
        char* const srcBase = src.base;
        char* const dstBase = dst.base;
        const std::uint32_t usedBytes = static_cast<std::uint32_t>(srcCurrent - srcBase);

        if (static_cast<std::uint32_t>(dst.end - dstBase) < usedBytes)
        {
            char* const newBase = self->AllocateType<char>(usedBytes);
            reinterpret_cast<CFileInstaller::CActiveFile*>(self)->OnReadFinished();
            dst.base = newBase;
            dst.end = newBase + usedBytes;
        }
        else if (static_cast<std::uint32_t>(dst.current - dstBase) < usedBytes)
        {
            char* const copiedEnd =
                srcBase + static_cast<std::uint32_t>(dst.current - dstBase);

            if (copiedEnd != srcBase)
            {
                std::memmove(dstBase, srcBase, copiedEnd - srcBase);
            }

            std::uint32_t maskedSelf =
                static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(self)) & 0x00FFFFFF;
            Copy_MemoryRange(dst.current, reinterpret_cast<std::uint8_t*>(&maskedSelf) + 3);
        }
        else if (srcCurrent != srcBase)
        {
            std::memmove(dstBase, srcBase, srcCurrent - srcBase);
        }

        dst.current = dst.base + usedBytes;
    }

    return self;
}