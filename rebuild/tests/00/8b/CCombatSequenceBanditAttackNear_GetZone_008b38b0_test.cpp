#include <cstdio>
int __fastcall CCombatSequence_BanditAttackNear_GetZone()
{
    return 4;
}
int main()
{
    if (CCombatSequence_BanditAttackNear_GetZone() == 4) { std::printf("AUTO_TINY_008b38b0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b38b0_TEST FAIL\n");
    return 1;
}