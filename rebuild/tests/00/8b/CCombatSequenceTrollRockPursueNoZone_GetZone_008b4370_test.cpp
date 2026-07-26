#include <cstdio>
int __fastcall CCombatSequence_TrollRockPursueNoZone_GetZone()
{
    return 64;
}
int main()
{
    if (CCombatSequence_TrollRockPursueNoZone_GetZone() == 64) { std::printf("AUTO_TINY_008b4370_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b4370_TEST FAIL\n");
    return 1;
}