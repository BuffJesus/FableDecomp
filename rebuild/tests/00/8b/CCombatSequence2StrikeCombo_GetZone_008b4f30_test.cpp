#include <cstdio>
int __fastcall CCombatSequence2StrikeCombo_GetZone()
{
    return 2;
}
int main()
{
    if (CCombatSequence2StrikeCombo_GetZone() == 2) { std::printf("AUTO_TINY_008b4f30_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b4f30_TEST FAIL\n");
    return 1;
}