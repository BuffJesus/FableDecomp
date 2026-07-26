#include <cstdio>
int __fastcall CCreatureAction_GetInBed_GetAnimExtraFlags()
{
    return 8;
}
int main()
{
    if (CCreatureAction_GetInBed_GetAnimExtraFlags() == 8) { std::printf("AUTO_TINY_0092e3c0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0092e3c0_TEST FAIL\n");
    return 1;
}