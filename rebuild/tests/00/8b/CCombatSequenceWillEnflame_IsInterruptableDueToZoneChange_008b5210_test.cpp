#include <cstdio>
bool __fastcall CCombatSequence_WillEnflame_IsInterruptableDueToZoneChange()
{
    return false;
}
int main()
{
    if (CCombatSequence_WillEnflame_IsInterruptableDueToZoneChange() == false) { std::printf("AUTO_TINY_008b5210_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b5210_TEST FAIL\n");
    return 1;
}