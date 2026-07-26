#include <cstdio>
int __fastcall CCombatSequence_BanditKingAttack_GetZone()
{
    return 2;
}
int main()
{
    if (CCombatSequence_BanditKingAttack_GetZone() == 2) { std::printf("AUTO_TINY_008b3a90_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b3a90_TEST FAIL\n");
    return 1;
}