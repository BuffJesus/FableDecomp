#include <cstdio>
int __fastcall CCombatSequence_TrollOpeningGroundPound_GetZone()
{
    return 2;
}
int main()
{
    if (CCombatSequence_TrollOpeningGroundPound_GetZone() == 2) { std::printf("AUTO_TINY_008b4120_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b4120_TEST FAIL\n");
    return 1;
}