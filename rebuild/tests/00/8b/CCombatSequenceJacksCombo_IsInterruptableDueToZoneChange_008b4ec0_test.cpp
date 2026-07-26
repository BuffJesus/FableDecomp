#include <cstdio>
bool __fastcall CCombatSequenceJacksCombo_IsInterruptableDueToZoneChange()
{
    return false;
}
int main()
{
    if (CCombatSequenceJacksCombo_IsInterruptableDueToZoneChange() == false) { std::printf("AUTO_TINY_008b4ec0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b4ec0_TEST FAIL\n");
    return 1;
}