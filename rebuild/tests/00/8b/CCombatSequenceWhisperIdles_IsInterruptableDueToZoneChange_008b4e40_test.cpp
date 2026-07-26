#include <cstdio>
bool __fastcall CCombatSequenceWhisperIdles_IsInterruptableDueToZoneChange()
{
    return true;
}
int main()
{
    if (CCombatSequenceWhisperIdles_IsInterruptableDueToZoneChange() == true) { std::printf("AUTO_TINY_008b4e40_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b4e40_TEST FAIL\n");
    return 1;
}