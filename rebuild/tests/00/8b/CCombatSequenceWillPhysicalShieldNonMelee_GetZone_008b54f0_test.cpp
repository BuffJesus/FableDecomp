#include <cstdio>
int __fastcall CCombatSequence_WillPhysicalShieldNonMelee_GetZone()
{
    return 3;
}
int main()
{
    if (CCombatSequence_WillPhysicalShieldNonMelee_GetZone() == 3) { std::printf("AUTO_TINY_008b54f0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b54f0_TEST FAIL\n");
    return 1;
}