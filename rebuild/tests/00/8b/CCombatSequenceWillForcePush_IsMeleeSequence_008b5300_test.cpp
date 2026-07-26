#include <cstdio>
bool __fastcall CCombatSequence_WillForcePush_IsMeleeSequence()
{
    return true;
}
int main()
{
    if (CCombatSequence_WillForcePush_IsMeleeSequence() == true) { std::printf("AUTO_TINY_008b5300_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b5300_TEST FAIL\n");
    return 1;
}