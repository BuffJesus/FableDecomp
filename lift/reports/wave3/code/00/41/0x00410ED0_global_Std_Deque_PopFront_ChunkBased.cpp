#include <cstddef>
#include <cstdint>
#include <cstdlib>

#pragma pack(push, 1)
struct StdDequeChunkBasedOverlay
{
    std::int32_t current;   // 0x00
    std::int32_t chunkBase; // 0x04
    std::int32_t chunkEnd;  // 0x08
    std::int32_t mapNode;   // 0x0C
};
#pragma pack(pop)

static_assert(offsetof(StdDequeChunkBasedOverlay, current) == 0x00);
static_assert(offsetof(StdDequeChunkBasedOverlay, chunkBase) == 0x04);
static_assert(offsetof(StdDequeChunkBasedOverlay, chunkEnd) == 0x08);
static_assert(offsetof(StdDequeChunkBasedOverlay, mapNode) == 0x0C);
static_assert(sizeof(StdDequeChunkBasedOverlay) == 0x10);

void __fastcall Std_Deque_PopFront_ChunkBased(StdDequeChunkBasedOverlay* deque)
{
    std::int32_t value;

    if (deque->current != deque->chunkEnd + -4) {
        deque->current = deque->current + 4;
        return;
    }

    if (deque->chunkBase != 0) {
        std::free(reinterpret_cast<void*>(deque->chunkBase));
    }

    value = deque->mapNode;
    deque->mapNode = value + 4;
    value = *reinterpret_cast<std::int32_t*>(value + 4);
    deque->chunkBase = value;
    deque->chunkEnd = value + 0x80;
    deque->current = deque->chunkBase;
}