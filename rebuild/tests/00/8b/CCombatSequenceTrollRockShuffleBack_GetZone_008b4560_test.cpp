#include <cstdio>
int __fastcall CCombatSequence_TrollRockShuffleBack_GetZone()
{
    return 4;
}
int main()
{
    if (CCombatSequence_TrollRockShuffleBack_GetZone() == 4) { std::printf("AUTO_TINY_008b4560_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b4560_TEST FAIL\n");
    return 1;
}