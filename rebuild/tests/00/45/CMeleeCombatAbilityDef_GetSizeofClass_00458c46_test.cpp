#include <cstdio>
int __fastcall CMeleeCombatAbilityDef_GetSizeofClass()
{
    return 124;
}
int main()
{
    if (CMeleeCombatAbilityDef_GetSizeofClass() == 124) { std::printf("AUTO_TINY_00458c46_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00458c46_TEST FAIL\n");
    return 1;
}