#include <cstdio>
int __fastcall CAttackPatternDef_GetSizeofClass()
{
    return 72;
}
int main()
{
    if (CAttackPatternDef_GetSizeofClass() == 72) { std::printf("AUTO_TINY_004584e4_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004584e4_TEST FAIL\n");
    return 1;
}