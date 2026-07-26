#include <cstdio>
bool __fastcall CCombatSequence_BanditKingAttack_IsMeleeSequence()
{
    return true;
}
int main()
{
    if (CCombatSequence_BanditKingAttack_IsMeleeSequence() == true) { std::printf("AUTO_TINY_008b3aa0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b3aa0_TEST FAIL\n");
    return 1;
}