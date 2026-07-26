#include <cstdio>
int __fastcall CCombatSequence_BanditAttackMiddle_GetZone()
{
    return 2;
}
int main()
{
    if (CCombatSequence_BanditAttackMiddle_GetZone() == 2) { std::printf("AUTO_TINY_008b37b0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b37b0_TEST FAIL\n");
    return 1;
}