#include <cstdio>

struct FrontEndHistoryDequeIterator_0059aa1d
{
    void** current;
    void** first;
    void** last;
    void*** node;

    void** BlockBegin() const;
};

int main()
{
    int failures = 0;
    void* block_a[32];
    void* block_b[32];
    void** map[2];
    FrontEndHistoryDequeIterator_0059aa1d iterator;

    map[0] = block_a;
    map[1] = block_b;
    iterator.current = block_a + 9;
    iterator.first = block_a;
    iterator.last = block_a + 32;
    iterator.node = map;

    if (iterator.BlockBegin() != block_a ||
        iterator.BlockBegin() == iterator.current ||
        iterator.BlockBegin() == iterator.last)
    {
        std::printf("block-begin field selection failed\n");
        ++failures;
    }

    iterator.first = block_b;
    if (iterator.BlockBegin() != block_b ||
        iterator.current != block_a + 9 ||
        iterator.last != block_a + 32 ||
        iterator.node != map)
    {
        std::printf("block-begin alias/layout preservation failed\n");
        ++failures;
    }

    if (failures != 0)
    {
        std::printf(
            "FRONTEND_0059aa1d_TEST FAIL count=%d\n",
            failures);
        return 1;
    }

    std::printf("FRONTEND_0059aa1d_TEST PASS\n");
    return 0;
}
