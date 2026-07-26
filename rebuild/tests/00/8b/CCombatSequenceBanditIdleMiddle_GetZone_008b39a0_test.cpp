#include <cstdio>
int __fastcall CCombatSequence_BanditIdleMiddle_GetZone()
{
    return 2;
}
int main()
{
    if (CCombatSequence_BanditIdleMiddle_GetZone() == 2) { std::printf("AUTO_TINY_008b39a0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b39a0_TEST FAIL\n");
    return 1;
}