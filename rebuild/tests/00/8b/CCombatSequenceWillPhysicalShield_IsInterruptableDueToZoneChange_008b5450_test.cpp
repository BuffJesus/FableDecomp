#include <cstdio>
bool __fastcall CCombatSequence_WillPhysicalShield_IsInterruptableDueToZoneChange()
{
    return false;
}
int main()
{
    if (CCombatSequence_WillPhysicalShield_IsInterruptableDueToZoneChange() == false) { std::printf("AUTO_TINY_008b5450_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b5450_TEST FAIL\n");
    return 1;
}