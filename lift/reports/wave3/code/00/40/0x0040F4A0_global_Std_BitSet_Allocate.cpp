#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct StdBitSetOverlay {
    std::uint32_t current_chunk;          // 0x00
    std::uint32_t first_chunk;            // 0x04
    std::uint32_t first_chunk_end;        // 0x08
    std::uint32_t* chunk_table_center;    // 0x0C
    std::uint32_t tail_ptr;               // 0x10
    std::uint32_t last_chunk;             // 0x14
    std::uint32_t last_chunk_end;         // 0x18
    std::uint32_t* chunk_table_last;      // 0x1C
    void* chunk_table_storage;            // 0x20
    std::uint32_t chunk_table_capacity;   // 0x24
};

static_assert(offsetof(StdBitSetOverlay, current_chunk) == 0x00);
static_assert(offsetof(StdBitSetOverlay, first_chunk) == 0x04);
static_assert(offsetof(StdBitSetOverlay, first_chunk_end) == 0x08);
static_assert(offsetof(StdBitSetOverlay, chunk_table_center) == 0x0C);
static_assert(offsetof(StdBitSetOverlay, tail_ptr) == 0x10);
static_assert(offsetof(StdBitSetOverlay, last_chunk) == 0x14);
static_assert(offsetof(StdBitSetOverlay, last_chunk_end) == 0x18);
static_assert(offsetof(StdBitSetOverlay, chunk_table_last) == 0x1C);
static_assert(offsetof(StdBitSetOverlay, chunk_table_storage) == 0x20);
static_assert(offsetof(StdBitSetOverlay, chunk_table_capacity) == 0x24);

void __thiscall Std_BitSet_Allocate(StdBitSetOverlay* self, std::uint32_t bit_count) {
    const std::uint32_t chunk_count = (bit_count >> 5) + 1;
    const std::uint32_t grow_count = (bit_count >> 5) + 3;
    const std::uint32_t minimum_capacity = 8;

    const std::uint32_t capacity = (grow_count < 9) ? minimum_capacity : grow_count;
    self->chunk_table_capacity = capacity;

    void* table_storage;
    if (capacity == 0) {
        table_storage = nullptr;
    } else {
        table_storage = std::malloc(capacity << 2);
    }

    auto* const centered_table = reinterpret_cast<std::uint32_t*>(
        reinterpret_cast<std::uintptr_t>(table_storage) + (((self->chunk_table_capacity - chunk_count) >> 1) * 4u)
    );
    auto* const table_end = centered_table + chunk_count;

    self->chunk_table_storage = table_storage;

    for (auto* it = centered_table; it < table_end; ++it) {
        void* const chunk = std::malloc(0x80);
        *it = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(chunk));
    }

    self->chunk_table_center = centered_table;

    const std::uint32_t first_chunk = *centered_table;
    self->first_chunk = first_chunk;
    self->first_chunk_end = first_chunk + 0x80;

    self->chunk_table_last = table_end - 1;

    const std::uint32_t last_chunk = table_end[-1];
    self->last_chunk = last_chunk;
    self->last_chunk_end = last_chunk + 0x80;

    self->current_chunk = self->first_chunk;
    self->tail_ptr = self->last_chunk + (bit_count & 0x1f) * 4;
}