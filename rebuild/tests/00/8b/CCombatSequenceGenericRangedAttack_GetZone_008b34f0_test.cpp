#include <cstdio>
int __fastcall CCombatSequence_GenericRangedAttack_GetZone()
{
    return 128;
}
int main()
{
    if (CCombatSequence_GenericRangedAttack_GetZone() == 128) { std::printf("AUTO_TINY_008b34f0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b34f0_TEST FAIL\n");
    return 1;
}