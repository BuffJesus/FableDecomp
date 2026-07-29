#include <cstdio>

#include "../../../src/compiled/00/59/FrontEndHistoryDequeIterator_Current_0059aae0.cpp"

int main()
{
    int failures = 0;
    void* block_a[32];
    void* block_b[32];
    void** map[2];
    FrontEndHistoryDequeIterator_0059aae0 iterator;

    map[0] = block_a;
    map[1] = block_b;
    iterator.current = block_a + 7;
    iterator.first = block_a;
    iterator.last = block_a + 32;
    iterator.node = map;

    if (iterator.Current() != block_a + 7 ||
        iterator.Current() == iterator.first ||
        iterator.Current() == iterator.last)
    {
        std::printf("current field selection failed\n");
        ++failures;
    }

    iterator.current = block_b + 4;
    if (iterator.Current() != block_b + 4 ||
        iterator.first != block_a ||
        iterator.last != block_a + 32 ||
        iterator.node != map)
    {
        std::printf("current alias/layout preservation failed\n");
        ++failures;
    }

    if (failures != 0)
    {
        std::printf(
            "FRONTEND_0059aae0_TEST FAIL count=%d\n",
            failures);
        return 1;
    }

    std::printf("FRONTEND_0059aae0_TEST PASS\n");
    return 0;
}
