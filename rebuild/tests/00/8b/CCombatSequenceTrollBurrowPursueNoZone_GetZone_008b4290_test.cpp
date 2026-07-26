#include <cstdio>
int __fastcall CCombatSequence_TrollBurrowPursueNoZone_GetZone()
{
    return 64;
}
int main()
{
    if (CCombatSequence_TrollBurrowPursueNoZone_GetZone() == 64) { std::printf("AUTO_TINY_008b4290_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b4290_TEST FAIL\n");
    return 1;
}