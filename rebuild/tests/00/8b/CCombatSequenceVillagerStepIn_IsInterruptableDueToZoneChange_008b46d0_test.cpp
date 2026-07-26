#include <cstdio>
bool __fastcall CCombatSequence_VillagerStepIn_IsInterruptableDueToZoneChange()
{
    return true;
}
int main()
{
    if (CCombatSequence_VillagerStepIn_IsInterruptableDueToZoneChange() == true) { std::printf("AUTO_TINY_008b46d0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b46d0_TEST FAIL\n");
    return 1;
}