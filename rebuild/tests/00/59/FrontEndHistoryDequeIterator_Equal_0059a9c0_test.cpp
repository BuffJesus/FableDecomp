#include <cstdio>

struct FrontEndHistoryDequeIterator_0059a9c0
{
    void** current;
    void** first;
    void** last;
    void*** node;
};

extern "C" bool __cdecl FrontEndHistoryDequeIterator_Equal_0059a9c0(
    const FrontEndHistoryDequeIterator_0059a9c0*,
    const FrontEndHistoryDequeIterator_0059a9c0*);

static FrontEndHistoryDequeIterator_0059a9c0 MakeIterator(
    void** current,
    void** first,
    void** last,
    void*** node)
{
    FrontEndHistoryDequeIterator_0059a9c0 iterator;
    iterator.current = current;
    iterator.first = first;
    iterator.last = last;
    iterator.node = node;
    return iterator;
}

int main()
{
    int failures = 0;
    void* block_a[32];
    void* block_b[32];
    void** map[2] = { block_a, block_b };

    FrontEndHistoryDequeIterator_0059a9c0 left =
        MakeIterator(block_a + 5, block_a, block_a + 32, map);
    FrontEndHistoryDequeIterator_0059a9c0 same_current_different_layout =
        MakeIterator(block_a + 5, block_b, block_b + 32, map + 1);
    FrontEndHistoryDequeIterator_0059a9c0 different_current =
        MakeIterator(block_a + 6, block_a, block_a + 32, map);

    if (!FrontEndHistoryDequeIterator_Equal_0059a9c0(
            &left,
            &same_current_different_layout))
    {
        std::printf("current-only equality failed\n");
        ++failures;
    }

    if (FrontEndHistoryDequeIterator_Equal_0059a9c0(
            &left,
            &different_current))
    {
        std::printf("different-current inequality failed\n");
        ++failures;
    }

    if (!FrontEndHistoryDequeIterator_Equal_0059a9c0(&left, &left))
    {
        std::printf("self equality failed\n");
        ++failures;
    }

    if (failures != 0)
    {
        std::printf(
            "FRONTEND_0059a9c0_TEST FAIL count=%d\n",
            failures);
        return 1;
    }

    std::printf("FRONTEND_0059a9c0_TEST PASS\n");
    return 0;
}
