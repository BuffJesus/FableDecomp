#include <cstdio>
int __fastcall CCombatSequence_VillagerBackOff_GetZone()
{
    return 4;
}
int main()
{
    if (CCombatSequence_VillagerBackOff_GetZone() == 4) { std::printf("AUTO_TINY_008b4650_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b4650_TEST FAIL\n");
    return 1;
}