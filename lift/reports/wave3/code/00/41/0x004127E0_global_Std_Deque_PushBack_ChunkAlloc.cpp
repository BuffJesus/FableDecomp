#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct StdDequeOverlay {
    std::byte pad_00[0x10];
    std::uint32_t* off_10;
    std::int32_t off_14;
    std::int32_t off_18;
    std::int32_t off_1C;
    std::int32_t off_20;
    std::int32_t off_24;
};

static_assert(offsetof(StdDequeOverlay, off_10) == 0x10);
static_assert(offsetof(StdDequeOverlay, off_14) == 0x14);
static_assert(offsetof(StdDequeOverlay, off_18) == 0x18);
static_assert(offsetof(StdDequeOverlay, off_1C) == 0x1C);
static_assert(offsetof(StdDequeOverlay, off_20) == 0x20);
static_assert(offsetof(StdDequeOverlay, off_24) == 0x24);

void __thiscall Std_Deque_Resize_WithGap(StdDequeOverlay* self, int gap_count, int gap_side);

void __thiscall Std_Deque_PushBack_ChunkAlloc(StdDequeOverlay* self, const std::uint32_t* value_ptr)
{
    const std::uint32_t value = *value_ptr;

    if (static_cast<std::uint32_t>(self->off_24 - ((self->off_1C - self->off_20) >> 2)) < 2) {
        Std_Deque_Resize_WithGap(self, 1, 0);
    }

    void* const new_chunk = std::malloc(0x80);
    *reinterpret_cast<void**>(self->off_1C + 4) = new_chunk;

    if (self->off_10 != nullptr) {
        *self->off_10 = value;
    }

    std::int32_t* const next_slot = reinterpret_cast<std::int32_t*>(self->off_1C + 4);
    self->off_1C = reinterpret_cast<std::int32_t>(next_slot);

    const std::int32_t chunk_base = *next_slot;
    self->off_14 = chunk_base;
    self->off_18 = chunk_base + 0x80;
    self->off_10 = reinterpret_cast<std::uint32_t*>(self->off_14);
}