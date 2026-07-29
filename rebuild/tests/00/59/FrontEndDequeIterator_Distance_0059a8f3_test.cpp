#include <cstdio>

#include "../../../src/compiled/00/59/FrontEndDequeIterator_Distance_0059a8f3.cpp"

static CComponent_0059a8f3* g_Blocks[3][32];
static CComponent_0059a8f3** g_Nodes[3] =
{
    g_Blocks[0],
    g_Blocks[1],
    g_Blocks[2]
};

static FrontEndDequeIterator_0059a8f3 MakeIterator(
    int node,
    int index)
{
    FrontEndDequeIterator_0059a8f3 iterator;
    iterator.current = g_Blocks[node] + index;
    iterator.first = g_Blocks[node];
    iterator.last = g_Blocks[node] + 32;
    iterator.node = g_Nodes + node;
    return iterator;
}

int main()
{
    int failures = 0;

    FrontEndDequeIterator_0059a8f3 start = MakeIterator(1, 5);
    FrontEndDequeIterator_0059a8f3 finish = MakeIterator(1, 12);
    if (finish.Distance(start) != 7)
    {
        std::printf("same-block distance failed\n");
        ++failures;
    }

    start = MakeIterator(0, 28);
    finish = MakeIterator(2, 3);
    if (finish.Distance(start) != 39)
    {
        std::printf("multi-block distance failed\n");
        ++failures;
    }

    start = MakeIterator(2, 5);
    finish = MakeIterator(0, 10);
    if (finish.Distance(start) != -59)
    {
        std::printf("negative distance failed\n");
        ++failures;
    }

    start = MakeIterator(0, 0);
    finish = MakeIterator(0, 0);
    if (finish.Distance(start) != 0)
    {
        std::printf("zero distance failed\n");
        ++failures;
    }

    if (failures != 0)
    {
        std::printf(
            "FRONTEND_0059a8f3_TEST FAIL count=%d\n",
            failures);
        return 1;
    }

    std::printf("FRONTEND_0059a8f3_TEST PASS\n");
    return 0;
}
