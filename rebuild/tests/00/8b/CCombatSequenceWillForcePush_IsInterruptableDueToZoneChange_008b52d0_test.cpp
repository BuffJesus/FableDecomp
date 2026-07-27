#include <cstdio>
bool __fastcall CCombatSequence_WillForcePush_IsInterruptableDueToZoneChange()
{
    return false;
}
int main()
{
    if (CCombatSequence_WillForcePush_IsInterruptableDueToZoneChange() == false) { std::printf("AUTO_TINY_008b52d0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b52d0_TEST FAIL\n");
    return 1;
}