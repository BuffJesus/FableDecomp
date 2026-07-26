#include <cstdio>
int __fastcall CCreatureAbilityDef_GetSizeofClass()
{
    return 64;
}
int main()
{
    if (CCreatureAbilityDef_GetSizeofClass() == 64) { std::printf("AUTO_TINY_0045934f_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0045934f_TEST FAIL\n");
    return 1;
}