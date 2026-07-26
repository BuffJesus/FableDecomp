#include <cstdio>
bool __fastcall CCombatSequence_WillLightning_IsInterruptableDueToZoneChange()
{
    return false;
}
int main()
{
    if (CCombatSequence_WillLightning_IsInterruptableDueToZoneChange() == false) { std::printf("AUTO_TINY_008b5160_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b5160_TEST FAIL\n");
    return 1;
}