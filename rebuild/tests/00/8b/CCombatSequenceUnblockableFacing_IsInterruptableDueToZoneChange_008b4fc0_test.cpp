#include <cstdio>
bool __fastcall CCombatSequenceUnblockableFacing_IsInterruptableDueToZoneChange()
{
    return false;
}
int main()
{
    if (CCombatSequenceUnblockableFacing_IsInterruptableDueToZoneChange() == false) { std::printf("AUTO_TINY_008b4fc0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b4fc0_TEST FAIL\n");
    return 1;
}