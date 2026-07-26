#include <cstdio>
int __fastcall CHeroCombatDef_GetSizeofClass()
{
    return 328;
}
int main()
{
    if (CHeroCombatDef_GetSizeofClass() == 328) { std::printf("AUTO_TINY_00458b7b_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00458b7b_TEST FAIL\n");
    return 1;
}