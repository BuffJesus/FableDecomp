#include <cstdio>
int __fastcall CCombatSequence_BanditKingBackOff_GetZone()
{
    return 4;
}
int main()
{
    if (CCombatSequence_BanditKingBackOff_GetZone() == 4) { std::printf("AUTO_TINY_008b3ba0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b3ba0_TEST FAIL\n");
    return 1;
}