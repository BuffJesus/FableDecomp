#include <cstdio>
bool __fastcall CCombatSequenceNonInterruptable2StrikeCombo_IsInterruptableDueToZoneChange()
{
    return false;
}
int main()
{
    if (CCombatSequenceNonInterruptable2StrikeCombo_IsInterruptableDueToZoneChange() == false) { std::printf("AUTO_TINY_008b5040_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b5040_TEST FAIL\n");
    return 1;
}