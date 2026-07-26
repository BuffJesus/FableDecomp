#include <cstdio>
int __fastcall CCombatSequence_BanditIdleNear_GetZone()
{
    return 4;
}
int main()
{
    if (CCombatSequence_BanditIdleNear_GetZone() == 4) { std::printf("AUTO_TINY_008b3930_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b3930_TEST FAIL\n");
    return 1;
}