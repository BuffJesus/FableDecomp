#include <cstdio>
int __fastcall CCombatSequence_VillagerStepIn_GetZone()
{
    return 2;
}
int main()
{
    if (CCombatSequence_VillagerStepIn_GetZone() == 2) { std::printf("AUTO_TINY_008b46c0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b46c0_TEST FAIL\n");
    return 1;
}