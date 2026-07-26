#include <cstdio>
int __fastcall CCombatSequenceWhisperAttackBigCombo_GetZone()
{
    return 2;
}
int main()
{
    if (CCombatSequenceWhisperAttackBigCombo_GetZone() == 2) { std::printf("AUTO_TINY_008b4ac0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b4ac0_TEST FAIL\n");
    return 1;
}