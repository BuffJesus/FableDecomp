#include <cstdio>
bool __fastcall CCombatSequence_KingScorpionTailPlunge_IsInterruptable()
{
    return false;
}
int main()
{
    if (CCombatSequence_KingScorpionTailPlunge_IsInterruptable() == false) { std::printf("AUTO_TINY_008b3f90_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b3f90_TEST FAIL\n");
    return 1;
}