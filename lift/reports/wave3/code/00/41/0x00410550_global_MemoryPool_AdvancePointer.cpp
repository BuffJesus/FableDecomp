#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct MemoryPoolOverlay {
    std::uint32_t current;         // 0x00
    std::uint32_t activeBlock;     // 0x04
    std::uint32_t activeBlockEnd;  // 0x08
    std::uint32_t rotationCursor;  // 0x0C
};

static_assert(offsetof(MemoryPoolOverlay, current) == 0x00);
static_assert(offsetof(MemoryPoolOverlay, activeBlock) == 0x04);
static_assert(offsetof(MemoryPoolOverlay, activeBlockEnd) == 0x08);
static_assert(offsetof(MemoryPoolOverlay, rotationCursor) == 0x0C);
static_assert(sizeof(MemoryPoolOverlay) == 0x10);
static_assert(sizeof(void*) == 4, "MemoryPool_AdvancePointer is a 32-bit binary layout.");

void __fastcall MemoryPool_AdvancePointer(MemoryPoolOverlay* pool)
{
    if (pool->current != pool->activeBlockEnd - 4) {
        pool->current = pool->current + 4;
        return;
    }

    if (pool->activeBlock != 0) {
        std::free(reinterpret_cast<void*>(pool->activeBlock));
    }

    std::uint32_t cursor = pool->rotationCursor;
    pool->rotationCursor = cursor + 4;
    cursor = *reinterpret_cast<std::uint32_t*>(cursor + 4);
    pool->activeBlock = cursor;
    pool->activeBlockEnd = cursor + 0x80;
    pool->current = pool->activeBlock;
}