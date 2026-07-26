#include <cstdio>
bool __fastcall CCombatSequence_FodderCombatIdle_IsInterruptableDueToZoneChange()
{
    return true;
}
int main()
{
    if (CCombatSequence_FodderCombatIdle_IsInterruptableDueToZoneChange() == true) { std::printf("AUTO_TINY_008b3e40_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b3e40_TEST FAIL\n");
    return 1;
}