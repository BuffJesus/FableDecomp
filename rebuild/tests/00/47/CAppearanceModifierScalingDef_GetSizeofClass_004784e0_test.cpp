#include <cstdio>
int __fastcall CAppearanceModifierScalingDef_GetSizeofClass()
{
    return 72;
}
int main()
{
    if (CAppearanceModifierScalingDef_GetSizeofClass() == 72) { std::printf("AUTO_TINY_004784e0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004784e0_TEST FAIL\n");
    return 1;
}