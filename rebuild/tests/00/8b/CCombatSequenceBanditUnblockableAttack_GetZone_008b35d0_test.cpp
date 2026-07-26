#include <cstdio>
int __fastcall CCombatSequence_BanditUnblockableAttack_GetZone()
{
    return 7;
}
int main()
{
    if (CCombatSequence_BanditUnblockableAttack_GetZone() == 7) { std::printf("AUTO_TINY_008b35d0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b35d0_TEST FAIL\n");
    return 1;
}