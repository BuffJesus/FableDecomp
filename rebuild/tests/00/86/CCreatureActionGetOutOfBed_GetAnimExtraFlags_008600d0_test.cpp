#include <cstdio>
int __fastcall CCreatureAction_GetOutOfBed_GetAnimExtraFlags()
{
    return 8;
}
int main()
{
    if (CCreatureAction_GetOutOfBed_GetAnimExtraFlags() == 8) { std::printf("AUTO_TINY_008600d0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008600d0_TEST FAIL\n");
    return 1;
}