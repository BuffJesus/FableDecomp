#include <cstdio>
int __fastcall CCombatSequence_BanditAttackMiddle_GetCombatSequenceType()
{
    return 1;
}
int main()
{
    if (CCombatSequence_BanditAttackMiddle_GetCombatSequenceType() == 1) { std::printf("AUTO_TINY_008b37c0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b37c0_TEST FAIL\n");
    return 1;
}