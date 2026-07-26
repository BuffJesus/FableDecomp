#include <cstdio>
int __fastcall CCreatureAction_PickUpGenericBox_GetAnimExtraFlags()
{
    return 8;
}
int main()
{
    if (CCreatureAction_PickUpGenericBox_GetAnimExtraFlags() == 8) { std::printf("AUTO_TINY_0084b390_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0084b390_TEST FAIL\n");
    return 1;
}