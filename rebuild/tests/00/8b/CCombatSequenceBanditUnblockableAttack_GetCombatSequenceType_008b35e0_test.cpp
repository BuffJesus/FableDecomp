#include <cstdio>
int __fastcall CCombatSequence_BanditUnblockableAttack_GetCombatSequenceType()
{
    return 1;
}
int main()
{
    if (CCombatSequence_BanditUnblockableAttack_GetCombatSequenceType() == 1) { std::printf("AUTO_TINY_008b35e0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b35e0_TEST FAIL\n");
    return 1;
}