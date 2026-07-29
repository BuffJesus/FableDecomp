#include <cstdio>

#include "../../../src/compiled/00/59/FrontEndHistoryDeque_FreeBlocks_0059aa64.cpp"

static void* g_DeletedBlocks[8];
static int g_DeleteCount;

extern "C" void __cdecl
FrontEndHistoryBlockDelete_0059aa64(void* block)
{
    if (g_DeleteCount < 8)
    {
        g_DeletedBlocks[g_DeleteCount] = block;
    }
    ++g_DeleteCount;
}

static void ResetProbe()
{
    int index;
    g_DeleteCount = 0;
    for (index = 0; index != 8; ++index)
    {
        g_DeletedBlocks[index] = 0;
    }
}

int main()
{
    int failures = 0;
    int block_a[32];
    int block_b[32];
    int block_c[32];
    void* slots[5];

    slots[0] = block_a;
    slots[1] = 0;
    slots[2] = block_b;
    slots[3] = block_c;
    slots[4] = 0;

    ResetProbe();
    FrontEndHistoryDeque_FreeBlocks_0059aa64(
        0,
        0,
        slots,
        slots + 5);
    if (g_DeleteCount != 3 ||
        g_DeletedBlocks[0] != block_a ||
        g_DeletedBlocks[1] != block_b ||
        g_DeletedBlocks[2] != block_c)
    {
        std::printf("forward non-null cleanup order failed\n");
        ++failures;
    }

    // Cleanup owns the pointed-to blocks but not the map slots themselves.
    if (slots[0] != block_a ||
        slots[1] != 0 ||
        slots[2] != block_b ||
        slots[3] != block_c ||
        slots[4] != 0)
    {
        std::printf("map slot preservation failed\n");
        ++failures;
    }

    ResetProbe();
    FrontEndHistoryDeque_FreeBlocks_0059aa64(
        0,
        0,
        slots + 2,
        slots + 2);
    if (g_DeleteCount != 0)
    {
        std::printf("empty range cleanup failed\n");
        ++failures;
    }

    if (failures != 0)
    {
        std::printf(
            "FRONTEND_0059aa64_TEST FAIL count=%d\n",
            failures);
        return 1;
    }

    std::printf("FRONTEND_0059aa64_TEST PASS\n");
    return 0;
}
