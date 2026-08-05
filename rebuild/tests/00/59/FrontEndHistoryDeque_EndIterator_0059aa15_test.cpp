#include <cstdio>

struct FrontEndHistoryDequeIterator_0059aa15
{
    void** current;
    void** first;
    void** last;
    void*** node;
};

struct FrontEndHistoryDeque_0059aa15
{
    FrontEndHistoryDequeIterator_0059aa15 begin;
    FrontEndHistoryDequeIterator_0059aa15 end;
};

extern "C" FrontEndHistoryDequeIterator_0059aa15* __cdecl
FrontEndHistoryDeque_EndIterator_0059aa15(
    FrontEndHistoryDeque_0059aa15*);

int main()
{
    int failures = 0;
    void* begin_block[32];
    void* end_block[32];
    void** map[2];
    FrontEndHistoryDeque_0059aa15 history;

    map[0] = begin_block;
    map[1] = end_block;

    history.begin.current = begin_block + 2;
    history.begin.first = begin_block;
    history.begin.last = begin_block + 32;
    history.begin.node = map;
    history.end.current = end_block + 7;
    history.end.first = end_block;
    history.end.last = end_block + 32;
    history.end.node = map + 1;

    FrontEndHistoryDequeIterator_0059aa15* end =
        FrontEndHistoryDeque_EndIterator_0059aa15(&history);
    if (end != &history.end ||
        (char*)end - (char*)&history != 0x10)
    {
        std::printf("end iterator address/stride failed\n");
        ++failures;
    }

    end->current = end_block + 9;
    end->node = map;
    if (history.end.current != end_block + 9 ||
        history.end.node != map ||
        history.begin.current != begin_block + 2 ||
        history.begin.node != map)
    {
        std::printf("end-only aliasing failed\n");
        ++failures;
    }

    if (failures != 0)
    {
        std::printf(
            "FRONTEND_0059aa15_TEST FAIL count=%d\n",
            failures);
        return 1;
    }

    std::printf("FRONTEND_0059aa15_TEST PASS\n");
    return 0;
}
