#include <cstdio>
bool __fastcall CCombatSequence_WillForcePush_IsInterruptable()
{
    return false;
}
int main()
{
    if (CCombatSequence_WillForcePush_IsInterruptable() == false) { std::printf("AUTO_TINY_008b52e0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b52e0_TEST FAIL\n");
    return 1;
}