#include <cstdio>

#include "../../../src/compiled/00/59/FrontEndHistoryDeque_AllocateBlocks_0059aa43.cpp"

static void* g_ReturnBlocks[8];
static unsigned long g_RequestedSizes[8];
static int g_AllocateCount;

extern "C" void* __cdecl
FrontEndHistoryBlockAllocate_0059aa43(unsigned long size)
{
    int index = g_AllocateCount;
    if (index < 8)
    {
        g_RequestedSizes[index] = size;
    }
    ++g_AllocateCount;
    return index < 8 ? g_ReturnBlocks[index] : 0;
}

static void ResetProbe()
{
    int index;
    g_AllocateCount = 0;
    for (index = 0; index != 8; ++index)
    {
        g_ReturnBlocks[index] = 0;
        g_RequestedSizes[index] = 0;
    }
}

int main()
{
    int failures = 0;
    int block_a[32];
    int block_c[32];
    int untouched_a = 1;
    int untouched_b = 2;
    void* slots[5];

    slots[0] = &untouched_a;
    slots[1] = &untouched_a;
    slots[2] = &untouched_a;
    slots[3] = &untouched_a;
    slots[4] = &untouched_b;

    ResetProbe();
    g_ReturnBlocks[0] = block_a;
    g_ReturnBlocks[1] = 0;
    g_ReturnBlocks[2] = block_c;
    FrontEndHistoryDeque_AllocateBlocks_0059aa43(
        0,
        0,
        slots + 1,
        slots + 4);

    if (g_AllocateCount != 3 ||
        g_RequestedSizes[0] != 0x80 ||
        g_RequestedSizes[1] != 0x80 ||
        g_RequestedSizes[2] != 0x80 ||
        slots[1] != block_a ||
        slots[2] != 0 ||
        slots[3] != block_c)
    {
        std::printf("forward allocation/store order failed\n");
        ++failures;
    }

    if (slots[0] != &untouched_a || slots[4] != &untouched_b)
    {
        std::printf("half-open range boundary failed\n");
        ++failures;
    }

    ResetProbe();
    FrontEndHistoryDeque_AllocateBlocks_0059aa43(
        0,
        0,
        slots + 2,
        slots + 2);
    if (g_AllocateCount != 0)
    {
        std::printf("empty range allocation failed\n");
        ++failures;
    }

    if (failures != 0)
    {
        std::printf(
            "FRONTEND_0059aa43_TEST FAIL count=%d\n",
            failures);
        return 1;
    }

    std::printf("FRONTEND_0059aa43_TEST PASS\n");
    return 0;
}
