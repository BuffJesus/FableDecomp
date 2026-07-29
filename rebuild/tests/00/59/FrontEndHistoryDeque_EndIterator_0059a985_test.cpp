#include <cstdio>

#include "../../../src/compiled/00/59/FrontEndHistoryDeque_EndIterator_0059a985.cpp"

int main()
{
    int failures = 0;
    void* begin_block[32];
    void* end_block[32];
    void** map[2] = { begin_block, end_block };
    FrontEndHistoryDeque_0059a985 history;

    history.begin.current = begin_block + 4;
    history.begin.first = begin_block;
    history.begin.last = begin_block + 32;
    history.begin.node = map;
    history.end.current = end_block + 9;
    history.end.first = end_block;
    history.end.last = end_block + 32;
    history.end.node = map + 1;

    FrontEndHistoryDequeIterator_0059a985* end =
        FrontEndHistoryDeque_EndIterator_0059a985(&history);
    if (end != &history.end ||
        (char*)end - (char*)&history != 0x10 ||
        end->current != end_block + 9 ||
        end->node != map + 1)
    {
        std::printf("end iterator address/layout failed\n");
        ++failures;
    }

    end->current = end_block + 12;
    end->node = map;
    if (history.end.current != end_block + 12 ||
        history.end.node != map ||
        history.begin.current != begin_block + 4 ||
        history.begin.node != map)
    {
        std::printf("end-only aliasing failed\n");
        ++failures;
    }

    if (failures != 0)
    {
        std::printf(
            "FRONTEND_0059a985_TEST FAIL count=%d\n",
            failures);
        return 1;
    }

    std::printf("FRONTEND_0059a985_TEST PASS\n");
    return 0;
}
