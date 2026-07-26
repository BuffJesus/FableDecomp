#include <cstdio>
int __fastcall CAICreatureWillPowerIndicatorDef_GetSizeofClass()
{
    return 44;
}
int main()
{
    if (CAICreatureWillPowerIndicatorDef_GetSizeofClass() == 44) { std::printf("AUTO_TINY_004d5e10_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d5e10_TEST FAIL\n");
    return 1;
}