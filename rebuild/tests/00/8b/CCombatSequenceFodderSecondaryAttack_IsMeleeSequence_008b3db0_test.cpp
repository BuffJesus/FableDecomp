#include <cstdio>
bool __fastcall CCombatSequence_FodderSecondaryAttack_IsMeleeSequence()
{
    return true;
}
int main()
{
    if (CCombatSequence_FodderSecondaryAttack_IsMeleeSequence() == true) { std::printf("AUTO_TINY_008b3db0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b3db0_TEST FAIL\n");
    return 1;
}