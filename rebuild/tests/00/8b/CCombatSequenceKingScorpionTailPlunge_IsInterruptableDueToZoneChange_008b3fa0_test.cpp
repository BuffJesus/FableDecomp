#include <cstdio>
bool __fastcall CCombatSequence_KingScorpionTailPlunge_IsInterruptableDueToZoneChange()
{
    return false;
}
int main()
{
    if (CCombatSequence_KingScorpionTailPlunge_IsInterruptableDueToZoneChange() == false) { std::printf("AUTO_TINY_008b3fa0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b3fa0_TEST FAIL\n");
    return 1;
}