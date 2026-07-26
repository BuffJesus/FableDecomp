#include <cstdio>
int __fastcall CCreatureAction_SitDown_GetAnimExtraFlags()
{
    return 8;
}
int main()
{
    if (CCreatureAction_SitDown_GetAnimExtraFlags() == 8) { std::printf("AUTO_TINY_008aba50_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008aba50_TEST FAIL\n");
    return 1;
}