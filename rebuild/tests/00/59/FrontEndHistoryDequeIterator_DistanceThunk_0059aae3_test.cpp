#include <cstdio>

#include "../../../src/compiled/00/59/FrontEndHistoryDequeIterator_DistanceThunk_0059aae3.cpp"

extern "C" long __fastcall
FrontEndHistoryDequeIteratorDistance_0059aae3(
    const FrontEndHistoryDequeIterator_0059aae3* finish,
    void*,
    const FrontEndHistoryDequeIterator_0059aae3* start)
{
    long node_delta = (long)(finish->node - start->node);
    long finish_prefix = (long)(finish->current - finish->first);
    long start_suffix = (long)(start->last - start->current);
    return node_delta * 32 + finish_prefix + start_suffix - 32;
}

static void* g_Blocks[3][32];
static void** g_Nodes[3] =
{
    g_Blocks[0],
    g_Blocks[1],
    g_Blocks[2]
};

static FrontEndHistoryDequeIterator_0059aae3 MakeIterator(
    int node,
    int index)
{
    FrontEndHistoryDequeIterator_0059aae3 iterator;
    iterator.current = g_Blocks[node] + index;
    iterator.first = g_Blocks[node];
    iterator.last = g_Blocks[node] + 32;
    iterator.node = g_Nodes + node;
    return iterator;
}

int main()
{
    int failures = 0;

    FrontEndHistoryDequeIterator_0059aae3 start = MakeIterator(1, 5);
    FrontEndHistoryDequeIterator_0059aae3 finish = MakeIterator(1, 12);
    if (finish.Distance(start) != 7)
    {
        std::printf("same-block thunk distance failed\n");
        ++failures;
    }

    start = MakeIterator(0, 28);
    finish = MakeIterator(2, 3);
    if (finish.Distance(start) != 39)
    {
        std::printf("cross-block thunk distance failed\n");
        ++failures;
    }

    start = MakeIterator(2, 5);
    finish = MakeIterator(0, 10);
    if (finish.Distance(start) != -59)
    {
        std::printf("reverse thunk distance failed\n");
        ++failures;
    }

    if (failures != 0)
    {
        std::printf(
            "FRONTEND_0059aae3_TEST FAIL count=%d\n",
            failures);
        return 1;
    }

    std::printf("FRONTEND_0059aae3_TEST PASS\n");
    return 0;
}
