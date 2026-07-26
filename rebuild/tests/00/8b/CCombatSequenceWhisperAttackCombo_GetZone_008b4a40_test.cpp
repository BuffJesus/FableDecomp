#include <cstdio>
int __fastcall CCombatSequenceWhisperAttackCombo_GetZone()
{
    return 2;
}
int main()
{
    if (CCombatSequenceWhisperAttackCombo_GetZone() == 2) { std::printf("AUTO_TINY_008b4a40_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b4a40_TEST FAIL\n");
    return 1;
}