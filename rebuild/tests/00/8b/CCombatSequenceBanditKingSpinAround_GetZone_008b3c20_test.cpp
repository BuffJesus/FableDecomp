#include <cstdio>
int __fastcall CCombatSequence_BanditKingSpinAround_GetZone()
{
    return 32;
}
int main()
{
    if (CCombatSequence_BanditKingSpinAround_GetZone() == 32) { std::printf("AUTO_TINY_008b3c20_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b3c20_TEST FAIL\n");
    return 1;
}