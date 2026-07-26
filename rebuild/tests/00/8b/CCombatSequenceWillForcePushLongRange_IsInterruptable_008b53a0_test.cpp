#include <cstdio>
bool __fastcall CCombatSequence_WillForcePushLongRange_IsInterruptable()
{
    return false;
}
int main()
{
    if (CCombatSequence_WillForcePushLongRange_IsInterruptable() == false) { std::printf("AUTO_TINY_008b53a0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b53a0_TEST FAIL\n");
    return 1;
}