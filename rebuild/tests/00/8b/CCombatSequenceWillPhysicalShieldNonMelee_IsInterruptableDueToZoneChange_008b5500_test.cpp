#include <cstdio>
bool __fastcall CCombatSequence_WillPhysicalShieldNonMelee_IsInterruptableDueToZoneChange()
{
    return false;
}
int main()
{
    if (CCombatSequence_WillPhysicalShieldNonMelee_IsInterruptableDueToZoneChange() == false) { std::printf("AUTO_TINY_008b5500_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b5500_TEST FAIL\n");
    return 1;
}