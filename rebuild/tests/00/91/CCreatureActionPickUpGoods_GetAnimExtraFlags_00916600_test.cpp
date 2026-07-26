#include <cstdio>
int __fastcall CCreatureAction_PickUpGoods_GetAnimExtraFlags()
{
    return 8;
}
int main()
{
    if (CCreatureAction_PickUpGoods_GetAnimExtraFlags() == 8) { std::printf("AUTO_TINY_00916600_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00916600_TEST FAIL\n");
    return 1;
}