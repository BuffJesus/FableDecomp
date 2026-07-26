#include <cstdio>
bool __fastcall CCombatSequence_VillagerAdvance_IsInterruptableDueToZoneChange()
{
    return true;
}
int main()
{
    if (CCombatSequence_VillagerAdvance_IsInterruptableDueToZoneChange() == true) { std::printf("AUTO_TINY_008b4750_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b4750_TEST FAIL\n");
    return 1;
}