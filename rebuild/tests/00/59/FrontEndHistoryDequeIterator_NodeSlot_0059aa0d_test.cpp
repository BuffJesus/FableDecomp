#include <cstdio>

#include "../../../src/compiled/00/59/FrontEndHistoryDequeIterator_NodeSlot_0059aa0d.cpp"

int main()
{
    int failures = 0;
    void* block_a[32];
    void* block_b[32];
    void** map[2];
    FrontEndHistoryDequeIterator_0059aa0d iterator;

    map[0] = block_a;
    map[1] = block_b;
    iterator.current = block_a + 3;
    iterator.first = block_a;
    iterator.last = block_a + 32;
    iterator.node = map;

    void**** slot =
        FrontEndHistoryDequeIterator_NodeSlot_0059aa0d(&iterator);
    if (slot != &iterator.node || *slot != map)
    {
        std::printf("node slot address/value failed\n");
        ++failures;
    }

    *slot = map + 1;
    if (iterator.node != map + 1 ||
        iterator.current != block_a + 3 ||
        iterator.first != block_a ||
        iterator.last != block_a + 32)
    {
        std::printf("node-only ownership slot alias failed\n");
        ++failures;
    }

    if (failures != 0)
    {
        std::printf(
            "FRONTEND_0059aa0d_TEST FAIL count=%d\n",
            failures);
        return 1;
    }

    std::printf("FRONTEND_0059aa0d_TEST PASS\n");
    return 0;
}
