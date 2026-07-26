#include <cstdio>
int __fastcall CCreatureAction_DropGenericBox_GetAnimExtraFlags()
{
    return 8;
}
int main()
{
    if (CCreatureAction_DropGenericBox_GetAnimExtraFlags() == 8) { std::printf("AUTO_TINY_0084a820_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0084a820_TEST FAIL\n");
    return 1;
}