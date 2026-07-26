#include <cstdio>
int __fastcall CCombatSequence_FodderUnblockableMainAttack_GetZone()
{
    return 3;
}
int main()
{
    if (CCombatSequence_FodderUnblockableMainAttack_GetZone() == 3) { std::printf("AUTO_TINY_008b3d20_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b3d20_TEST FAIL\n");
    return 1;
}