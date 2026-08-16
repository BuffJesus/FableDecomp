#include <cstdio>
int __fastcall CCombatSequence_BanditAttackNear_GetCombatSequenceType()
{
    return 1;
}
int main()
{
    if (CCombatSequence_BanditAttackNear_GetCombatSequenceType() == 1) { std::printf("AUTO_TINY_008b38c0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b38c0_TEST FAIL\n");
    return 1;
}