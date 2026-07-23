#include <cstdio>
int __fastcall CMeleeCombatKnockdownEffects_GetSizeofClass()
{
    return 92;
}
int main()
{
    if (CMeleeCombatKnockdownEffects_GetSizeofClass() == 92) { std::printf("AUTO_TINY_00458c99_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00458c99_TEST FAIL\n");
    return 1;
}