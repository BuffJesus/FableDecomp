#include <cstdio>
int __fastcall CCombatSequence_TrollProjectileAttack_GetZone()
{
    return 2;
}
int main()
{
    if (CCombatSequence_TrollProjectileAttack_GetZone() == 2) { std::printf("AUTO_TINY_008b41a0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b41a0_TEST FAIL\n");
    return 1;
}