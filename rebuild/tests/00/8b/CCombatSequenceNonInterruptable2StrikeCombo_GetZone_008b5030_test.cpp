#include <cstdio>
int __fastcall CCombatSequenceNonInterruptable2StrikeCombo_GetZone()
{
    return 2;
}
int main()
{
    if (CCombatSequenceNonInterruptable2StrikeCombo_GetZone() == 2) { std::printf("AUTO_TINY_008b5030_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b5030_TEST FAIL\n");
    return 1;
}