#include <cstdio>
bool __fastcall CCombatSequence_FodderCombatIdle_IsMeleeSequence()
{
    return false;
}
int main()
{
    if (CCombatSequence_FodderCombatIdle_IsMeleeSequence() == false) { std::printf("AUTO_TINY_008b3e30_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b3e30_TEST FAIL\n");
    return 1;
}