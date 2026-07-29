#include <cstdio>

#include "../../../src/compiled/00/59/FrontEndHistoryDequeIterator_BlockEndSlot_0059a970.cpp"

int main()
{
    int failures = 0;
    void* block_a[32];
    void* block_b[32];
    void** map[2] = { block_a, block_b };
    FrontEndHistoryDequeIterator_0059a970 iterator;

    iterator.current = block_a + 7;
    iterator.first = block_a;
    iterator.last = block_a + 32;
    iterator.node = map;

    void*** slot =
        FrontEndHistoryDequeIterator_BlockEndSlot_0059a970(&iterator);
    if (slot != &iterator.last ||
        *slot != block_a + 32 ||
        (char*)slot - (char*)&iterator != 8)
    {
        std::printf("block-end slot address/value failed\n");
        ++failures;
    }

    *slot = block_b + 32;
    if (iterator.last != block_b + 32 ||
        iterator.current != block_a + 7 ||
        iterator.first != block_a ||
        iterator.node != map)
    {
        std::printf("block-end-only write-through failed\n");
        ++failures;
    }

    if (failures != 0)
    {
        std::printf(
            "FRONTEND_0059a970_TEST FAIL count=%d\n",
            failures);
        return 1;
    }

    std::printf("FRONTEND_0059a970_TEST PASS\n");
    return 0;
}
