#include <cstdio>
int __fastcall CCreatureAction_PlayHopscotch_GetAnimExtraFlags()
{
    return 8;
}
int main()
{
    if (CCreatureAction_PlayHopscotch_GetAnimExtraFlags() == 8) { std::printf("AUTO_TINY_00941ba0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00941ba0_TEST FAIL\n");
    return 1;
}