#include <cstdio>
bool __fastcall CCombatSequence_WillForcePushLongRange_IsMeleeSequence()
{
    return true;
}
int main()
{
    if (CCombatSequence_WillForcePushLongRange_IsMeleeSequence() == true) { std::printf("AUTO_TINY_008b53c0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b53c0_TEST FAIL\n");
    return 1;
}