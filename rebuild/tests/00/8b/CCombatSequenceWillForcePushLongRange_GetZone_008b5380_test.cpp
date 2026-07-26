#include <cstdio>
int __fastcall CCombatSequence_WillForcePushLongRange_GetZone()
{
    return 1;
}
int main()
{
    if (CCombatSequence_WillForcePushLongRange_GetZone() == 1) { std::printf("AUTO_TINY_008b5380_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b5380_TEST FAIL\n");
    return 1;
}