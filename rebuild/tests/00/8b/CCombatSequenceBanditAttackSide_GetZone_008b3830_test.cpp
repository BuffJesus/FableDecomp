#include <cstdio>
int __fastcall CCombatSequence_BanditAttackSide_GetZone()
{
    return 56;
}
int main()
{
    if (CCombatSequence_BanditAttackSide_GetZone() == 56) { std::printf("AUTO_TINY_008b3830_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b3830_TEST FAIL\n");
    return 1;
}