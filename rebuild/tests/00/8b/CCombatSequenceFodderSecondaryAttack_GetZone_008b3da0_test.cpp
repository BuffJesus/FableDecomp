#include <cstdio>
int __fastcall CCombatSequence_FodderSecondaryAttack_GetZone()
{
    return 7;
}
int main()
{
    if (CCombatSequence_FodderSecondaryAttack_GetZone() == 7) { std::printf("AUTO_TINY_008b3da0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b3da0_TEST FAIL\n");
    return 1;
}