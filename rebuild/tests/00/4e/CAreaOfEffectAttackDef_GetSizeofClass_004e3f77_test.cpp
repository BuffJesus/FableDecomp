#include <cstdio>
int __fastcall CAreaOfEffectAttackDef_GetSizeofClass()
{
    return 76;
}
int main()
{
    if (CAreaOfEffectAttackDef_GetSizeofClass() == 76) { std::printf("AUTO_TINY_004e3f77_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004e3f77_TEST FAIL\n");
    return 1;
}