#include <cstdio>
bool __fastcall CCombatSequence_WillForcePushLongRange_IsInterruptableDueToZoneChange()
{
    return false;
}
int main()
{
    if (CCombatSequence_WillForcePushLongRange_IsInterruptableDueToZoneChange() == false) { std::printf("AUTO_TINY_008b5390_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b5390_TEST FAIL\n");
    return 1;
}