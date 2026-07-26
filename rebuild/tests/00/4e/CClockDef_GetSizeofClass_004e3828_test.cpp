#include <cstdio>
int __fastcall CClockDef_GetSizeofClass()
{
    return 56;
}
int main()
{
    if (CClockDef_GetSizeofClass() == 56) { std::printf("AUTO_TINY_004e3828_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004e3828_TEST FAIL\n");
    return 1;
}