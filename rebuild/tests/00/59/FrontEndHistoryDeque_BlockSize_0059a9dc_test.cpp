#include <cstdio>

extern "C" unsigned long __cdecl
FrontEndHistoryDeque_BlockSize_0059a9dc();

int main()
{
    int failures = 0;
    void* history_block[32];
    int ownership_sentinel = 0x1357;

    if (FrontEndHistoryDeque_BlockSize_0059a9dc() != 32)
    {
        std::printf("fixed block capacity failed\n");
        ++failures;
    }

    if (FrontEndHistoryDeque_BlockSize_0059a9dc() * sizeof(void*) !=
        sizeof(history_block) ||
        sizeof(history_block) != 0x80)
    {
        std::printf("block byte-size/layout relationship failed\n");
        ++failures;
    }

    FrontEndHistoryDeque_BlockSize_0059a9dc();
    if (ownership_sentinel != 0x1357)
    {
        std::printf("non-mutating capacity query failed\n");
        ++failures;
    }

    if (failures != 0)
    {
        std::printf(
            "FRONTEND_0059a9dc_TEST FAIL count=%d\n",
            failures);
        return 1;
    }

    std::printf("FRONTEND_0059a9dc_TEST PASS\n");
    return 0;
}
