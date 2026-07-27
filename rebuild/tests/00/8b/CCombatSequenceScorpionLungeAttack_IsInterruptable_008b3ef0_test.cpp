#include <cstdio>
bool __fastcall CCombatSequence_ScorpionLungeAttack_IsInterruptable()
{
    return false;
}
int main()
{
    if (CCombatSequence_ScorpionLungeAttack_IsInterruptable() == false) { std::printf("AUTO_TINY_008b3ef0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b3ef0_TEST FAIL\n");
    return 1;
}