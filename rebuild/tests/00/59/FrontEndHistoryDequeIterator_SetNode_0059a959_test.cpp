#include <cstdio>

struct FrontEndHistoryDequeIterator_0059a959
{
    void** current;
    void** first;
    void** last;
    void*** node;

    void SetNode(void*** new_node);
};

int main()
{
    int failures = 0;
    void* block_a[32];
    void* block_b[32];
    void** map[2] = { block_a, block_b };
    FrontEndHistoryDequeIterator_0059a959 iterator;

    iterator.current = block_a + 11;
    iterator.first = 0;
    iterator.last = 0;
    iterator.node = 0;

    iterator.SetNode(map + 1);
    if (iterator.node != map + 1 ||
        iterator.first != block_b ||
        iterator.last != block_b + 32 ||
        iterator.last - iterator.first != 32)
    {
        std::printf("node/block-bound binding failed\n");
        ++failures;
    }

    if (iterator.current != block_a + 11)
    {
        std::printf("current preservation failed\n");
        ++failures;
    }

    iterator.SetNode(map);
    if (iterator.node != map ||
        iterator.first != block_a ||
        iterator.last != block_a + 32 ||
        iterator.current != block_a + 11 ||
        map[0] != block_a ||
        map[1] != block_b)
    {
        std::printf("node rebinding/layout preservation failed\n");
        ++failures;
    }

    if (failures != 0)
    {
        std::printf(
            "FRONTEND_0059a959_TEST FAIL count=%d\n",
            failures);
        return 1;
    }

    std::printf("FRONTEND_0059a959_TEST PASS\n");
    return 0;
}
