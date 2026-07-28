#include <cstddef>
#include <cstdint>

struct BitsetAdvanceCursorOverlay {
    std::uint32_t unused_0x00;
    std::uint32_t next_block_base_0x04;
};

static_assert(offsetof(BitsetAdvanceCursorOverlay, next_block_base_0x04) == 0x04);

void __stdcall bitset_advance_iterator(
    std::uint32_t current,
    std::uint32_t /*unused*/,
    std::uint32_t block_end,
    std::uint32_t block_cursor,
    std::uint32_t target)
{
    while (current != target) {
        current = current + 4;
        if (current == block_end) {
            current = reinterpret_cast<const BitsetAdvanceCursorOverlay*>(block_cursor)->next_block_base_0x04;
            block_cursor = block_cursor + 4;
            block_end = current + 0x80;
        }
    }
}