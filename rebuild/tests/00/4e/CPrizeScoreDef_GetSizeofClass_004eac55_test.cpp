#include <cstdio>
int __fastcall CPrizeScoreDef_GetSizeofClass()
{
    return 48;
}
int main()
{
    if (CPrizeScoreDef_GetSizeofClass() == 48) { std::printf("AUTO_TINY_004eac55_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004eac55_TEST FAIL\n");
    return 1;
}