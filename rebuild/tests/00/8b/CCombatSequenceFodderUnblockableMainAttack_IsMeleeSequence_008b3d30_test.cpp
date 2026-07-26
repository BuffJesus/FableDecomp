#include <cstdio>
bool __fastcall CCombatSequence_FodderUnblockableMainAttack_IsMeleeSequence()
{
    return true;
}
int main()
{
    if (CCombatSequence_FodderUnblockableMainAttack_IsMeleeSequence() == true) { std::printf("AUTO_TINY_008b3d30_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b3d30_TEST FAIL\n");
    return 1;
}