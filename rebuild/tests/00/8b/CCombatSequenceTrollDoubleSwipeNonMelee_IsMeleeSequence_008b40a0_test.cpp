#include <cstdio>
bool __fastcall CCombatSequence_TrollDoubleSwipeNonMelee_IsMeleeSequence()
{
    return false;
}
int main()
{
    if (CCombatSequence_TrollDoubleSwipeNonMelee_IsMeleeSequence() == false) { std::printf("AUTO_TINY_008b40a0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b40a0_TEST FAIL\n");
    return 1;
}