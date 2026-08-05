#include <cstdio>

struct FrontEndHistoryDequeIterator_0059a978
{
    void** current;
    void** first;
    void** last;
    void*** node;
};

extern "C" void**** __cdecl
FrontEndHistoryDequeIterator_NodeSlot_0059a978(
    FrontEndHistoryDequeIterator_0059a978*);

int main()
{
    int failures = 0;
    void* block_a[32];
    void* block_b[32];
    void** map[2] = { block_a, block_b };
    FrontEndHistoryDequeIterator_0059a978 iterator;

    iterator.current = block_a + 6;
    iterator.first = block_a;
    iterator.last = block_a + 32;
    iterator.node = map;

    void**** slot =
        FrontEndHistoryDequeIterator_NodeSlot_0059a978(&iterator);
    if (slot != &iterator.node ||
        *slot != map ||
        (char*)slot - (char*)&iterator != 0x0c)
    {
        std::printf("node slot address/value failed\n");
        ++failures;
    }

    *slot = map + 1;
    if (iterator.node != map + 1 ||
        iterator.current != block_a + 6 ||
        iterator.first != block_a ||
        iterator.last != block_a + 32)
    {
        std::printf("node-only write-through failed\n");
        ++failures;
    }

    if (failures != 0)
    {
        std::printf(
            "FRONTEND_0059a978_TEST FAIL count=%d\n",
            failures);
        return 1;
    }

    std::printf("FRONTEND_0059a978_TEST PASS\n");
    return 0;
}
