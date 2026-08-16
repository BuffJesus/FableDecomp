#include <cstdio>
int __fastcall CCombatSequence_BanditAttackFar_GetCombatSequenceType()
{
    return 1;
}
int main()
{
    if (CCombatSequence_BanditAttackFar_GetCombatSequenceType() == 1) { std::printf("AUTO_TINY_008b3740_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b3740_TEST FAIL\n");
    return 1;
}