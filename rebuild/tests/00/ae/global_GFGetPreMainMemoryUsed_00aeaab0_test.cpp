#include <cstdio>
extern "C" long g_GFPreMainMemoryUsed;
long g_GFPreMainMemoryUsed = 0;

long __fastcall GFGetPreMainMemoryUsed();

int main()
{
    g_GFPreMainMemoryUsed = 0x12345678;
    long v = GFGetPreMainMemoryUsed();
    if (v == 0x12345678)
        printf("00aeaab0_TEST PASS\n");
    else
        printf("FAIL %ld\n", v);
    return 0;
}