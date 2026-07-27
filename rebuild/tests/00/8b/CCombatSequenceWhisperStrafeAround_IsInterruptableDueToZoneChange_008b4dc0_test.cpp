#include <cstdio>
bool __fastcall CCombatSequenceWhisperStrafeAround_IsInterruptableDueToZoneChange()
{
    return false;
}
int main()
{
    if (CCombatSequenceWhisperStrafeAround_IsInterruptableDueToZoneChange() == false) { std::printf("AUTO_TINY_008b4dc0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b4dc0_TEST FAIL\n");
    return 1;
}