#include <cstdio>
int __fastcall CCombatSequence_BanditBoast_GetCombatSequenceType()
{
    return 3;
}
int main()
{
    if (CCombatSequence_BanditBoast_GetCombatSequenceType() == 3) { std::printf("AUTO_TINY_008b3a20_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b3a20_TEST FAIL\n");
    return 1;
}