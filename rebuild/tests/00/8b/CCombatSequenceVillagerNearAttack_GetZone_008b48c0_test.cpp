#include <cstdio>
int __fastcall CCombatSequence_VillagerNearAttack_GetZone()
{
    return 4;
}
int main()
{
    if (CCombatSequence_VillagerNearAttack_GetZone() == 4) { std::printf("AUTO_TINY_008b48c0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b48c0_TEST FAIL\n");
    return 1;
}