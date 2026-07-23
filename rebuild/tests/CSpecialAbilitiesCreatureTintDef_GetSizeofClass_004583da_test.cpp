#include <cstdio>
int __fastcall CSpecialAbilitiesCreatureTintDef_GetSizeofClass()
{
    return 80;
}
int main()
{
    if (CSpecialAbilitiesCreatureTintDef_GetSizeofClass() == 80) { std::printf("AUTO_TINY_004583da_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004583da_TEST FAIL\n");
    return 1;
}