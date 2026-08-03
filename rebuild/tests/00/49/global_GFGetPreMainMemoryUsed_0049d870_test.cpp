#include <cstdio>

unsigned int g_preMainMemoryUsed;

unsigned int GFGetPreMainMemoryUsed()
{
    return g_preMainMemoryUsed;
}

int main()
{
    g_preMainMemoryUsed = 0xDEADBEEF;
    unsigned int v = GFGetPreMainMemoryUsed();
    if (v != 0xDEADBEEF) { std::printf("FAIL got %08X\n", v); return 1; }
    g_preMainMemoryUsed = 0x12345678;
    if (GFGetPreMainMemoryUsed() != 0x12345678) { std::printf("FAIL2\n"); return 1; }
    std::printf("OK_0x0049d870\n");
    return 0;
}