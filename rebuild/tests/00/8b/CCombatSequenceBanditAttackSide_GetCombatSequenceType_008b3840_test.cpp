#include <cstdio>
int __fastcall CCombatSequence_BanditAttackSide_GetCombatSequenceType()
{
    return 1;
}
int main()
{
    if (CCombatSequence_BanditAttackSide_GetCombatSequenceType() == 1) { std::printf("AUTO_TINY_008b3840_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b3840_TEST FAIL\n");
    return 1;
}