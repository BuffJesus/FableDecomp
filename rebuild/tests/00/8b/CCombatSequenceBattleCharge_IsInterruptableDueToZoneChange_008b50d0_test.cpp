#include <cstdio>
bool __fastcall CCombatSequence_BattleCharge_IsInterruptableDueToZoneChange()
{
    return false;
}
int main()
{
    if (CCombatSequence_BattleCharge_IsInterruptableDueToZoneChange() == false) { std::printf("AUTO_TINY_008b50d0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b50d0_TEST FAIL\n");
    return 1;
}