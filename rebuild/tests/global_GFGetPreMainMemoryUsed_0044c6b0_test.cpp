#include <cstdio>

long g_GFPreMainMemoryUsed = 0;

long __fastcall GFGetPreMainMemoryUsed()
{
    return g_GFPreMainMemoryUsed;
}

int main()
{
    g_GFPreMainMemoryUsed = 0x12345678;
    if (GFGetPreMainMemoryUsed() != 0x12345678) { std::printf("FAIL\n"); return 1; }
    g_GFPreMainMemoryUsed = -1;
    if (GFGetPreMainMemoryUsed() != -1) { std::printf("FAIL\n"); return 1; }
    std::printf("_global_0044c6b0_TEST PASS\n");
    return 0;
}