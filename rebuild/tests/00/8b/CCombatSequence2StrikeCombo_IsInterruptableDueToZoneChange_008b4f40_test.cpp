#include <cstdio>
bool __fastcall CCombatSequence2StrikeCombo_IsInterruptableDueToZoneChange()
{
    return false;
}
int main()
{
    if (CCombatSequence2StrikeCombo_IsInterruptableDueToZoneChange() == false) { std::printf("AUTO_TINY_008b4f40_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b4f40_TEST FAIL\n");
    return 1;
}