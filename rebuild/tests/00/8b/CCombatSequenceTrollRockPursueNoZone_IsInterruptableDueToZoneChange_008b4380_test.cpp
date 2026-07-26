#include <cstdio>
bool __fastcall CCombatSequence_TrollRockPursueNoZone_IsInterruptableDueToZoneChange()
{
    return true;
}
int main()
{
    if (CCombatSequence_TrollRockPursueNoZone_IsInterruptableDueToZoneChange() == true) { std::printf("AUTO_TINY_008b4380_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b4380_TEST FAIL\n");
    return 1;
}